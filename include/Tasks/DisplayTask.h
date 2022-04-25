#include <Arduino.h>
#include <FreeRTOS.h>
#include <math.h>
#include <elapsedMillis.h>

#include <RF24.h>
#include <RF24Network.h>
#include <NRF24L01Lib.h>

#include <Types.h>

namespace DisplayTask
{
  ManagerData packet;
  elapsedMillis since_printed_queue_read = 0;

  const int LCD_WIDTH = 320;
  const int LCD_HEIGHT = 240;

  const int LCD_XLARGE_FONT = 7;
  const int LCD_LARGE_FONT = 5;
  const int LCD_MED_FONT = 3;
  const int LCD_SMALL_FONT = 2;
  const int LCD_XSMALL_FONT = 1;

  // prototypes

  void setup()
  {
    if (xSemaphoreTake(semaphore_SPI, TICKS_50ms) == pdPASS)
    {
      portENTER_CRITICAL(&mmux);

      m5.Lcd.setTextSize(2);
      m5.Lcd.setTextColor(TFT_BLUE);
      m5.Lcd.setTextDatum(MC_DATUM);
      m5.Lcd.setTextSize(LCD_MED_FONT);
      m5.Lcd.drawString("READY", LCD_WIDTH / 2, LCD_HEIGHT / 2, 2);

      xSemaphoreGive(semaphore_SPI);

      portEXIT_CRITICAL(&mmux);
    }
    else
    {
      Serial.printf("Failed to take semaphore: %s \n", __func__);
    }
  }

  void Task(void *pvParameters)
  {
    unsigned long _last_packet_id = -1;

    // setup
    setup();

    Serial.printf("DisplayTask ready\n");

    // work
    for (;;)
    {
      if (xQueuePeek(xManagerDataQueue, &packet, TICKS_50ms))
      {
        bool newPacket = packet.packet_id != _last_packet_id;
        if (newPacket)
        {
          _last_packet_id = packet.packet_id;
          if (since_printed_queue_read > 1000)
          {
            since_printed_queue_read = 0;
            Serial.printf("ManagerData queue id: %lu\n", packet.packet_id);
          }
        }
      }

      vTaskDelay(1);
    }

    vTaskDelete(NULL);
  }

  void createTask()
  {
    xTaskCreatePinnedToCore(
        Task,
        "DisplayTask",
        10000, // stack size,
        NULL,  // params
        3,     // priority
        NULL,  // pvCreatedTask
        1      // core
    );
  }
}