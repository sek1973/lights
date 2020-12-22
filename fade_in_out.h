void fadeInOut() {
  randomSeed(analogRead(0));
  int r = random(0, 255);
  int g = random(0, 255);
  int b = random(0, 255);

  for (int k = 0; k < 256; k = k + 1) {
    setAllWBright(r, g, b, k);
    showStrip();
    delay(10);
  }

  for (int k = 255; k >= 0; k = k - 2) {
    setAllWBright(r, g, b, k);
    showStrip();
    delay(10);
  }
}
