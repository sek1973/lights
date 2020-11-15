//fire settings - begin
#define FIRERATE 400 //max 100000
#define MINPOWER 200
#define MAXPOWER 245
#define FIRETICK 25
#define MINFIRELENGTH 30
#define MAXFIRELENGTH 40
#define NULLMULTIPLY 10000
int power[LED_COUNT];
double rate = 10.0;
int getrbypower(int power){
  if(power>255){
    return 0;
  }
  else if(power>=225){
    return (255-power)*8.5;
  }
  else if(power>=100){
    return 255;
  }
  else if(power>=0){
    return 255*power/100;
  }
  else{
    return 0;
  }
}
int getgbypower(int power){
  if(power>255){
    return 0;
  }
  else if(power>=225){
    return (255-power)*8.5;
  }
  else if(power>=100){
    return (power)*1.13;
  }
  else if(power>=50){
    return (power-50)*2.25;
  }
  else{
    return 0;
  }
}
int getbbypower(int power){
  if(power>=225){
    return 255;
  }
  else if(power>=100){
    return (power-100)*2;
  }
  else{
    return 0;
  }
}
//fire settings - end

void fire(){
  randomSeed(analogRead(0));
  int possibility = random(0,100000);
  if(possibility<FIRERATE*rate/10){
    rate=10;
    int fireplace = random(0,LED_COUNT);
    int firepower = random(MINPOWER, MAXPOWER+1);
    int firelength = random(MINFIRELENGTH,MAXFIRELENGTH+1);
    power[fireplace] = firepower;
    for(int i=1; i<=firelength; i++){
      if(fireplace+i<LED_COUNT){
        if(power[fireplace+i]<(firepower - (i*0.7*firepower/firelength))){
          power[fireplace+i] = firepower - (i*0.7*firepower/firelength);
        }
      }
      if(fireplace-i>=0){
        if(power[fireplace-i]<firepower - (i*0.7*firepower/firelength)){
          power[fireplace-i] = firepower - (i*0.7*firepower/firelength);
        }
      }
    }
  }
  for(int x=0; x<LED_COUNT; x++){
    strip.SetPixelColor(x,
      RgbColor(getrbypower(power[x]),
      getgbypower(power[x]),
      getbbypower(power[x])));
    if (power[x]>0){
      power[x]-=1;
    }
  }
  int stripon = LED_COUNT;
  for(int pixel = 0; pixel<LED_COUNT; pixel++){
    if(strip.GetPixelColor(pixel) == RgbColor(0,0,0)){
      stripon += -1;
    }
  }
  /*if(stripon==0){
    rate = rate * NULLMULTIPLY/1000;
    for(int i=0; i<rate/10; i++){
      strip.strip.strip.SetPixelColor(i,RgbColor(50,50,50));
    }
  }*/
  showStrip();
  delay(FIRETICK);
}
