#pragma once

#include "anim.hpp"

uint16_t SINE_TABLE[] = {
    0, 1144, 2287, 3430, 4571, 5712, 6850, 7987, 9121, 10252, 11380,
    12505, 13625, 14742, 15854, 16962, 18064, 19161, 20251, 21336, 22414,
    23486, 24550, 25607, 26655, 27696, 28729, 29752, 30767, 31772, 32768,

    33753, 34728, 35693, 36647, 37589, 38521, 39440, 40347, 41243, 42125,
    42995, 43851, 44695, 45524, 46340, 47142, 47929, 48702, 49460, 50203,
    50930, 51642, 52339, 53019, 53683, 54331, 54962, 55577, 56174, 56755,

    57318, 57864, 58392, 58902, 59395, 59869, 60325, 60763, 61182, 61583,
    61965, 62327, 62671, 62996, 63302, 63588, 63855, 64103, 64331, 64539,
    64728, 64897, 65047, 65176, 65286, 65375, 65445, 65495, 65525, 65535,
};

class anim_plasma final : public animation {
public:
    anim_plasma() : anims(2) {
        anims.StartAnimation(0, 10000,
            std::bind(&anim_plasma::animate, this, std::placeholders::_1));

        anims.StartAnimation(1, 3333,
            std::bind(&anim_plasma::animate_p1, this, std::placeholders::_1));
    }

    void loop() override {
        anims.UpdateAnimations();
    }

private:

    float fast_sin(float f) {
        long x = f + 0.5f;
        boolean pos = true;  // positive - keeps an eye on the sign.

        if (x < 0) {
            x = -x;
            pos = !pos;
        }

        if (x >= 360) {
            x %= 360;
        }

        if (x > 180) {
            x -= 180;
            pos = !pos;
        }

        if (x > 90) {
            x = 180 - x;
        }

        return SINE_TABLE[x] * (pos ? 0.0000152590219f : -0.0000152590219f);
    }

    RgbwColor map_grey(float v) {
        return RgbwColor(255.0f * (0.5f + 0.5f * v * 0.8f));
    }

    RgbwColor map_colour_1(float v) {
        const uint8_t r = 255.0f * (0.5f + 0.5f * fast_sin(v));
        const uint8_t g = 255.0f * (0.5f + 0.5f * fast_sin(v + 90));

        return RgbwColor(r, g, 0, 0);
    }

    void animate(const AnimationParam& param) {
        if (param.state == AnimationState_Completed) {
            anims.RestartAnimation(param.index);
        } else if (param.state == AnimationState_Progress) {
            const float time = param.progress * 3600.0f;

            for (int col = 0; col < COLUMNS; col++) {
                for (int row = 0; row < ROWS; row++) {
                    float v = 0.0f;
                    const float x = (float)col / M_PI * 180.0f / COLUMNS;
                    const float y = (float)row / M_PI * 180.0f / ROWS;

                    v += fast_sin(x * (5.0f + 5.0f * p1) + time);
                    v += fast_sin((y * 10.0f + time) * 0.5f);
                    v += fast_sin((x * 10.0f + y * 10.0f + time) / 2.0f);

                    const float cx = x + 0.5f * fast_sin(time / 15.0f);
                    const float cy = y + 0.5f * fast_sin(time / 13.0f + 90.0f);
                    v += fast_sin(sqrtf(100.0f * (cx * cx + cy * cy) + 1) + time);

                    const auto c = map_colour_1(v * 180.0f);

                    strip.SetPixelColor(to_index(col, row), colorGamma.Correct(c));
                }
            }
        }
    }

    void animate_p1(const AnimationParam& param) {
        if (param.state == AnimationState_Progress) {
            p1 = fast_sin(param.progress * 360.0f);
        } else if (param.state == AnimationState_Completed) {
            anims.RestartAnimation(param.index);
        }
    }

    NeoPixelAnimator anims;
    float p1 = 0;
};
