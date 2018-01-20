#pragma once

#include "anim.hpp"

class anim_fire final : public animation {
public:
    static constexpr float MAX_HEAT = 255.0f;
    static constexpr float SCALE_NEW = 0.28f;
    static constexpr float SCALE_OLD = 0.70f;

    anim_fire(output * out) : animation(out) { }

    void loop() override {
        std::swap(heat_src, heat_target);
        add_sparks();
        burn();
        show();
    }

private:
    void burn() {
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLUMNS; c++) {
                float sum = 0;

                if (r > 0) {
                    sum += 0.50 * heat[heat_src][r-1][c];  // below
                }

                sum +=
                    0.25 * heat[heat_src][r][c == 0 ? (COLUMNS - 1) : c - 1] +  // left
                    0.25 * heat[heat_src][r][c == COLUMNS - 1 ? 0 : c + 1];   // right

                heat[heat_target][r][c] = SCALE_NEW * sum + SCALE_OLD * heat[heat_src][r][c];
            }
        }
    }

    void add_sparks() {
        for (int c=0; c < COLUMNS; c++) {
            if (random(100) > 94) {
                heat[heat_src][0][c] = MAX_HEAT;
            }
        }
    }

    RgbwColor map_color(float heat) const {
        const float d = std::max(std::min(heat / MAX_HEAT, 1.0f), 0.0f);
        const uint8_t maxBright = 128;

        static const RgbwColor black = RgbwColor(0);
        static const RgbwColor red = RgbwColor(maxBright, 0, 0);
        static const RgbwColor yellow = RgbwColor(maxBright, maxBright, 0);
        static const RgbwColor white = RgbwColor(maxBright, maxBright, maxBright);

        RgbwColor result;

        if (d < 0.25f) {
            return RgbwColor::LinearBlend(black, red, d * 4);
        } else if (d < 0.5f) {
            return RgbwColor::LinearBlend(red, yellow, (d - 0.25f) * 4);
        } else {
            return RgbwColor::LinearBlend(yellow, white, (d - 0.5f) * 2);
        }
    }

    void show() const {
        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                set_pixel(c, r, map_color(heat[heat_target][r][c]));
            }
        }
    }

    uint8_t heat_target = 0;
    uint8_t heat_src = 1;
    float heat[2][ROWS][COLUMNS];
};
