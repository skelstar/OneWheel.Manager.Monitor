#pragma once

#include <Arduino.h>
#include <elapsedMillis.h>

typedef void (*SetOutputCallback)(uint8_t state);

// override in platformio.ini
// #define PULSE_FAST_ON_DURATION 50
// #define PULSE_FAST_OFF_DURATION 150
// #define PULSE_ONE_LONG_DURATION 800

class Pulser
{
private:
  elapsedMillis _sincePulse;
  unsigned int _numPulses = 0;
  bool _forever = false;
  SetOutputCallback _setOutput_cb = nullptr;
  uint8_t _state = LOW;

public:
  enum PulsePattern
  {
    FAST_ONE,
    FAST_TWO,
    FAST_THREE,
    ONE_LONG,
  };

  ulong _onDuration = 0, _offDuration = 0;
  Pulser(SetOutputCallback setOutput_cb, uint8_t initialState = LOW)
  {
    _setOutput_cb = setOutput_cb;
    _setOutput_cb(initialState);
  }

  void pulses(unsigned int numPulses, ulong onDuration, ulong offDuration, uint8_t initialState = HIGH)
  {
    _numPulses = numPulses;
    _forever = false;
    _onDuration = onDuration;
    _offDuration = offDuration;
    _sincePulse = 0;
    _state = initialState;
    _setOutput_cb(_state);
  }

  void pulses(PulsePattern pattern)
  {
    switch (pattern)
    {
    case FAST_ONE:
      pulses(1, PULSE_FAST_ON_DURATION, PULSE_FAST_OFF_DURATION);
      break;
    case FAST_TWO:
      pulses(2, PULSE_FAST_ON_DURATION, PULSE_FAST_OFF_DURATION);
      break;
    case FAST_THREE:
      pulses(3, PULSE_FAST_ON_DURATION, PULSE_FAST_OFF_DURATION);
      break;
    case ONE_LONG:
      pulses(1, PULSE_ONE_LONG_DURATION, 0);
      break;
    default:
      Serial.printf("WARNING: unhandled pulse pattern: %d\n", pattern);
      break;
    }
  }

  void forever(ulong onDuration, ulong offDuration, uint8_t initialState = HIGH)
  {
    _forever = true;
    _onDuration = onDuration;
    _offDuration = offDuration;
    _sincePulse = 0;
    _state = initialState;
    _setOutput_cb(_state);
    _numPulses = 0;
  }

  void forever(uint8_t initialState = HIGH)
  {
    _forever = true;
    _onDuration = ULONG_MAX;
    _offDuration = 0;
    _sincePulse = 0;
    _state = initialState;
    _setOutput_cb(_state);
    _numPulses = 0;
  }

  void update()
  {
    bool timeToToggle = (_state == HIGH && _sincePulse > _onDuration) ||
                        (_state == LOW && _sincePulse > _offDuration);
    bool pulsesRemain = _numPulses > 0 || _forever;

    if (timeToToggle && pulsesRemain)
    {
      _state = !_state;
      _setOutput_cb(_state);
      _sincePulse = 0;
      if (_state == LOW && !_forever)
        _numPulses--;
    }
  }

  void stop()
  {
    _numPulses = 0;
    _state = LOW;
    _setOutput_cb(_state);
    _forever = false;
  }
};