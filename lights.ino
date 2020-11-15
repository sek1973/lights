#include "common.h"
#include "webserver.h"

// !!!!!!!!!!!!!!!!!!!!!!! To add new effect go to effects.h file !!!!!!!!!!!!!!!!!!!!!!!

void setup() {
  strip.Begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  showStrip();            // Turn OFF all pixels ASAP
  strip.SetBrightness(BRIGHTNESS);
  runTasks();  
}

void loop() {  
}

void runTasks() {
  xTaskCreate(
    runLED,          /* Task function. */
    "runLED",        /* String with name of task. */
    10000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    2,                /* Priority of the task. */
    NULL);            /* Task handle. */

  xTaskCreate(
    runWebServer,          /* Task function. */
    "taskWebServer",        /* String with name of task. */
    10000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */
}

void runLED(void *parameter) {
  while(true) {
    switch_effect();
  }
  Serial.println("Ending task LED");
  vTaskDelete( NULL );
}
