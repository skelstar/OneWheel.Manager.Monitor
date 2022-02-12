#include <Arduino.h>

namespace ManagerState
{
  typedef enum
  {
    WAITING,
    HEARTBEAT,
    RUNNING,
    DUTYCYCLE_LIMIT,
    DUTYCYCLE_WARNING,
    REVERSE_STOP,
    ONE_FOOTPAD_SENSOR_NOT_READING,
    BOTH_FOOTPAD_SENSORS_NOT_READING,
  } State;

  static const char *getState(uint8_t state)
  {
    switch (state)
    {
    case WAITING:
      return "WAITING";
    case HEARTBEAT:
      return "HEARTBEAT";
    case RUNNING:
      return "RUNNING";
    case DUTYCYCLE_LIMIT:
      return "DUTYCYCLE_LIMIT";
    case DUTYCYCLE_WARNING:
      return "DUTYCYCLE_WARNING";
    case REVERSE_STOP:
      return "REVERSE_STOP";
    case ONE_FOOTPAD_SENSOR_NOT_READING:
      return "ONE_FOOTPAD_SENSOR_NOT_READING";
    case BOTH_FOOTPAD_SENSORS_NOT_READING:
      return "BOTH_FOOTPAD_SENSORS_NOT_READING";
    default:
      return "Unknown ManagerState!";
    }
  }
}

class ManagerData
{
public:
  unsigned long id;
  ManagerState::State state;
};

struct HUDResponse
{
  unsigned long id;
} response;