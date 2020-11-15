// Declare structure for flash times
// Each flash consists of lighter and darker periods
struct Duration {
  int light;
  int dark;  
};

void strobe(byte red, byte green, byte blue, int strobeCount, int flashDelay, int endPause){
  for(int j = 0; j < strobeCount; j++) {
    setAll(red, green, blue);
    showStrip();
    delay(flashDelay);
    setAll(0,0,0);
    showStrip();
    delay(flashDelay);
  } 

  delay(endPause);
}

void strobeRandom(byte red, byte green, byte blue){
  // prepare randomize logic
  randomSeed(analogRead(0));

  int strobeCount = random(1,10);
  Duration times[strobeCount];  
  
  // populate times array to have different times for each light and bright period
  for (int i = 0; i < strobeCount; i++) {
    times[i].light = random(1,100);
    times[i].dark = random(1,200);    
  }
  for(int j = 0; j < strobeCount; j++) {
    setAllWBright(red, green, blue, random(25, 200));    
    delay(times[j].light);
    setAllWBright(red, green, blue, random(5, 25));    
    delay(times[j].dark);
  }
  setAll(0,0,0);
  delay(random(100, 2000));
}
