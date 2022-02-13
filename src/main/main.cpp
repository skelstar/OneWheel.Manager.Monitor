#include <Arduino.h>

#ifdef DEBUG_SERIAL
#define DEBUG_OUT Serial
#endif
#define PRINTSTREAM_FALLBACK
#include "Debug.hpp"

#include <elapsedMillis.h>

#include <RF24.h>
#include <RF24Network.h>
#include <NRF24L01Lib.h>
#include <GenericClient.h>
#include <Button2.h>

#include "Types.h"

#define MANAGER_ID 00
#define HUD_ID 01

ManagerState::State managerState = ManagerState::WAITING;
ManagerData managerData;

elapsedMillis since_sent_to_manager = 0;

RF24 radio(NRF_CE, NRF_CS);
RF24Network network(radio);
NRF24L01Lib nrf24;

GenericClient</*OUT*/ HUD::Packet, /*IN*/ ManagerData> managerClient(MANAGER_ID);

void managerClientPacketAvailable_cb(uint16_t from_id, uint8_t t)
{
  ManagerData payload = managerClient.read();

  if (payload.id != managerData.id)
  {
    // new data
    if (payload.state != managerData.state)
    {
      // state changed
      Serial.printf("ManagerState changed: %s\n", ManagerState::getState(payload.state));
      managerData = payload;
    }
  }
  // Serial.printf("Rx packet from Manager: %d\n", payload.id);
}

bool sendPacketToManager(bool print)
{
  HUD::Packet packet;
  packet.id = managerData.id;
  packet.action = HUD::Action::NONE;

  bool success = managerClient.sendTo(0, packet);

  return success;
}

Button2 button;

#define BUTTON_PIN 35

void buttonClick_handler(Button2 &btn)
{
  Serial.printf("Button clicked\n");
  HUD::Packet packet;
  packet.action = HUD::Action::BUTTON_CLICKED;
  managerClient.sendTo(1, packet);
  since_sent_to_manager = 0;
}

//------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.printf("------------------------ BOOT ------------------------\n");

  nrf24.begin(&radio, &network, HUD_ID, /*cb*/ nullptr, /*multicast*/ false, /*print radio details*/ true);

  button.begin(BUTTON_PIN);
  button.setPressedHandler(buttonClick_handler);

  managerClient.begin(&network, managerClientPacketAvailable_cb);
  Serial.printf("managerClient ready\n");

  vTaskDelay(100);
}
//---------------------------------------------------------------

void loop()
{
  managerClient.update();

  button.loop();

  if (since_sent_to_manager > 1000)
  {
    since_sent_to_manager = 0;
    if (!sendPacketToManager(true))
    {
      Serial.printf("Send packet to manager failed\n");
    }
    else
    {
      // Serial.printf("Send packet to manager OK\n");
    }
  }

  vTaskDelay(10);
}

//------------------------------------------------------------------
