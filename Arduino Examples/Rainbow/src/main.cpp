#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 2
#define NUM_LEDS 1
#define BRIGHTNESS 255
#define DELAY_MS 30

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(115200);
  Serial.setTxTimeoutMs(0);
  Serial.println("Starting...");

  strip.begin();
  strip.show();
  strip.setBrightness(BRIGHTNESS);
}

void loop()
{
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256)
  {
    for (int i = 0; i < strip.numPixels(); i++)
    {
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(DELAY_MS);
  }
}
