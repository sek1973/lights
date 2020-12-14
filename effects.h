
// ------------------------------- effects include section -----------------------------------
#include "strobe.h"
#include "fade_in_out.h"
#include "rgb_loop.h"
#include "rainbow.h"
#include "running_led.h"
#include "rainbow_static.h"
#include "fire.h"
#include "leds_off.h"
// -------------------------------------------------------------------------------------------

typedef void (*EffectFunction) ();

struct EffectsData {  
  String url;
  String title;
  EffectFunction fn;
};

// *************************** ADD NEW EFFECT DEF HERE ***************************************
// to add new effect:
// * define its subrutine in a dedicated file
// * add #include clause to include section of this file
// * modify effects array by inserting new item referring your new effect
// * update effectsCount constant

EffectsData effects[] = {
  {url: "rainbow", title: "Rainbow", fn: []() { rainbow(100); }},
  {url: "rainbowstatic", title: "Rainbow static", fn: []() { rainbow_static(100); }},
  {url: "fire", title: "Fire", fn: []() { fire(); }},
  {url: "rgbloop", title: "RGB loop", fn: []() { RGBLoop(); }},
  {url: "runningled", title: "Running LED", fn: []() { runningLed(); }},
  {url: "lightning", title: "Lightning", fn: []() { strobeRandom(200, 200, 200); }},
  {url: "ledsoff", title: "OFF", fn: []() { leds_off(); }},
};

const int effectsCount = 7;
// *******************************************************************************************

void switch_effect() {
  syncTime();
  if (checkTimeSpan()) {
    effects[LED_Effect].fn();  
  } else {
    leds_off();
  }
}

void create_buttons() {
  for (int i = 0; i < effectsCount; i++) {
    client.println("<p><a href=\"/" + effects[i].url + "/\"><button class=\"button\">" + effects[i].title + "</button></a></p>");  
  } 
}

void effect_from_header(String header) {  
  for (int i = 0; i < effectsCount; i++) {
    if (header == "/" + effects[i].url + "/") {
      Serial.println("effect switched to " + effects[i].title + "...");      
      LED_Effect = i;
      break;
    }
  }  
}
