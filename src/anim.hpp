#pragma once

#include <Arduino.h>
#include <Encoder.h>
#include <NeoPixelBrightnessBus.h>
#include <NeoPixelAnimator.h>

#include <cstdint>

const uint8_t COLUMNS = 25;
const uint8_t ROWS = 12;
const uint16_t PIXEL_COUNT = COLUMNS * ROWS;

class output {
public:
    static const uint16_t BRIGHT_MIN = 1;
    static const uint16_t BRIGHT_MAX = 200;

    output() : strip(PIXEL_COUNT), brightness(BRIGHT_MAX) {
        strip.Begin();
        strip.SetBrightness(brightness);
    }

    void set_pixel(uint16_t idx, RgbwColor const & c) {
        strip.SetPixelColor(idx, colorGamma.Correct(c));
    }

    void show() {
        strip.Show();
    }

    void adjust_brightness(bool darker) {
        if (darker) {
            brightness = std::min(brightness * 0.9f, brightness - 1.0f);
        } else {
            brightness = std::max(brightness * 1.1f, brightness + 1.0f);
        }

        brightness = std::max(BRIGHT_MIN, std::min(BRIGHT_MAX, brightness));
        strip.SetBrightness(brightness);
    }

private:
    NeoPixelBrightnessBus<NeoGrbwFeature, Neo800KbpsMethod> strip;
    NeoGamma<NeoGammaTableMethod> colorGamma;
    uint16_t brightness;
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

class work_light final : public animation {
public:
    work_light(output * out) : animation(out) {
        for (int i = 0; i < PIXEL_COUNT; i++) {
            set_pixel(i, RgbwColor(255, 255, 255));
        }
    }
};
