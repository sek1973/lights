int current_r = 0;
int current_g = 0;
int current_b = 0;

void changeColor() {
  randomSeed(analogRead(0));
  int delta_r = random(0, 255) - current_r;
  int delta_g = random(0, 255) - current_g;
  int delta_b = random(0, 255) - current_b;
  int delta = abs(delta_r) + abs(delta_g) + abs(delta_b);

  float dr = 0;
  float dg = 0;
  float db = 0;

  for (int i = 0; i < delta; i++) {    
    dr += (float(delta_r) / float(delta));    
    dg += (float(delta_g) / float(delta));    
    db += (float(delta_b) / float(delta));    
    setAll(current_r + dr, current_g + dg, current_b + db);
    showStrip();
    delay(10);
  }

  current_r += dr;
  current_g += dg;
  current_b += db; 
}
