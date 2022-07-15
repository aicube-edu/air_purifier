#include <Adafruit_NeoPixel.h>
#define PIN 3
#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
uint8_t Rc=0,Gc=0,Bc=0;

void setup() {
  Serial.begin(9600);
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(Rc, Gc, Bc));
  pixels.show();
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'

  if(Serial.available())
  {
    int targetRed = Serial.parseInt();
    int targetGreen = Serial.parseInt();
    int targetBlue = Serial.parseInt();
    ColorChange(0,targetRed,targetGreen,targetBlue);
  }
  /*pixels.setPixelColor(0,pixels.Color(0, 150, 0));
  //pixels.setPixelColor(0,0xffff00);
  pixels.show();   */
}

void ColorChange(unsigned int N, uint8_t Rt, uint8_t Gt, uint8_t Bt)
{
  int dR = Rt - Rc;
  int dG = Gt - Gc;
  int dB = Bt - Bc;
  int showSpeed = 3;

  if (dR > 0)
  {
    for (int i=0 ; i <= dR ; i++)
    {
      pixels.setPixelColor(N, pixels.Color(Rc + i, Gc, Bc));
      pixels.show(); delay(showSpeed);
    }
  }
  else 
  {
    for (int i=0 ; i <= (-1)*dR ; i++)
    {
      pixels.setPixelColor(N, pixels.Color(Rc - i, Gc, Bc));
      pixels.show(); delay(showSpeed);
    }
  }

  if (dG > 0)
  {
    for (int i=0 ; i <= dG ; i++)
    {
      pixels.setPixelColor(N, pixels.Color(Rt, Gc + i, Bc));
      pixels.show(); delay(showSpeed);
    }
  }
  else 
  {
    for (int i=0 ; i <= (-1)*dG ; i++)
    {
      pixels.setPixelColor(N, pixels.Color(Rt, Gc - i, Bc));
      pixels.show(); delay(showSpeed);
    }
  }

  if (dB > 0)
  {
    for (int i=0 ; i <= dB ; i++)
    {
      pixels.setPixelColor(N, pixels.Color(Rt, Gt, Bc + i));
      pixels.show(); delay(showSpeed);
    }
  }
  else 
  {
    for (int i=0 ; i <= (-1)*dB ; i++)
    {
      pixels.setPixelColor(N, pixels.Color(Rt, Gt, Bc - i));
      pixels.show(); delay(showSpeed);
    }
  }

  Rc = Rt;
  Gc = Gt;
  Bc = Bt;  
}
