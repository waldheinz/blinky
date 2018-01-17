// NeoPixelFunLoop
// This example will move a trail of light around a series of pixels.
// A ring formation of pixels looks best.
// The trail will have a slowly fading tail.
//
// This will demonstrate the use of the NeoPixelAnimator.
// It shows the advanced use an animation to control the modification and
// starting of other animations.
// It also shows the normal use of animating colors.
// It also demonstrates the ability to share an animation channel rather than
// hard code them to pixels.
//

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "anim.hpp"
#include "anim_lighthouse.hpp"
#include "anim_starry_night.hpp"
#include "anim_warp_core.hpp"

NeoGamma<NeoGammaTableMethod> colorGamma;
NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> strip(PIXEL_COUNT);

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

animation* anim = nullptr;

void setup() {
    strip.Begin();
    delay(2);
    strip.Show();

    SetRandomSeed();
}

void loop() {
    if (!anim) {
        anim = new anim_lighthouse();
    }

    anim->loop();
    strip.Show();
}
