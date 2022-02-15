#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Pulser.h>

#define NOTE_D0 -1
#define NOTE_D1 294
#define NOTE_D2 330
#define NOTE_D3 350
#define NOTE_D4 393
#define NOTE_D5 441
#define NOTE_D6 495
#define NOTE_D7 556

#define NOTE_DL1 147
#define NOTE_DL2 165
#define NOTE_DL3 175
#define NOTE_DL4 196
#define NOTE_DL5 221
#define NOTE_DL6 248
#define NOTE_DL7 278

#define NOTE_DH1 589
#define NOTE_DH2 661
#define NOTE_DH3 700
#define NOTE_DH4 786
#define NOTE_DH5 882
#define NOTE_DH6 990
#define NOTE_DH7 112

//-----------------------------------------
namespace NRF
{
  void managerClientPacketAvailable_cb(uint16_t from_id, uint8_t t);

  void setup(bool printDetails)
  {
    radio.begin();
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_250KBPS);

    network.begin(HUD_ID);
    radio.flush_rx();
    radio.flush_tx();

    if (printDetails)
      radio.printDetails(); // Dump the configuration of the rf unit for debugging
  }

  void update()
  {
    network.update();
    if (network.available())
    {
      RF24NetworkHeader header;
      network.peek(header);
      managerClientPacketAvailable_cb(header.from_node, header.type);
    }
  }

  void managerClientPacketAvailable_cb(uint16_t from_id, uint8_t t)
  {
    RF24NetworkHeader header;
    ManagerData data;
    uint8_t len = sizeof(ManagerData);
    uint8_t buff[len];

    network.read(header, buff, len);
    memcpy(&data, &buff, len);

    // Serial.printf("rx id: %lu| state: %s from Manager\n", data.id, ManagerState::getState(data.state));

    handlePacket(&data);
  }

}

//-----------------------------------------
namespace Buttons
{
  void sendAction(HUD::Action action)
  {
    Serial.printf("Sending action: %s\n", HUD::getAction(action));

    HUD::Packet packet;
    packet.action = action;

    if (sendPacket(&packet, 1))
    {
      since_sent_to_manager = 0;
    }
  }

  void setup()
  {
  }

  void update()
  {
    if (M5.BtnA.wasPressed())
    {
      sendAction(HUD::BUTTON_A_CLICKED);
    }
    else if (M5.BtnB.wasPressed())
    {
      sendAction(HUD::BUTTON_B_CLICKED);
    }
    else if (M5.BtnC.wasPressed())
    {
      sendAction(HUD::BUTTON_C_CLICKED);
    }
  }
}
//-----------------------------------------
namespace Beeper
{
  void update()
  {
    M5.update();
  }
}

//-----------------------------------------
namespace LEDs
{
#define M5STACK_FIRE_NEO_NUM_LEDS 10
#define M5STACK_FIRE_NEO_DATA_PIN 15

  // prototypes
  void pulse_cb(uint8_t state);

  Pulser leds(pulse_cb);

  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(M5STACK_FIRE_NEO_NUM_LEDS, M5STACK_FIRE_NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);

  uint32_t COLOUR_RED = pixels.Color(PIXEL_BRIGHTNESS_MAX, 0, 0);
  uint32_t COLOUR_GREEN = pixels.Color(0, PIXEL_BRIGHTNESS_MAX, 0);
  uint32_t COLOUR_BLUE = pixels.Color(0, 0, PIXEL_BRIGHTNESS_MAX);
  uint32_t COLOUR_WHITE = pixels.Color(PIXEL_BRIGHTNESS_MAX / 2, PIXEL_BRIGHTNESS_MAX / 2, PIXEL_BRIGHTNESS_MAX / 2);
  uint32_t COLOUR_BLACK = pixels.Color(0, 0, 0);

  uint32_t colour = COLOUR_BLACK;

  void setPixels(uint32_t colour);

  void pulse_cb(uint8_t state)
  {
    if (state == 1)
    {
      setPixels(colour);
    }
    else
    {
      setPixels(COLOUR_BLACK);
    }
  }

  void setup()
  {
    pixels.begin();
    setPixels(COLOUR_WHITE);
  }

  void setPixels(uint32_t colour)
  {
    for (int i = 0; i < M5STACK_FIRE_NEO_NUM_LEDS; i++)
    {
      pixels.setPixelColor(i, colour);
    }
    pixels.show();
  }

  void update()
  {
    leds.update();
  }
}
//-----------------------------------------