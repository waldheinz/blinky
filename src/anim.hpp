#pragma once

#include <Arduino.h>
#include <Encoder.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include <cstdint>

const uint8_t COLUMNS = 25;
const uint8_t ROWS = 12;
const uint16_t PIXEL_COUNT = COLUMNS * ROWS;

class output {
public:
    output() : strip(PIXEL_COUNT), darken(128) {
        strip.Begin();
    }

    void set_pixel(uint16_t idx, RgbwColor const & c) {
        buffer[idx] = c;
    }

    void show() {
        for (int i = 0; i < PIXEL_COUNT; i++) {
            auto c = buffer[i];
            c.Darken(darken);
            strip.SetPixelColor(i, colorGamma.Correct(c));
        }

        strip.Show();
    }

    void set_darken(bool brighter) {
        if (brighter && darken > 0) {
            darken -= 1;
        } else if (!brighter && darken < 255) {
            darken += 1;
        }
    }

private:
    NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> strip;
    NeoGamma<NeoGammaTableMethod> colorGamma;
    RgbwColor buffer[PIXEL_COUNT];
    uint8_t darken;
};

class animation {
public:
    animation(output * out) : out(out) { }

    virtual ~animation() { }

    virtual void loop() { }

    static uint16_t to_index(uint8_t col, uint8_t row) {
        const int16_t c = col;
        const int16_t r = row;

        return c * (int16_t)ROWS + ((c % 2 == 0) ? r : ((int16_t)ROWS - r - 1));
    }

protected:
    void set_pixel(uint8_t col, uint8_t row, RgbwColor const & c) const {
        out->set_pixel(to_index(col, row), c);
    }

    void set_pixel(uint16_t idx, RgbwColor const & c) const {
        out->set_pixel(idx, c);
    }

private:
    output * const out;
};
