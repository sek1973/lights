void effect0(){
  int r = random(0, 256);
  int g = random(0, 256);
  int b = random(0, 256);

  for(int i = 0; i < LED_COUNT; i++){
    setPixel(i, r, g, b);
    showStrip();
    delay(101);
  }
}

void effect1(){
  int r = random(0, 256);
  int g = random(0, 256);
  int b = random(0, 256);

    for(int i = 1; i <= LED_COUNT; i++){
      setPixel(LED_COUNT-i, r, g, b);
      showStrip();
      delay(101);
    }
}

void runningDot(){
  effect0();
  effect1();
}
