struct lamp{ int r; int g; int b; } current[LED_COUNT];
void setAll(){
  for(int i = 0; i < LED_COUNT; i++){
    current[i].r %= 256;
    current[i].g %= 256;
    current[i].b %= 256;
    setPixel(i, current[i].r, current[i].g, current[i].b);
  }
  showStrip();
}
void set(int index, int r, int g, int b){
  current[index].r = r;
  current[index].g = g;
  current[index].b = b;
}
lamp target[LED_COUNT];
int tiksLeft[LED_COUNT];
int TIKLENGTH = 10;

void randomPixelColorChange(){
  for(int i = 0; i < LED_COUNT; i++){
    
    if(tiksLeft[i]==0){
      target[i].r = random(0,256);
      target[i].g = random(0,256);
      target[i].b = random(0,256);

      tiksLeft[i] = random(20, 200);
    }
        
  }
  for(int i = 0; i < LED_COUNT; i++){
    current[i].r += (target[i].r - current[i].r)/tiksLeft[i];
    current[i].g += (target[i].g - current[i].g)/tiksLeft[i];
    current[i].b += (target[i].b - current[i].b)/tiksLeft[i];
    tiksLeft[i]--;
  }
  setAll();
  delay(TIKLENGTH);
}