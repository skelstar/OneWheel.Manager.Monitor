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

#define MANAGER_ID 00
#define HUD_ID 01

RF24 radio(NRF_CE, NRF_CS);
RF24Network network(radio);
NRF24L01Lib nrf24;

class HUDType
{
public:
  unsigned long id;
};

GenericClient<HUDType, HUDType> managerClient(MANAGER_ID);

void managerClientPacketAvailable_cb(uint16_t from_id, uint8_t t)
{
  HUDType payload = managerClient.read();
  Serial.printf("Rx packet from Manager: %d\n", payload.id);
}

bool sendPacketToManager(bool print)
{
  HUDType data;
  data.id = 0;

  bool success = managerClient.sendTo(0, data);

  return success;
}

//------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.printf("------------------------ BOOT ------------------------\n");

  nrf24.begin(&radio, &network, HUD_ID, /*cb*/ nullptr, /*multicast*/ false, /*print radio details*/ true);

  managerClient.begin(&network, managerClientPacketAvailable_cb);
  Serial.printf("managerClient ready\n");

  vTaskDelay(100);
}
//---------------------------------------------------------------

elapsedMillis since_sent_to_manager = 0;

void loop()
{
  managerClient.update();

  if (since_sent_to_manager > 1000)
  {
    since_sent_to_manager = 0;
    if (!sendPacketToManager(true))
    {
      Serial.printf("Send packet to manager failed\n");
    }
    else
    {
      Serial.printf("Send packet to manager OK\n");
    }
  }

  vTaskDelay(10);
}

//------------------------------------------------------------------
