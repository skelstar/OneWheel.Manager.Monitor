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
#include <Tasks/DisplayTask.h>

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

#include "Utils.h"

//------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.printf("------------------------ BOOT ------------------------\n");

  m5.begin();

  // button.begin(BUTTON_PIN);
  // button.setPressedHandler(buttonClick_handler);

  LEDs::setup();

  Beeper::beeper.update();

  M5.begin(/*lcd enable*/ false);

  M5.Speaker.tone(NOTE_DH2, 200);

  LEDs::setPixels(LEDs::COLOUR_RED);

  Buttons::setup();

  NrfTask::createTask();
  DisplayTask::createTask();
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
