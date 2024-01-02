#include "SmoothLighting.h"
#include <Adafruit_NeoPixel.h>

/*!
  @brief    Initiate SmoothLights
  @param    strip  Neopixel strip for Smooth Lights
*/
SmoothLights::SmoothLights(Adafruit_NeoPixel strip) {
    _strip = strip;
}

/*!
  @brief    Initial Configure of Variables for Smooth Lights To Work (Simplified)
*/
void SmoothLights::begin(void) {
    _refreshrate = 240;
    _adjustmentValue = 10;
    _segmented = false;
    _numLeds = _strip.numPixels();
    _startHue = 65536 / 2;
    _endHue = _startHue;
    _maxBrightness = 255;
    _lowerClipping = 0;
    _upperClipping = 0;
    _hasBegun = false;

    _timeLastUpdated = millis() - int(1000 / _refreshrate);
    _currentPos = 0;
    _maximumPos = _maxBrightness * (_segmented ? _numSegments : _numLeds);
    _targetPos = 0.0;

    _strip.begin();
    _strip.clear();
    _strip.show();
}

/*!
  @brief    Initial Configure of Variables for Smooth Lights To Work (Expanded)
  @param    startHue        Beginning hue of leds in strip
  @param    stopHue         Final hue of leds in strip
  @param    segmented       If LEDs are chuncked together into segments
  @param    numOfSegments   Total nunmber of segments
  @param    shiftAmount     How much brightness with change every time the leds are updated
*/
void SmoothLights::begin(uint16_t startHue, uint16_t stopHue, bool segmented, uint16_t numOfSegments, uint16_t shiftAmount) {
    _strip.begin();

    _refreshrate = 240;
    _adjustmentValue = shiftAmount;
    _segmented = segmented;
    _numLeds = _strip.numPixels();
    _numSegments = numOfSegments;
    _ledsPerSegment = _numLeds / _numSegments;
    _startHue = startHue;
    _endHue = stopHue;
    _maxBrightness = 255;
    _lowerClipping = 0;
    _upperClipping = 0;
    _hasBegun = false;

    _timeLastUpdated = millis() - int(1000 / _refreshrate);
    _currentPos = 0;
    _maximumPos = _maxBrightness * (_segmented ? _numSegments : _numLeds);
    _targetPos = 0.0;

    _strip.clear();
    _strip.show();

    clear();
}

/*!
  @brief    Update the Leds if applicable
*/
void SmoothLights::update(void) {
  if(millis() - _timeLastUpdated >= (1000 / _refreshrate)) {
    //_hasBegun is set to true when this is run for the first time
    //I use this to know if it needs to be cleared if a change is made later to things like _maxBrightness
    if(!_hasBegun) { _hasBegun = true; }

    //Adjust the _currentPos by the _adjustmentValue in the correct direction, making sure not to overshoot
    //This will end the function if the _currentPos is the same as the _targetPos
    if(_targetPos > _currentPos) {
      _currentPos += _adjustmentValue;
      _currentPos = constrain(_currentPos, 0, _targetPos);
    } else if (_targetPos < _currentPos) {
      _currentPos -= _adjustmentValue;
      _currentPos = constrain(_currentPos, _targetPos, _maximumPos);
    } else {
      return;
    }

    //Find the right hue for the current pixel or segment
    int current = _currentPos / _maxBrightness;
    int brightness = _currentPos % _maxBrightness;
    uint16_t hue;
    if(current < _lowerClipping) {
      hue = _startHue;
    } else if (current > ((_segmented ? _numSegments : _numLeds) - _upperClipping)) {
      hue = _endHue;
    } else {
      hue = map(current, _lowerClipping, ((_segmented ? _numSegments : _numLeds) - _upperClipping), _startHue, _endHue);
    }

    //Update Current
    //If Segmented, this will not half-light a segment
    if(_segmented && current != int(_targetPos / _maxBrightness)) {
      _strip.fill(_strip.gamma32(_strip.ColorHSV(hue, 255, brightness)), current * _ledsPerSegment, _ledsPerSegment);
    }else if (!_segmented) {
      _strip.setPixelColor(current, _strip.gamma32(_strip.ColorHSV(hue, 255, brightness)));
    } else {
      _targetPos = _currentPos;
    }
    
    if(_targetPos > _currentPos && current != 0) {
      //Make Lower _maxBrightness Just In-Case (Only if it's greater than 0)
      current--;
      if(current < _lowerClipping) {
        hue = _startHue;
      } else if (current > ((_segmented ? _numSegments : _numLeds) - _upperClipping)) {
        hue = _endHue;
      } else {
        hue = map(current, _lowerClipping, ((_segmented ? _numSegments : _numLeds) - _upperClipping), _startHue, _endHue);
      }

      if(_segmented) {
        _strip.fill(_strip.gamma32(_strip.ColorHSV(hue, 255, _maxBrightness)), current * _ledsPerSegment, _ledsPerSegment);
      }else {
        _strip.setPixelColor(current, _strip.gamma32(_strip.ColorHSV(hue, 255, _maxBrightness)));
      }
    } else if(_targetPos < _currentPos && current != int(_maximumPos / _maxBrightness)) {
      //Turn Upper Off just In-Case (if it's not at the maximum)
      current++;
      if(current < _lowerClipping) {
        hue = _startHue;
      } else if (current > ((_segmented ? _numSegments : _numLeds) - _upperClipping)) {
        hue = _endHue;
      } else {
        hue = map(current, _lowerClipping, ((_segmented ? _numSegments : _numLeds) - _upperClipping), _startHue, _endHue);
      }

      if(_segmented) {
        _strip.fill(_strip.gamma32(_strip.ColorHSV(hue, 255, 0)), current * _ledsPerSegment, _ledsPerSegment);
      }else {
        _strip.setPixelColor(current, _strip.gamma32(_strip.ColorHSV(hue, 255, 0)));
      }
      }
      
      _strip.show();
      _timeLastUpdated = millis();
  }
}

/*!
  @brief    Set new target percent
  @param    percent     Percentage (0.0-1.0) that the leds should be filled
*/
void SmoothLights::setTarget(double percent) {
  _targetPos = _maximumPos * percent;
  _targetPos = constrain(_targetPos, 0, _maximumPos);
}

/*!
  @brief    Clear the leds
*/
void SmoothLights::clear(void) {
    setTarget(0.0);
    _currentPos = 0.0;
    _strip.clear();
    _strip.show();
    _timeLastUpdated = millis();
}

/*!
  @brief    Change maxiumum brightness of the leds
  @param    brightness      New brightness value
*/
void SmoothLights::setBrightness(uint8_t brightness) {
    _maxBrightness = brightness;
    _maximumPos = _maxBrightness * (_segmented ? _numSegments : _numLeds);
    double percent = _targetPos / _maximumPos;
    setTarget(percent);
    long currentPosSaved = _currentPos;
    delay(450);
    clear();
    _currentPos = currentPosSaved;
}

/*!
  @brief    Add/adjust clipping of the led hue color.
            By adjusting the lower or upper clipping values,
            the hue for leds below or above the clipping values will be
            either the startHue or endHue
  @param    lower   Lower clipping amount
  @param    upper   Upper clipping amount
*/
void SmoothLights::adjustClipping(uint16_t lower, uint16_t upper) {
  _lowerClipping = lower;
  _upperClipping = upper;

  if(_hasBegun) {
    long currentPosSaved = _currentPos;
    delay(450);
    clear();
    _currentPos = currentPosSaved;
  }
}