#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>
#include "ClosedCube_HDC1080.h"

ClosedCube_HDC1080 hdc1080;
BH1750 lightMeter(0x23); // Unsoldered: 0x23 / Soldered: 0x5C

void setup()
{
  Serial.begin(115200);
  Serial.setTxTimeoutMs(0);
  Serial.println("Starting Sensors...");

  Wire.begin(5, 6);
  hdc1080.begin(0x40);
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
}

void loop()
{
  Serial.print("Temperature: ");
  Serial.print(hdc1080.readTemperature());
  Serial.print("°C, Relative Humidity: ");
  Serial.print(hdc1080.readHumidity());
  Serial.print("%, Light: ");
  Serial.print(lightMeter.readLightLevel());
  Serial.println("lx");
  delay(3000);
}
