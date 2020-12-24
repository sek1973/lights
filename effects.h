
// ------------------------------- effects include section -----------------------------------
#include "strobe.h"
#include "fade_in_out.h"
#include "rgb_loop.h"
#include "rainbow.h"
#include "running_led.h"
#include "rainbow_static.h"
#include "fire.h"
#include "leds_off.h"
#include "change_color.h"
#include "randomPixelColorChange.h"
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

const int effectsCount = 10;

EffectsData effects[effectsCount] = {
  {title: "Rainbow", fn: []() { rainbow(100); }},
  {title: "Rainbow static", fn: []() { rainbow_static(100); }},
  {title: "Fire", fn: []() { fire(); }},
  {title: "Rainbow", fn: []() { rainbow(100); }},
  {title: "colorful", fn: []() { randomPixelColorChange(); }},
  {title: "Running LED", fn: []() { runningLed(); }},
  {title: "Fade in-out", fn: []() { fadeInOut(); }},
  {title: "Change color", fn: []() { changeColor(); }},
  {title: "Lightning", fn: []() { strobeRandom(200, 200, 200); }},
  {title: "OFF", fn: []() { leds_off(); }},
};

// *******************************************************************************************

void switch_effect() {
  syncTime();
  if (checkTimeSpan() && settings.effect >= 0 && settings.effect < effectsCount) {        
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
