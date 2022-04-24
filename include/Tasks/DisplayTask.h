#include <Arduino.h>
#include <FreeRTOS.h>
#include <math.h>
#include <elapsedMillis.h>

#include <RF24.h>
#include <RF24Network.h>
#include <NRF24L01Lib.h>

#include <Types.h>
#include <M5Stack.h>

namespace DisplayTask
{
  ManagerData packet;

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
    m5.Lcd.setTextDatum(MC_DATUM);
    m5.Lcd.setTextSize(LCD_MED_FONT);
    m5.Lcd.drawString("READY", LCD_WIDTH / 2, LCD_HEIGHT / 2, 2);
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