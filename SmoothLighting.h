/*!
 *  @file SmoothLighting.h
 *
 *  This is a library for smoothly animating between values with Neopixels
 *
 *  Designed to aid me in Product Walk Demos where the marjority of the time
 *  I am tasked with lighting a strip or segments of LEDs.
 *
 *  Ben Politte
 *
 *  BSD license, all text above must be included in any redistribution
 */

#ifndef SmoothLighting_h
#define SmoothLighting_h

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class SmoothLights
{
  public:
    SmoothLights(Adafruit_NeoPixel strip);

    void begin(void);
    void begin(uint16_t startHue, uint16_t stopHue, bool segmented = false, uint16_t numOfSegments = 14, uint16_t shiftAmount = 10);

    void update(void);
    void setTarget(double percent);
    void clear(void);
    void setBrightness(uint8_t brightness);
    void adjustClipping(uint16_t lower, uint16_t upper);

    bool hasArrived(void) {
      return _currentPos == _targetPos;
    }
  private:
    int _refreshrate;
    int _adjustmentValue;
    bool _segmented;
    int _numSegments;
    int _numLeds;
    int _ledsPerSegment;
    uint16_t _startHue;
    uint16_t _endHue;
    uint16_t _lowerClipping;
    uint16_t _upperClipping;
    uint8_t _maxBrightness;
    Adafruit_NeoPixel _strip;
    bool _hasBegun;
    bool _newTarget;

    long _timeLastUpdated;
    long _targetPos;
    long _currentPos;
    long _maximumPos;
};

#endif