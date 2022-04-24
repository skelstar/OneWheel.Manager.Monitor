#include <Arduino.h>
#include <FreeRTOS.h>
#include <math.h>
#include <elapsedMillis.h>

#include <RF24.h>
#include <RF24Network.h>
#include <NRF24L01Lib.h>
#include <GenericClient.h>

#include <Types.h>

namespace NrfTask
{
  const int MANAGER_ID = 00;
  const int HUD_ID = 01;

  elapsedMillis since_sent_to_manager = 0, since_printed_rx_packet;
  int _lastSentId = 0;

  RF24 radio(NRF_CE, NRF_CS);
  RF24Network network(radio);
  NRF24L01Lib nrf24;

  // prototypes

  // GenericClient</*OUT*/ VescData, /*IN*/ VescData> nrfClient(/*to*/ MANAGER_ID);
  GenericClient</*OUT*/ int, /*IN*/ int> *nrfClient;

  void nrfClientPacketAvailable_cb(uint16_t from_id, uint8_t t)
  {
    int data = nrfClient->read();

    if (since_printed_rx_packet > 1000)
    {
      since_printed_rx_packet = 0;
      Serial.printf("NRF received packet from Manager, id: %lu \n", data);
    }
  }

  void setup()
  {
    nrf24.begin(&radio, &network, /*addr*/ HUD_ID, /*cb*/ nullptr, /*multicast*/ false, /*print radio details*/ true);

    nrfClient = new GenericClient<int, int>(/*to*/ MANAGER_ID);

    nrfClient->begin(&network, nrfClientPacketAvailable_cb);
  }

  void Task(void *pvParameters)
  {
    unsigned long _last_packet_id = -1;

    // setup
    setup();

    Serial.printf("NrfTask ready\n");

    // work
    for (;;)
    {
      if (since_sent_to_manager > 3000)
      {
        since_sent_to_manager = 0;

        bool success = nrfClient->sendTo(1, _lastSentId);

        Serial.printf("Send to Manager %s, id: %lu \n", success ? "OK" : "FAIL", _lastSentId);
        _lastSentId++;
      }

      nrfClient->update();

      vTaskDelay(1);
    }

    vTaskDelete(NULL);
  }

  void createTask()
  {
    xTaskCreatePinnedToCore(
        Task,
        "NrfTask",
        10000, // stack size,
        NULL,  // params
        3,     // priority
        NULL,  // pvCreatedTask
        1      // core
    );
  }
}