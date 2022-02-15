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

#include "Types.h"

#include <M5Stack.h>

#define MANAGER_ID 00
#define HUD_ID 01

ManagerState::State managerState = ManagerState::WAITING;
ManagerData managerData;

elapsedMillis since_sent_to_manager = 0;

RF24 radio(NRF_CE, NRF_CS);
RF24Network network(radio);

void handlePacket(ManagerData *data)
{
  bool state_changed = managerData.state != data->state;

  if (state_changed)
  {
    switch (data->state)
    {
    case ManagerState::RUNNING:
      M5.Speaker.beep();
      break;
    case ManagerState::DUTYCYCLE_WARNING:
      break;
    case ManagerState::DUTYCYCLE_LIMIT:
      break;
    case ManagerState::REVERSE_STOP:
      M5.Speaker.beep();
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

bool sendPacket(HUD::Packet *packet, uint8_t type)
{
  uint8_t len = sizeof(HUD::Packet);
  uint8_t bs[len];
  memcpy(bs, packet, len);
  RF24NetworkHeader header(MANAGER_ID, type);

  bool connected = network.write(header, bs, len);

  // if (_sentPacketCallback != nullptr)
  //   _sentPacketCallback(data);

  return connected;
}

#include "Utils.h"

// Button2 button;

// #define BUTTON_PIN 35

// void buttonClick_handler(Button2 &btn)
// {
//   Serial.printf("Button clicked\n");
//   HUD::Packet packet;
//   packet.action = HUD::Action::BUTTON_CLICKED;

//   if (sendPacket(&packet, 1))
//   {
//     since_sent_to_manager = 0;
//   }
// }

//------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.printf("------------------------ BOOT ------------------------\n");

  // button.begin(BUTTON_PIN);
  // button.setPressedHandler(buttonClick_handler);

  NRF::setup(true);

  LEDs::setup();

  M5.begin(/*lcd enable*/ false);

  M5.Speaker.tone(NOTE_DH2, 200);

  LEDs::setPixels(LEDs::COLOUR_RED);

  Buttons::setup();
}
//---------------------------------------------------------------

bool manager_online = false;

void loop()
{
  NRF::update();

  Beeper::update();

  Buttons::update();

  if (since_sent_to_manager > 500)
  {
    since_sent_to_manager = 0;

    bool _online = manager_online;
    HUD::Packet packet;
    packet.id++;
    packet.action = HUD::Action::NONE;
    manager_online = sendPacket(&packet, 0);

    if (_online != manager_online)
    {
      _online = manager_online;
      LEDs::setPixels(manager_online
                          ? LEDs::COLOUR_GREEN
                          : LEDs::COLOUR_RED);
    }
  }

  delay(10);
}

//------------------------------------------------------------------
