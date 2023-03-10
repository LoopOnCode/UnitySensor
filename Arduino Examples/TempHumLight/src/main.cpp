#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>
#include "ClosedCube_HDC1080.h"

#define IIC_SDA_PIN 5
#define IIC_SCL_PIN 6
#define DELAY_MS 3000

ClosedCube_HDC1080 hdc1080;
BH1750 lightMeter(0x23); // Unsoldered: 0x23 / Soldered: 0x5C
float tempOffset = -7;

void setup()
{
  Serial.begin(115200);
  Serial.setTxTimeoutMs(0);
  Serial.println("Starting...");

  Wire.begin(IIC_SDA_PIN, IIC_SCL_PIN);
  hdc1080.begin(0x40);
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
}

float calcTemp()
{
  float internalTemp = temperatureRead();
  float externalTemp = hdc1080.readTemperature();
  return externalTemp - (abs(internalTemp - externalTemp) / 2.0) + tempOffset;
}

void loop()
{
  Serial.print("Temperature: ");
  Serial.print(calcTemp());
  Serial.print("°C, Relative Humidity: ");
  Serial.print(hdc1080.readHumidity());
  Serial.print("%, Light: ");
  Serial.print(lightMeter.readLightLevel());
  Serial.println("lx");
  delay(DELAY_MS);
}
