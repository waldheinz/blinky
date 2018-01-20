#pragma once

#include "anim.hpp"

class anim_rain final : public animation {
public:
    anim_rain(output * out) : animation(out), anims(COLUMNS) {
        for (int i = 0; i < COLUMNS; i++) {
            start(i);
        }
    }

    void loop() override {
        anims.UpdateAnimations();
    }

private:
    void start(uint16_t i) {
        anims.StartAnimation(i, 500 + random(2500),
                std::bind(&anim_rain::animate, this, std::placeholders::_1));
    }

    void animate(const AnimationParam& param) {
        if (param.state == AnimationState_Completed) {
            start(param.index);
        } else if (param.state == AnimationState_Progress) {
            float drop_y = ROWS * (1 - param.progress);

            for (int row = 0; row < ROWS; row++) {
                const float d = std::min(std::max(fabsf(row - drop_y) / 2.0f, 0.0f), 1.0f);
                const float de = NeoEase::CubicInOut(d);
                const RgbwColor c = RgbwColor::LinearBlend(RgbwColor(0, 15, 64), RgbwColor(0), de);

                set_pixel(param.index, row, c);
            }
        }
    }

    NeoPixelAnimator anims;
};
