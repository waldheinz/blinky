#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include <functional>

#include "anim.hpp"

class anim_lighthouse : public animation {
public:
    static const uint16_t NextPixelMoveDuration = 100 / PIXEL_COUNT; // how fast we move through the pixels
    static const uint16_t AnimCount = PIXEL_COUNT / 5 * 2 + 1; // we only need enough animations for the tail and one extra
    static const uint16_t PixelFadeDuration = 300; // third of a second

    anim_lighthouse() : animations(AnimCount) {
        // we use the index 0 animation to time how often we move to the next
        // pixel in the strip
        animations.StartAnimation(0, NextPixelMoveDuration,
            std::bind(&anim_lighthouse::LoopAnimUpdate, this, std::placeholders::_1));
    }

    ~anim_lighthouse() {
        animations.StopAll();
    }

    void loop() override {
        animations.UpdateAnimations();
    }

private:
    struct MyAnimationState {
        RgbColor StartingColor;
        RgbColor EndingColor;
        uint16_t IndexPixel; // which pixel this animation is effecting
    };

    void FadeOutAnimUpdate(const AnimationParam& param) {
        // this gets called for each animation on every time step
        // progress will start at 0.0 and end at 1.0
        // we use the blend function on the RgbColor to mix
        // color based on the progress given to us in the animation
        RgbColor updatedColor = RgbColor::LinearBlend(
            animationState[param.index].StartingColor,
            animationState[param.index].EndingColor,
            param.progress);
        // apply the color to the strip
        strip.SetPixelColor(animationState[param.index].IndexPixel,
            colorGamma.Correct(updatedColor));
    }

    void LoopAnimUpdate(const AnimationParam& param) {
        // wait for this animation to complete,
        // we are using it as a timer of sorts
        if (param.state == AnimationState_Completed) {
            // done, time to restart this position tracking animation/timer
            animations.RestartAnimation(param.index);

            // pick the next pixel inline to start animating
            //
            frontPixel = (frontPixel + 1) % PIXEL_COUNT; // increment and wrap
            if (frontPixel == 0) {
                // we looped, lets pick a new front color
                frontColor = HslColor(random(360) / 360.0f, 1.0f, 0.15f);
            }

            uint16_t indexAnim;
            // do we have an animation available to use to animate the next front pixel?
            // if you see skipping, then either you are going to fast or need to increase
            // the number of animation channels
            if (animations.NextAvailableAnimation(&indexAnim, 1)) {
                animationState[indexAnim].StartingColor = frontColor;
                animationState[indexAnim].EndingColor = RgbColor(0, 0, 0);
                animationState[indexAnim].IndexPixel = frontPixel;

                animations.StartAnimation(indexAnim, PixelFadeDuration,
                    std::bind(&anim_lighthouse::FadeOutAnimUpdate, this, std::placeholders::_1));
            }
        }
    }

    NeoPixelAnimator animations; // NeoPixel animation management object
    MyAnimationState animationState[AnimCount];
    uint16_t frontPixel = 0;  // the front of the loop
    RgbColor frontColor;  // the color at the front of the loop
};
