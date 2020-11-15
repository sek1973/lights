// colors must be greater then LED_COUNT
void rainbow_static(int colors) {  
  float freq = 10.0 / colors;  
  byte red, green, blue;
  for (int i = 0; i < colors; i++)
  {    
    red   = sin(freq * i + 0) * 127 + 128;
    green = sin(freq * i + 2 * PI/3) * 127 + 128;
    blue  = sin(freq * i + 4 * PI/3) * 127 + 128;
    setAll(red, green, blue);     
    showStrip();
    delay(100);
  }
  for (int i = colors; i > 0 ; i--)
  {    
    red   = sin(freq * i + 0) * 127 + 128;
    green = sin(freq * i + 2 * PI/3) * 127 + 128;
    blue  = sin(freq * i + 4 * PI/3) * 127 + 128;
    setAll(red, green, blue);     
    showStrip();
    delay(100);
  }
  delay(100);
}
