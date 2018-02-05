#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "anim.hpp"

class anim_starry_night : public animation {
public:
    static const uint16_t STAR_COUNT = 32;
    static const long FADE_TIME_MIN = 1000;
    static const long FADE_TIME = 2000;

    anim_starry_night(output * out) : animation(out), anims(STAR_COUNT) {
        for (uint16_t i = 0; i < STAR_COUNT; i++) {
            anims.StartAnimation(i, FADE_TIME_MIN + random(FADE_TIME),
                std::bind(&anim_starry_night::FadeOutAnimUpdate, this, std::placeholders::_1));
        }
    }

    void loop() {
        anims.UpdateAnimations();
    }

    ~anim_starry_night() {
        anims.StopAll();
    }

private:
    void FadeOutAnimUpdate(const AnimationParam& param) {

        switch (param.state) {
            case AnimationState_Progress:
                set_pixel(indices[param.index],
                    RgbwColor::LinearBlend(RgbwColor(255), RgbwColor(0), fabs(param.progress - 0.5) * 2));
                break;

            case AnimationState_Started:
                {
                    // find a pixel that is currently not animated
                    bool again = true;
                    uint16_t next_pixel;
                    while (again) {
                        again = false;
                        next_pixel = random(PIXEL_COUNT);

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
                set_pixel(indices[param.index], RgbwColor(0));
                anims.StartAnimation(param.index, FADE_TIME_MIN + random(FADE_TIME),
                    std::bind(&anim_starry_night::FadeOutAnimUpdate, this, std::placeholders::_1));
                break;

        }
    }

    NeoPixelAnimator anims;
    uint16_t indices[STAR_COUNT];
};
