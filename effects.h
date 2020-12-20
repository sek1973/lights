
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
  {title: "Rainbow", fn: []() { rainbow(100); }},
  {title: "Rainbow static", fn: []() { rainbow_static(100); }},
  {title: "Fire", fn: []() { fire(); }},
  {title: "RGB loop", fn: []() { RGBLoop(); }},
  {title: "Running LED", fn: []() { runningLed(); }},
  {title: "Lightning", fn: []() { strobeRandom(200, 200, 200); }},
  {title: "OFF", fn: []() { leds_off(); }},
};

const int effectsCount = 7;
// *******************************************************************************************

void switch_effect() {
  syncTime();
  if (checkTimeSpan()) {        
    effects[settings.effect].fn();  
  } else {
    leds_off();
  }
}

void createButtons() {
  for (int i = 0; i < effectsCount; i++) {    
    client.println("<form action=\"\" method=\"post\"><button name=\"effect\" class=\"button\" value=\"" + String(i) + "\">" + effects[i].title + "</button></form>");  
  } 
}
