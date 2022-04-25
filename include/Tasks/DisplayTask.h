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
  elapsedMillis since_printed_queue_read = 0;
  uint16_t _lastbalanceState = _BALANCE_STATE_LIMIT;
  unsigned long _last_packet_id = -1;

  const int LCD_WIDTH = 320;
  const int LCD_HEIGHT = 240;

  const int LCD_XLARGE_FONT = 7;
  const int LCD_LARGE_FONT = 5;
  const int LCD_MED_FONT = 3;
  const int LCD_SMALL_FONT = 2;
  const int LCD_XSMALL_FONT = 1;

  // prototypes
  void displayBalanceState(uint16_t balanceState);

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
    setup();

    Serial.printf("DisplayTask ready\n");

    // work
    for (;;)
    {
      ManagerData *packet = nullptr;

      if (xQueuePeek(xManagerDataQueue, &(packet), TICKS_50ms) == pdTRUE)
      {
        if (packet != nullptr)
        {
          bool newPacket = packet->packet_id != _last_packet_id;

          if (newPacket)
          {
            if (_lastbalanceState != packet->balanceState)
            {
              Serial.printf("Balance state now: %d \n", packet->balanceState);
              displayBalanceState(packet->balanceState);
            }
          }

          _lastbalanceState = packet->balanceState;
          _last_packet_id = packet->packet_id;
        }
        else
        {
          Serial.printf("packet is nullptr \n");
        }
      }

      vTaskDelay(1);
    }

    vTaskDelete(NULL);
  }

  void updateScreen(const char *balanceState)
  {
    m5.Lcd.clearDisplay();
    m5.Lcd.setTextSize(1);
    m5.Lcd.setTextColor(TFT_BLUE);
    m5.Lcd.setTextDatum(MC_DATUM);
    m5.Lcd.setTextSize(LCD_MED_FONT);
    m5.Lcd.drawString(balanceState, LCD_WIDTH / 2, LCD_HEIGHT / 2, 2);
  }

  void displayBalanceState(uint16_t balanceState)
  {
    switch (balanceState)
    {
    case STARTUP:
      updateScreen("STARTUP");
      break;
    case RUNNING:
      updateScreen("RUNNING");
      break;
    case RUNNING_TILTBACK_DUTY:
      updateScreen("TILTBACK_DUTY");
      break;
    case RUNNING_TILTBACK_HIGH_VOLTAGE:
      updateScreen("TILTBACK_HIGH_VOLTAGE");
      break;
    case RUNNING_TILTBACK_LOW_VOLTAGE:
      updateScreen("TILTBACK_LOW_VOLTAGE");
      break;
    case FAULT_ANGLE_PITCH:
      updateScreen("FAULT_ANGLE_PITCH");
      break;
    case FAULT_ANGLE_ROLL:
      updateScreen("FAULT_ANGLE_ROLL");
      break;
    case FAULT_SWITCH_HALF:
      updateScreen("FAULT_HALF");
      break;
    case FAULT_SWITCH_FULL:
      updateScreen("FAULT_FULL");
      break;
    case FAULT_DUTY:
      updateScreen("FAULT_DUTY");
      break;
    case FAULT_STARTUP:
      updateScreen("FAULT_STARTUP");
      break;
    case FAULT_REVERSE:
      updateScreen("FAULT_REVERSE");
      break;
    }
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