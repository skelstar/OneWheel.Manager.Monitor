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
    ONE_FOOT_ON_PAD,
    BOTH_FEET_ON_PAD,
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
    case ONE_FOOT_ON_PAD:
      return "ONE_FOOT_ON_PAD";
    case BOTH_FEET_ON_PAD:
      return "BOTH_FEET_ON_PAD";
    default:
      return "Unknown ManagerState!";
    }
  }
}

namespace HUD
{
  enum Action
  {
    NONE,
    BUTTON_A_CLICKED,
    BUTTON_B_CLICKED,
    BUTTON_C_CLICKED,
  };

  static const char *getAction(uint8_t action)
  {
    switch (action)
    {
    case NONE:
      return "NONE";
    case BUTTON_A_CLICKED:
      return "BUTTON_A_CLICKED";
    case BUTTON_B_CLICKED:
      return "BUTTON_B_CLICKED";
    case BUTTON_C_CLICKED:
      return "BUTTON_C_CLICKED";
    default:
      return "unhanded action";
    }
  }

  struct Packet
  {
    unsigned long id;
    HUD::Action action;
  };
}

class ManagerData
{
public:
  unsigned long id;
  ManagerState::State state;
};
