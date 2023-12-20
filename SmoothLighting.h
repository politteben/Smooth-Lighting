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
    SmoothLights(Adafruit_Neopixel strip);

    void begin(void);
    void update(void);
    void setTarget(double target);

    bool hasArrived(void);
  private:
    int _refreshrate;
    int _adjustmentValue;
    bool _segmented;
    int _numSegments;
    int _numLeds;
    int _ledsPerSegment;
    int _ledPin;
    uint16_t _startHue;
    uint16_t _endHue;

    long _timeLastUpdated;
    double _targetPercent;
    long _currentPos;
    long _maximumPos;
};

#endif