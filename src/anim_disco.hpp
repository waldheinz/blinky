#pragma once

#include "anim.hpp"

class anim_disco final : public animation {
public:
    anim_disco(output * out) : animation(out), anims(ANIMS), state(ANIMS) {
        for (int i=0; i < ANIMS; i++) {
            start(i);
        }
    }

    void loop() override {
        anims.UpdateAnimations();
    }

private:
    typedef std::pair<HslColor, HslColor> state_t;

    int const SROWS = 4;
    int const SCOLS = 5;
    int const ANIMS = SROWS * SCOLS;

    void start(uint16_t i) {
        anims.StartAnimation(i, 500 + random(500),
                std::bind(&anim_disco::animate, this, std::placeholders::_1));
    }

    void animate(const AnimationParam& param) {
        if (param.state == AnimationState_Started) {
            state[param.index].first = state[param.index].second;
            state[param.index].second = HslColor(random(100) / 100.0f, 1.0f, 0.5f);
        } else if (param.state == AnimationState_Completed) {
            start(param.index);
        } else if (param.state == AnimationState_Progress) {
            int const r0 = (param.index / SCOLS) * 3;
            int const r1 = r0 + 3;
            int const c0 = (param.index % SCOLS) * 5;
            int const c1 = c0 + 5;

            for (int r = r0; r < r1; r++) {
                for (int c = c0; c < c1; c++) {
                    HslColor const result = HslColor::LinearBlend<NeoHueBlendShortestDistance>(
                        state[param.index].first, state[param.index].second, param.progress);
                    set_pixel(c, r, result);
                }
            }
        }
    }

    NeoPixelAnimator anims;
    std::vector<state_t> state;
};
