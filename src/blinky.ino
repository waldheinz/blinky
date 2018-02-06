#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <WiFiManager.h>

#include <memory>

#include "anim.hpp"
#include "anim_disco.hpp"
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

WiFiManager wifiManager;
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

output out;
std::unique_ptr<animation> anim = nullptr;
Encoder knob(D1, D2);
int32_t knob_last = 0;
uint32_t anim_idx = 0;
bool btn_handled = false;
unsigned long last_btn_input = 0;
long const debounce_delay = 50; /* ms */

void set_animation() {
    out.clear();

    switch (anim_idx) {
    case 0:
        anim = std::unique_ptr<animation>(new anim_disco(&out));
        break;

    case 1:
        anim = std::unique_ptr<animation>(new anim_fire(&out));
        break;

    case 2:
        anim = std::unique_ptr<animation>(new anim_lighthouse(&out));
        break;

    case 3:
        anim = std::unique_ptr<animation>(new anim_plasma(&out));
        break;

    case 4:
        anim = std::unique_ptr<animation>(new anim_rain(&out));
        break;

    case 5:
        anim = std::unique_ptr<animation>(new anim_starry_night(&out));
        break;

    default:
        anim = std::unique_ptr<animation>(new anim_warp_core(&out));
        break;
    }
}

void setup() {
    out.show();
    SetRandomSeed();
    knob.write(knob_last);

    pinMode(D3, INPUT | INPUT_PULLUP);
    pinMode(D0, OUTPUT);
    digitalWrite(D0, 1);

    MDNS.begin("blinky");
    wifiManager.autoConnect();
    httpUpdater.setup(&httpServer);
    httpServer.begin();
    MDNS.addService("http", "tcp", 80);
    ArduinoOTA.begin();

    set_animation();
}

void loop() {
    httpServer.handleClient();
    ArduinoOTA.handle();

    /* adjust brightness */
    const int32_t knob_now = knob.read();
    if (std::abs(knob_now - knob_last) >= 4) {
        out.adjust_brightness(knob_now > knob_last);
        knob_last = knob_now;
    }

    /* toggle animations */
    auto const btn_state = digitalRead(D3);
    long const now = millis();

    if ((now - last_btn_input) > debounce_delay) {
        last_btn_input = now;

        if (!btn_state && !btn_handled) {
            auto const ANIM_COUNT = 6;
            anim_idx = (anim_idx + 1) % ANIM_COUNT;
            digitalWrite(D0, anim_idx % 2);
            set_animation();
            btn_handled = true;
        } else if (btn_state) {
            btn_handled = false;
        }
    }

    anim->loop();
    out.show();
}
