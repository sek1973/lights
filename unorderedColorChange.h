int DELAY = 10;
int PIXELCHANGEROUNDS = 20;
int COLORCHANGEDELAY = 1000;
struct Lamp{ byte r; byte g; byte b; } currentState[LED_COUNT], targetColor;
bool changed[LED_COUNT];
void showAll(){
  for(int i = 0; i < LED_COUNT; i++){
    setPixel(i, currentState[i].r, currentState[i].g, currentState[i].b);
  }
  showStrip();
}
void setChanged(bool b){
  for(int i = 0; i < LED_COUNT; i++){
    changed[i] = b;
  }
}
void changeTo(int pixel, byte r, byte g, byte b){
  currentState[pixel].r = r;
  currentState[pixel].g = g;
  currentState[pixel].b = b;
}
void smoothlyChangeTo(int pixel, byte r, byte g, byte b){
  int roundsLeft = PIXELCHANGEROUNDS;
  while(roundsLeft>0){
    int deltaR = r - currentState[pixel].r;
    int deltaG = g - currentState[pixel].g;
    int deltaB = b - currentState[pixel].b;

    currentState[pixel].r += deltaR/roundsLeft;
    currentState[pixel].g += deltaG/roundsLeft;
    currentState[pixel].b += deltaB/roundsLeft;
    showAll();
    delay(DELAY);
    roundsLeft--;
  }
}
bool allChanged(){
  for(int i = 0; i < LED_COUNT; i++){
    if(!changed[i]) return false;
  }
  return true;
}
void randomOrderColorChange(){
  if(allChanged()){
    setChanged(false);
    targetColor.r = random(0, 256);
    targetColor.g = random(0, 256);
    targetColor.b = random(0, 256);
    delay(COLORCHANGEDELAY);
  }
  else{
    int targetLed = random(0, LED_COUNT);
    
    if(!changed[targetLed]){
      smoothlyChangeTo(targetLed, targetColor.r, targetColor.g, targetColor.b);
      changed[targetLed] = true;
    }
  }
}
