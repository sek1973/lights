void runningLed() {
  randomSeed(analogRead(0));
  int r = random(0,255);
  int g = random(0,255);
  int b = random(0,255);  
  for(int i = 0; i < LED_COUNT; i++ ) {    
    setPixel(i, r, g, b);    
    showStrip();
    delay(50);    
  }
}
