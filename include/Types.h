#pragma once

#include <Arduino.h>

class ManagerData
{
public:
  unsigned long packet_id = 0;
  double dutyCycle = 0.0;
  double erpm = 0.0;
  // double current = 0.0;
  double ampHours = 0.0;
  // double ampHoursCharged = 0.0;
  // double wattHours = 0.0;
  // double wattHoursCharged = 0.0;
  // double mosfetTemp = 0.0;
  // double motorTemp = 0.0;
  // double totalCurrentIn = 0.0;
  // double inputVoltage = 0.0;
  // double tachometer = 0.0;
  // double tachometerAbsolut = 0.0;

  // double pidOutput = 0.0;
  // double pitch = 0.0;
  // double roll = 0.0;
  // double motorCurrent = 0.0;
  // double motorPosition = 0.0;
  // uint16_t balanceState = 0;
  // uint16_t switchState = 0;
  // double adc1 = 0.0;
  // double adc2 = 0.0;
};

class VescData
{
public:
  unsigned long packet_id = 0;
  uint8_t majorVersion;
  uint8_t minorVersion;
  std::string name;
  std::string uuid;

  double dutyCycle = 0.0;
  double erpm = 0.0;
  double current = 0.0;
  double ampHours = 0.0;
  double ampHoursCharged = 0.0;
  double wattHours = 0.0;
  double wattHoursCharged = 0.0;
  double mosfetTemp = 0.0;
  double motorTemp = 0.0;
  double totalCurrentIn = 0.0;
  double pidPosition = 0.0;
  double inputVoltage = 0.0;
  double tachometer = 0.0;
  double tachometerAbsolut = 0.0;

  double pidOutput = 0.0;
  double pitch = 0.0;
  double roll = 0.0;
  uint32_t loopTime = 0;
  double motorCurrent = 0.0;
  double motorPosition = 0.0;
  uint16_t balanceState = 0;
  uint16_t switchState = 0;
  double adc1 = 0.0;
  double adc2 = 0.0;
  uint8_t fault = 0;
};

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

  // static const char *getState(uint8_t state)
  // {
  //   switch (state)
  //   {
  //   case WAITING:
  //     return "WAITING";
  //   case HEARTBEAT:
  //     return "HEARTBEAT";
  //   case RUNNING:
  //     return "RUNNING";
  //   case DUTYCYCLE_LIMIT:
  //     return "DUTYCYCLE_LIMIT";
  //   case DUTYCYCLE_WARNING:
  //     return "DUTYCYCLE_WARNING";
  //   case REVERSE_STOP:
  //     return "REVERSE_STOP";
  //   case ONE_FOOT_ON_PAD:
  //     return "ONE_FOOT_ON_PAD";
  //   case BOTH_FEET_ON_PAD:
  //     return "BOTH_FEET_ON_PAD";
  //   default:
  //     return "Unknown ManagerState!";
  //   }
  // }
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

#ifndef TICKS_5ms
const TickType_t TICKS_0ms = 0;
const TickType_t TICKS_5ms = 5 / portTICK_PERIOD_MS;
const TickType_t TICKS_10ms = 10 / portTICK_PERIOD_MS;
const TickType_t TICKS_50ms = 50 / portTICK_PERIOD_MS;
const TickType_t TICKS_100ms = 100 / portTICK_PERIOD_MS;
const TickType_t TICKS_500ms = 500 / portTICK_PERIOD_MS;
const TickType_t TICKS_1s = 1000 / portTICK_PERIOD_MS;
const TickType_t TICKS_2s = 2000 / portTICK_PERIOD_MS;
const TickType_t TICKS_3s = 3000 / portTICK_PERIOD_MS;
const TickType_t TICKS_4s = 4000 / portTICK_PERIOD_MS;
#endif
