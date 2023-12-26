#include "SmoothLighting.h"
#include <Adafruit_NeoPixel.h>

SmoothLights:SmoothLights(Adafruit_NeoPixel strip) {
    _strip = strip;
}

SmoothLights::begin(void) {
    _refreshrate = 240;
    _adjustmentValue = 10;
    _segmented = false;
    _numLeds = strip.numPixels();
    _startHue = 65536 / 2;
    _endHue = _startHue;
    _maxBrightness = 255;

    _timeLastUpdated = millis() - int(1000 / _refreshrate);
    _currentPos = 0;
    _maximumPos = _maxBrightness * _numLeds;

    _strip.begin();
    _strip.clear();
    _strip.show();
}