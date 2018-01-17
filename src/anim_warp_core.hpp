#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include <functional>

#include "anim.hpp"

class anim_warp_core : public animation {
public:
    static const int PULSE_COUNT = 4;
    static constexpr float PULSE_WIDTH = 1.5;

    anim_warp_core() : anims(PULSE_COUNT + 1)  {
        anims.StartAnimation(0, random(500) + 1000,
            std::bind(&anim_warp_core::start_pulse, this, std::placeholders::_1));
    }

    ~anim_warp_core() {
        anims.StopAll();
    }

    void loop() override {
        anims.UpdateAnimations();
    }

private:
    void start_pulse(AnimationParam const & param) {
        if (param.state == AnimationState_Started) {
            uint16_t indexAnim;
            if (anims.NextAvailableAnimation(&indexAnim, 1)) {
                pulses[indexAnim].pulse_center = 0;

                anims.StartAnimation(indexAnim, 3000,
                    std::bind(&anim_warp_core::move_pulse, this, std::placeholders::_1));
            }
        } else if (param.state == AnimationState_Completed) {
            anims.StartAnimation(0, random(500) + 1000,
                std::bind(&anim_warp_core::start_pulse, this, std::placeholders::_1));
        }
    }

    void move_pulse(AnimationParam const & param) {
        if (param.state == AnimationState_Progress) {

            const float ideal_center = (ROWS + 8) * param.progress - 4.0f;
            const int8_t row_center = ideal_center;

            for (int8_t row = row_center - 2; row < row_center + 2; row++) {
                if (row < 0 | row >= ROWS) {
                    continue;
                }

                const float d = fabsf(ideal_center - (float)row - PULSE_WIDTH) / PULSE_WIDTH;
                const auto color = d > 0 && d <= 1
                    ? RgbwColor::LinearBlend(RgbwColor(0,0,255), RgbwColor(0, 0, 32), d * d)
                    : RgbwColor(0, 0, 32);

                for (uint8_t col = 0; col < COLUMNS; col++) {
                    strip.SetPixelColor(to_index(col, row), color);
                }
            }

            pulses[param.index].pulse_center++;
        } else if (param.state == AnimationState_Completed) {
            for (uint8_t col = 0; col < COLUMNS; col++) {
                //strip.SetPixelColor(to_index(col, ROWS - 1), RgbwColor(0, 0, 0));
            }
        }
    }

    struct pulse_state {
        uint8_t pulse_center;
    };

    NeoPixelAnimator anims;
    pulse_state pulses[PULSE_COUNT];
};
