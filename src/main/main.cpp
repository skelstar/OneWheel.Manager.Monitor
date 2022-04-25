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

QueueHandle_t xManagerDataQueue;

SemaphoreHandle_t semaphore_SPI;

portMUX_TYPE mmux = portMUX_INITIALIZER_UNLOCKED;

#include <Tasks/NrfTask.h>
#include <Tasks/DisplayTask.h>

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

  xManagerDataQueue = xQueueCreate(1, sizeof(ManagerData *));

  semaphore_SPI = xSemaphoreCreateMutex();

  m5.begin(/*LCDEnable*/ true, /*SDEnable*/ false, /*SerialEnable*/ false, /*I2CEnable*/ false);

  // m5.begin(true, false, false, false);

  // button.begin(BUTTON_PIN);
  // button.setPressedHandler(buttonClick_handler);

  LEDs::setup();

  Beeper::beeper.update();

  m5.Speaker.tone(NOTE_DH2, 200);

  LEDs::setPixels(LEDs::COLOUR_RED);

  Buttons::setup();

  NRFTask::createTask();
  DisplayTask::createTask();
}
//---------------------------------------------------------------

bool manager_online = false;

void loop()
{
  Beeper::update();

  Buttons::update();

  LEDs::update();

  delay(10);
}

//------------------------------------------------------------------
