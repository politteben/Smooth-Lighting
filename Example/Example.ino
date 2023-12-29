#include <Adafruit_NeoPixel.h>
#include <SmoothLighting.h>

const int numSegments = 14;
const int numLeds = 10;
const int ledsPerSegment = numLeds / numSegments;
const int ledPin = 8;
const uint16_t startHue = 0;
const uint16_t endHue = 65536 / 3;

Adafruit_NeoPixel strip(numLeds, ledPin, NEO_GRB + NEO_KHZ800);
SmoothLights leds(strip);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  
  leds.begin(startHue, endHue, true, 5, 1);
  leds.setBrightness(75);

  leds.setTarget(1.0);
}

void loop() {
  leds.update();
  if(leds.hasArrived()) {
    delay(1000);
    double targetPercent = double(random(100)) / 100;
    leds.setTarget(targetPercent);
    Serial.println(targetPercent);
  }
}
