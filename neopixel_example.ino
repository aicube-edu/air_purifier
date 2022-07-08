#include <Adafruit_NeoPixel.h>
const int PIN = D3;
const int NUMPIXELS = 1;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
pixels.begin();
}
void loop() {
pixels.setPixelColor(0, pixels.Color(255,0,0));
pixels.show();
delay(1000);
pixels.setPixelColor(0, pixels.Color(0,255,0));
pixels.show();
delay(1000);
pixels.setPixelColor(0, pixels.Color(0,0,255));
pixels.show();
delay(1000);
}
