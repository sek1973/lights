#include "time.h"
#include <NeoPixelBrightnessBus.h> // instead of NeoPixelBus.h
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN     4

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  8

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

// signum function
#define sgn(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))

// store time received from NTP server
struct tm timeinfo;
// to register current tick and infer time span
clock_t registeredTick;

struct TimeSpan {
  byte startHour;
  byte stopHour;
};

const int settingsCount = 6;

const String settingsParams[settingsCount] {
  "brightness",
  "effect",
  "start_hour1",
  "stop_hour1",
  "start_hour2",
  "stop_hour2"
};

struct Settings {
  byte brightness; // NeoPixel brightness, 0 (min) to 255 (max)
  byte effect;
  TimeSpan spans[2];
};

Settings settings = {
  brightness: 50,
  effect: 0,
  spans: {
    { startHour: 0, stopHour: 0 },
    { startHour: 0, stopHour: 0 }
  }
};

void showStrip() {
  strip.Show();
}

void setRelativeBrightness(byte bright) {
  byte relative = settings.brightness * bright / 255;
  strip.SetBrightness(relative);
}

void setPixel(int pixel, byte red, byte green, byte blue) {
  strip.SetBrightness(settings.brightness);
  strip.SetPixelColor(pixel, RgbColor(red, green, blue));
}

void setPixelWBright(int pixel, byte red, byte green, byte blue, byte bright) {
  setRelativeBrightness(bright);
  strip.SetPixelColor(pixel, RgbColor(red, green, blue));
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < LED_COUNT; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

void setAllWBright(byte red, byte green, byte blue, byte bright) {  
  for (int i = 0; i < LED_COUNT; i++ ) {
    setPixelWBright(i, red, green, blue, bright);
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
  if (settings.spans[0].startHour == 0 && settings.spans[0].stopHour == 0
      && settings.spans[1].startHour == 0 && settings.spans[1].stopHour == 0) {
    return true;
  } else if (timeinfo.tm_hour >= settings.spans[0].startHour && timeinfo.tm_hour < settings.spans[0].stopHour) {
    return true;
  } else if (timeinfo.tm_hour >= settings.spans[1].startHour && timeinfo.tm_hour < settings.spans[1].stopHour) {
    return true;
  } else {
    return false;
  }
}
