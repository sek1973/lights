#include "time.h"
#include <NeoPixelBrightnessBus.h> // instead of NeoPixelBus.h
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN     4

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  98

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 10

// Declare our NeoPixel strip object:
// Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
//NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod> strip(LED_COUNT, LED_PIN);
NeoPixelBrightnessBus<NeoRgbFeature, Neo800KbpsMethod> strip(LED_COUNT, LED_PIN);

// Current effect
int LED_Effect = 0;

// store time received from NTP server
struct tm timeinfo;
// to register current tick and infer time span
clock_t registeredTick;

void showStrip() {
  strip.Show();
}

void setPixel(int pixel, byte red, byte green, byte blue) {  
  strip.SetPixelColor(pixel, RgbColor(red, green, blue));
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < LED_COUNT; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}

void setAllWBright(byte red, byte green, byte blue, byte bright) {
  strip.SetBrightness(bright);
  for(int i = 0; i < LED_COUNT; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}

void getTimeInfo() {
  if (getLocalTime(&timeinfo)) {    
    Serial.print("Time received: ");
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  } else {
    Serial.println("Failed to obtain time");
  }
}

void syncTime() {
  clock_t currentTick;
  currentTick = clock();
  if ((currentTick - registeredTick) / CLOCKS_PER_SEC >= 60) {
    registeredTick = currentTick;
    getTimeInfo();
  }
}

bool checkTimeSpan() {
  syncTime();
  if (timeinfo.tm_hour > 15 && timeinfo.tm_hour < 22) {
    return true;
  } else {
    return false;
  }
}
