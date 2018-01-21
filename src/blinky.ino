#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "anim.hpp"
#include "anim_fire.hpp"
#include "anim_lighthouse.hpp"
#include "anim_plasma.hpp"
#include "anim_rain.hpp"
#include "anim_starry_night.hpp"
#include "anim_warp_core.hpp"

void SetRandomSeed() {
    uint32_t seed;
    seed = analogRead(0);
    delay(1);

    for (int shifts = 3; shifts < 31; shifts += 3) {
        seed ^= analogRead(0) << shifts;
        delay(1);
    }

    randomSeed(seed);
}

output out;
animation* anim = nullptr;
Encoder knob(D1, D2);
int32_t knob_last = 0;

void setup() {
    out.show();
    SetRandomSeed();
    knob.write(knob_last);
}

void loop() {
    if (!anim) {
        anim = new anim_fire(&out);
    }

    const int32_t knob_now = knob.read();
    if (std::abs(knob_now - knob_last) >= 4) {
        out.adjust_brightness(knob_now < knob_last);
        knob_last = knob_now;
    }

    anim->loop();
    out.show();
}
