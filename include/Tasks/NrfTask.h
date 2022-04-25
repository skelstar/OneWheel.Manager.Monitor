#include <Arduino.h>
#include <FreeRTOS.h>
#include <math.h>
#include <elapsedMillis.h>

#include <RF24.h>
#include <RF24Network.h>
#include <NRF24L01Lib.h>
#include <GenericClient.h>

#include <Types.h>

namespace NRFTask
{
  const int MANAGER_ID = 00;
  const int HUD_ID = 01;

  elapsedMillis since_sent_to_manager = 0, since_printed_rx_packet;

  RF24 radio(NRF_CE, NRF_CS);
  RF24Network network(radio);
  NRF24L01Lib nrf24;

  ManagerData packet;

  // prototypes

  GenericClient</*OUT*/ int, /*IN*/ ManagerData> *managerClient;

  void managerClientPacketAvailable_cb(uint16_t from_id, uint8_t t);

  // -----------------------------------------

  void setup()
  {
    nrf24.begin(&radio, &network, /*addr*/ HUD_ID, /*cb*/ nullptr, /*multicast*/ false, /*print radio details*/ true);

    managerClient = new GenericClient<int, ManagerData>(/*to*/ MANAGER_ID);

    managerClient->begin(&network, managerClientPacketAvailable_cb);
  }

  void Task(void *pvParameters)
  {
    unsigned long _last_packet_id = -1;

    // setup
    setup();

    Serial.printf("NRFTask ready\n");

    // work
    for (;;)
    {
      if (since_sent_to_manager > 3000)
      {
        since_sent_to_manager = 0;

        packet.packet_id++;

        if (xSemaphoreTake(semaphore_SPI, TICKS_50ms) == pdPASS)
        {
          portENTER_CRITICAL(&mmux);

          bool success = managerClient->sendTo(1, _last_packet_id);

          Serial.printf("Send to Manager %s, id: %lu \n", success ? "OK" : "FAIL", _last_packet_id++);
          vTaskDelay(TICKS_10ms);
          xSemaphoreGive(semaphore_SPI);
          portEXIT_CRITICAL(&mmux);
        }
        else
        {
          Serial.printf("Failed to take semaphore: %s \n", __func__);
        }
      }

      managerClient->update();

      vTaskDelay(1);
    }

    vTaskDelete(NULL);
  }

  void managerClientPacketAvailable_cb(uint16_t from_id, uint8_t t)
  {
    if (xSemaphoreTake(semaphore_SPI, TICKS_50ms) == pdPASS)
    {
      portENTER_CRITICAL(&mmux);
      ManagerData data = managerClient->read();
      vTaskDelay(TICKS_10ms);
      xSemaphoreGive(semaphore_SPI);
      xQueueOverwrite(xManagerDataQueue, (void *)&data);
      portEXIT_CRITICAL(&mmux);

      if (since_printed_rx_packet > 1000)
      {
        since_printed_rx_packet = 0;
        Serial.printf("NRF received packet from Manager, id: %lu \n", data.packet_id);
      }
    }
    else
    {
      Serial.printf("Failed to take semaphore: %s \n", __func__);
    }
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