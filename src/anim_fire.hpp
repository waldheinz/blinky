#pragma once

#include "anim.hpp"

class anim_fire final : public animation {
public:
    static constexpr float MAX_HEAT = 255;

    anim_fire() {

    }

    void loop() override {
        heat_target = (heat_target + 1) % 2;
        add_sparks();
        burn();
        show();
        delay(15);
    }

private:
    void burn() {
        const uint8_t src = (heat_target + 1) % 2;

        for (int r = 1; r < ROWS; r++) {
            for (int c = 0; c < COLUMNS; c++) {
                //const auto left =
                heat[heat_target][r][c] =
                    0.70 * heat[src][r][c] +
                    0.20 * heat[src][r-1][c] +  // below
                    0.05 * heat[src][r][c-1] +  // left
                    0.05 * heat[src][r][c+1];   // right
            }
        }

    }

    void add_sparks() {
        for (int c=0; c < COLUMNS; c++) {
            if (random(100) > 80) {
                heat[heat_target][0][c] = random(MAX_HEAT);
            } else {
                heat[heat_target][0][c] = 0;
            }
        }
    }

    void show() const {
        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                strip.SetPixelColor(to_index(c, r),
                    colorGamma.Correct(RgbwColor(0, 0, 0, heat[heat_target][r][c])));
            }
        }
    }

    uint8_t heat_target = 0;
    float heat[2][ROWS][COLUMNS];
};
