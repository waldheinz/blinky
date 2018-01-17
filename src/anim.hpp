#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include <cstdint>

const uint8_t COLUMNS = 25;
const uint8_t ROWS = 12;
const uint16_t PIXEL_COUNT = COLUMNS * ROWS;

extern NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> strip;
extern NeoGamma<NeoGammaTableMethod> colorGamma;

class animation {
public:

    virtual ~animation() { }

    virtual void loop() { }

    static uint16_t to_index(uint8_t col, uint8_t row) {
        const int16_t c = col;
        const int16_t r = row;

        return c * (int16_t)ROWS + ((c % 2 == 0) ? r : ((int16_t)ROWS - r - 1));
    }
};
