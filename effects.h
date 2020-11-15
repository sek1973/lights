// ------------------------------- effects include section -----------------------------------
#include "strobe.h"
#include "fade_in_out.h"
#include "rgb_loop.h"
#include "rainbow.h"
#include "running_led.h"
#include "rainbow_static.h"
#include "fire.h"
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
};

const int effectsCount = 6;
// *******************************************************************************************

void switch_effect() {
  effects[LED_Effect].fn();  
}

void create_buttons() {
  for (int i = 0; i < effectsCount; i++) {
    client.println("<p><a href=\"/" + effects[i].url + "/\"><button class=\"button\">" + effects[i].title + "</button></a></p>");  
  } 
}

void effect_from_header() {  
  for (int i = 0; i < effectsCount; i++) {
    if (header.indexOf("GET /" + effects[i].url + "/") >= 0) {
      Serial.println("effect switched to " + effects[i].title + "...");
      LED_Effect = i;
      break;
    }
  }  
}
