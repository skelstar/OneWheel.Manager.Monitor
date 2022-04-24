#include <Arduino.h>

#ifdef DEBUG_SERIAL
#define DEBUG_OUT Serial
#endif
#define PRINTSTREAM_FALLBACK
#include "Debug.hpp"

#include <elapsedMillis.h>

#include <RF24.h>
#include <RF24Network.h>
#include <Button2.h>
#include <Pulser.h>

#include "Types.h"

#include <M5Stack.h>

#include <Tasks/NrfTask.h>

ManagerState::State managerState = ManagerState::WAITING;
ManagerData managerData;

elapsedMillis since_sent_to_manager = 0;

RF24 radio(NRF_CE, NRF_CS);
RF24Network network(radio);

#define NOTE_DH2 661

namespace Beeper
{
  void beeper_cb(uint8_t state);

  Pulser beeper(beeper_cb);
}

void handlePacket(ManagerData *data)
{
  bool state_changed = managerData.state != data->state;

  if (state_changed)
  {
    switch (data->state)
    {
    case ManagerState::RUNNING:
      Beeper::beeper.pulses(Pulser::FAST_TWO);
      break;
    case ManagerState::DUTYCYCLE_WARNING:
      break;
    case ManagerState::DUTYCYCLE_LIMIT:
      break;
    case ManagerState::REVERSE_STOP:
      Beeper::beeper.pulses(Pulser::ONE_LONG);
      break;
    case ManagerState::BOTH_FEET_ON_PAD:
      break;
    case ManagerState::WAITING:
    case ManagerState::HEARTBEAT:
    case ManagerState::ONE_FOOT_ON_PAD:
      break;
    default:
      Serial.printf("Not handling ManagerData.state %d\n", data->state);
    }
  }

  managerData.id = data->id;
  managerData.state = data->state;
}

#include "Utils.h"

//------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.printf("------------------------ BOOT ------------------------\n");

  // button.begin(BUTTON_PIN);
  // button.setPressedHandler(buttonClick_handler);

  LEDs::setup();

  Beeper::beeper.update();

  M5.begin(/*lcd enable*/ false);

  M5.Speaker.tone(NOTE_DH2, 200);

  LEDs::setPixels(LEDs::COLOUR_RED);

  Buttons::setup();

  NrfTask::createTask();
}
//---------------------------------------------------------------

bool manager_online = false;

void loop()
{
  // NRF::update();

  Beeper::update();

  Buttons::update();

  // if (since_sent_to_manager > 500)
  // {
  //   since_sent_to_manager = 0;

  //   bool _online = manager_online;
  //   HUD::Packet packet;
  //   packet.id++;
  //   packet.action = HUD::Action::NONE;
  //   manager_online = sendPacket(&packet, 0);

  //   if (_online != manager_online)
  //   {
  //     _online = manager_online;
  //     if (manager_online)
  //     {
  //       LEDs::colour = LEDs::COLOUR_GREEN;
  //       LEDs::leds.forever();
  //     }
  //     else
  //     {
  //       LEDs::colour = LEDs::COLOUR_RED;
  //       LEDs::leds.forever(200, 1000);
  //     }
  //   }
  // }

  LEDs::update();

  delay(10);
}

//------------------------------------------------------------------
