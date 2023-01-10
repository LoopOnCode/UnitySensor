#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 2
#define NUM_LEDS 1
#define LED_CHIPSET WS2812
#define BRIGHTNESS 255
#define DELAY_MS 25

CRGB leds[NUM_LEDS];

void setup()
{
  Serial.begin(115200);
  Serial.setTxTimeoutMs(0);
  Serial.println("Starting Rainbow...");

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop()
{
  for (int j = 0; j < 255; j++)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CHSV(i - (j * 2), 255, BRIGHTNESS);
    }
    FastLED.show();
    delay(DELAY_MS);
  }
}
