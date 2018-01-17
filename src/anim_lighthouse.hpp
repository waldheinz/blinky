#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include <functional>

#include "anim.hpp"

class anim_lighthouse final : public animation {
public:

    anim_lighthouse() : animations(1) {
        animations.StartAnimation(0, 2000,
            std::bind(&anim_lighthouse::animate, this, std::placeholders::_1));
    }

    ~anim_lighthouse() {
        animations.StopAll();
    }

    void loop() override {
        animations.UpdateAnimations();
    }

private:
    void animate(const AnimationParam& param) {
        if (param.state == AnimationState_Completed) {
            animations.RestartAnimation(param.index);
        } else if (param.state == AnimationState_Progress) {
            for (int col = 0; col < COLUMNS; col++) {
                const float d1 = param.progress * M_PI * 2;
                const float d2 = ((float)col / COLUMNS) * M_PI * 2;
                const float d = fabsf(sin(d1 - d2));
                const float de = NeoEase::CubicInOut(d);
                const RgbwColor c = colorGamma.Correct(RgbwColor::LinearBlend(RgbwColor(0), RgbwColor(96), de));

                for (int row = 0; row < ROWS; row++) {
                    strip.SetPixelColor(to_index(col, row), c);
                }
            }
        }
    }

    NeoPixelAnimator animations; // NeoPixel animation management object
    uint16_t frontPixel = 0;  // the front of the loop
    RgbColor frontColor;  // the color at the front of the loop
};
