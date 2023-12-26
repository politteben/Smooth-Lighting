#include <Adafruit_NeoPixel.h>

const int refreshRate = 240;
const int adjustmentValue = 1;
const bool segmented = false;
const int numSegments = 14;
const int numLeds = 10;
const int ledsPerSegment = numLeds / numSegments;
const int ledPin = 8;
const uint16_t startHue = 0;
const uint16_t endHue = 65536 / 2;
const int maxBrightness = 100;

unsigned long timeLastUpdated = 0;
double targetPercent = 0;

long currentPos = 0;
long maximumPos = segmented ? maxBrightness * numSegments : maxBrightness * numLeds;

Adafruit_NeoPixel strip(numLeds, ledPin, NEO_GRB + NEO_KHZ800);

unsigned long waitTime = 0;

void setup() {
  Serial.begin(115200);

  strip.begin();
  strip.setBrightness(maxBrightness);
  strip.clear();
  strip.show();

  delay(1000);
}

void loop() {
  update();
  if(millis() - waitTime > 4000) {
    targetPercent = double(random(100)) / 100;
    Serial.println(targetPercent);
    waitTime = millis();
  }
}

void update() {
  if(millis() - timeLastUpdated >= 1000 / refreshRate) {
    long targetPos = maximumPos * targetPercent;
    targetPos = constrain(targetPos, 0, maximumPos);

    currentPos += targetPos > currentPos ? adjustmentValue : adjustmentValue * -1;
    if(targetPos > currentPos) {
      currentPos = constrain(currentPos, 0, targetPos);
    } else {
      currentPos = constrain(currentPos, targetPos, maximumPos);
    }

    int current = currentPos / maxBrightness;
    int brightness = currentPos % maxBrightness;
    uint16_t hue = map(current, 0, segmented ? numSegments : numLeds, startHue, endHue);

    //Update Current
    //If Segmented, this will not half-light a segment
    if(segmented && current != int(targetPos / maxBrightness)) {
      strip.fill(strip.gamma32(strip.ColorHSV(hue, 255, brightness)), current * ledsPerSegment, ledsPerSegment);
    }else if (!segmented) {
      strip.setPixelColor(current, strip.gamma32(strip.ColorHSV(hue, 255, brightness)));
    }
    
    if(targetPos > currentPos && current != 0) {
      //Make Lower Full Brightness Just In-Case (Only if it's greater than 0)
      current--;
      hue = map(current, 0, segmented ? numSegments : numLeds, startHue, endHue);

      if(segmented) {
        strip.fill(strip.gamma32(strip.ColorHSV(hue, 255, maxBrightness)), current * ledsPerSegment, ledsPerSegment);
      }else {
        strip.setPixelColor(current, strip.gamma32(strip.ColorHSV(hue, 255, maxBrightness)));
      }
    } else if(targetPos < currentPos && current != int(maximumPos / maxBrightness)) {
      //Turn Upper Off just In-Case (if it's not at the maximum)
      current++;
      hue = map(current, 0, segmented ? numSegments : numLeds, startHue, endHue);

      if(segmented) {
        strip.fill(strip.gamma32(strip.ColorHSV(hue, 255, 0)), current * ledsPerSegment, ledsPerSegment);
      }else {
        strip.setPixelColor(current, strip.gamma32(strip.ColorHSV(hue, 255, 0)));
      }
    }
    
    strip.show();
    timeLastUpdated = millis();
  }
}
