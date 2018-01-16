// NeoPixelFunLoop
// This example will move a trail of light around a series of pixels.
// A ring formation of pixels looks best.
// The trail will have a slowly fading tail.
//
// This will demonstrate the use of the NeoPixelAnimator.
// It shows the advanced use an animation to control the modification and
// starting of other animations.
// It also shows the normal use of animating colors.
// It also demonstrates the ability to share an animation channel rather than
// hard code them to pixels.
//

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

const uint16_t PixelCount = 300; // make sure to set this to the number of pixels in your strip
const uint16_t PixelPin = 25;  // make sure to set this to the correct pin, ignored for Esp8266
const uint16_t AnimCount = PixelCount / 5 * 2 + 1; // we only need enough animations for the tail and one extra

const uint16_t PixelFadeDuration = 300; // third of a second
// one second divide by the number of pixels = loop once a second
const uint16_t NextPixelMoveDuration = 100 / PixelCount; // how fast we move through the pixels

NeoGamma<NeoGammaTableMethod> colorGamma; // for any fade animations, best to correct gamma

NeoPixelBus<NeoGrbwFeature, NeoEsp32BitBang800KbpsMethod> strip(PixelCount, PixelPin);

class Animation {
public:
    virtual ~Animation() { }

    virtual void loop() { }
};

class AnimStarryNight : public Animation {
public:
    static const uint16_t STAR_COUNT = 16;
    static const long FADE_TIME = 2000;

    AnimStarryNight() : anims(STAR_COUNT) {
        for (uint16_t i = 0; i < STAR_COUNT; i++) {
            anims.StartAnimation(i, random(FADE_TIME),
                std::bind(&AnimStarryNight::FadeOutAnimUpdate, this, std::placeholders::_1));
        }
    }

    void loop() {
        anims.UpdateAnimations();
    }

    ~AnimStarryNight() {
        anims.StopAll();
    }

private:
    void FadeOutAnimUpdate(const AnimationParam& param) {

        switch (param.state) {
            case AnimationState_Progress:
                strip.SetPixelColor(indices[param.index],
//                    colorGamma.Correct(
                        RgbwColor::LinearBlend(
                            RgbwColor(90), RgbwColor(0), param.progress));
                break;

            case AnimationState_Started:
                {
                    // find a pixel that is currently not animated
                    bool again = true;
                    uint16_t next_pixel;
                    while (again) {
                        again = false;
                        next_pixel = random(PixelCount);

                        for (auto p : indices) {
                            if (p == next_pixel) {
                                again = true;
                                break;
                            }
                        }
                    }

                    indices[param.index] = next_pixel;
                }

                break;

            case AnimationState_Completed:
                strip.SetPixelColor(indices[param.index], RgbwColor(0));
                anims.StartAnimation(param.index, random(FADE_TIME),
                    std::bind(&AnimStarryNight::FadeOutAnimUpdate, this, std::placeholders::_1));
                break;

        }
    }

    NeoPixelAnimator anims;
    uint16_t indices[STAR_COUNT];
};

class AnimLightHouse : public Animation {
public:
    AnimLightHouse() : animations(AnimCount) {
        // we use the index 0 animation to time how often we move to the next
        // pixel in the strip
        animations.StartAnimation(0, NextPixelMoveDuration,
            std::bind(&AnimLightHouse::LoopAnimUpdate, this, std::placeholders::_1));
    }

    ~AnimLightHouse() {
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
            frontPixel = (frontPixel + 1) % PixelCount; // increment and wrap
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
                    std::bind(&AnimLightHouse::FadeOutAnimUpdate, this, std::placeholders::_1));
            }
        }
    }

    NeoPixelAnimator animations; // NeoPixel animation management object
    MyAnimationState animationState[AnimCount];
    uint16_t frontPixel = 0;  // the front of the loop
    RgbColor frontColor;  // the color at the front of the loop
};

void SetRandomSeed() {
    uint32_t seed;

    // random works best with a seed that can use 31 bits
    // analogRead on a unconnected pin tends toward less than four bits
    seed = analogRead(0);
    delay(1);

    for (int shifts = 3; shifts < 31; shifts += 3)
    {
        seed ^= analogRead(0) << shifts;
        delay(1);
    }

    // Serial.println(seed);
    randomSeed(seed);
}

Animation* anim = nullptr;

void setup() {
    strip.Begin();
    delay(2);
    strip.Show();

    SetRandomSeed();
}

void loop() {
    // this is all that is needed to keep it running
    // and avoiding using delay() is always a good thing for
    // any timing related routines
    if (!anim) {
        anim = new AnimStarryNight();
    }

    anim->loop();
    delay(1);
    strip.Show();
}
