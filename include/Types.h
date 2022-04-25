#pragma once

#include <Arduino.h>

enum BalanceState
{
  STARTUP = 0,
  RUNNING = 1,
  RUNNING_TILTBACK_DUTY = 2,
  RUNNING_TILTBACK_HIGH_VOLTAGE = 3,
  RUNNING_TILTBACK_LOW_VOLTAGE = 4,
  FAULT_ANGLE_PITCH = 6,
  FAULT_ANGLE_ROLL = 7,
  FAULT_SWITCH_HALF = 8,
  FAULT_SWITCH_FULL = 9,
  FAULT_DUTY = 10,
  FAULT_STARTUP = 11,
  FAULT_REVERSE = 12,
  _BALANCE_STATE_LIMIT
};

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
  double inputVoltage = 0.0;
  // double tachometer = 0.0;
  // double tachometerAbsolut = 0.0;

  // double pidOutput = 0.0;
  // double pitch = 0.0;
  // double roll = 0.0;
  // double motorCurrent = 0.0;
  // double motorPosition = 0.0;
  uint16_t balanceState = 0;
  uint16_t switchState = 0;
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
