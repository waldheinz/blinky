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

void setup() {
    out.show();
    SetRandomSeed();
}

void loop() {
    if (!anim) {
        anim = new anim_plasma(&out);
    }

    anim->loop();
    out.show();
}
