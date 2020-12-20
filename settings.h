#include <EEPROM.h>

int getSettingAddress(String name) {
  int result = -1;
  for (int i = 0; i < settingsCount; i++) {
   if (settingsParams[i] == name) {
     return i;     
   }
  }
}

void writeToMemory(String paramName, byte value) {
  int addr = getSettingAddress(paramName);
  if (addr < settingsCount) {    
    byte oldVal = EEPROM.read(addr);      
    if (value != oldVal) {
      Serial.println("Saving new value...");
      Serial.print(value);
      Serial.print(" ---> ");
      Serial.println(addr);
      EEPROM.write(addr, value);
      EEPROM.commit();      
    }
  } else {
    Serial.print("Got invalid address for ");
    Serial.println(paramName);
  }
}

byte readFromMemory(String paramName) {
  return EEPROM.read(getSettingAddress(paramName));
}

void initFlashMemory() {
  EEPROM.begin(settingsCount);;
}

void readParamsFromFlash() {
  settings.brightness = readFromMemory("brightness");
  settings.effect = readFromMemory("effect");
  settings.spans[0].startHour = readFromMemory("start_hour1");
  settings.spans[0].stopHour = readFromMemory("stop_hour1");
  settings.spans[1].startHour = readFromMemory("start_hour2");
  settings.spans[1].stopHour = readFromMemory("stop_hour2");
  strip.SetBrightness(settings.brightness);
}
