// colors must be greater then LED_COUNT
void rainbow(int colors) {  
  float freq = 10.0 / colors;  
  byte red, green, blue;
  for (int j = 0; j < colors; j++)
  {
    for (int i = 0; i < LED_COUNT; i++)
    {
      red   = sin(freq * (i + j) + 0) * 127 + 128;
      green = sin(freq * (i + j) + 2 * PI/3) * 127 + 128;
      blue  = sin(freq * (i + j) + 4 * PI/3) * 127 + 128;
      setPixel(i, red, green, blue); 
    }
    showStrip();
    delay(100);
  }
  for (int j = colors; j > 0 ; j--)
  {
    for (int i = 0; i < LED_COUNT; i++)
    {
      red   = sin(freq * (i + j) + 0) * 127 + 128;
      green = sin(freq * (i + j) + 2 * PI/3) * 127 + 128;
      blue  = sin(freq * (i + j) + 4 * PI/3) * 127 + 128;
      setPixel(i, red, green, blue); 
    }
    showStrip();
    delay(100);
  }
  delay(100);
}
