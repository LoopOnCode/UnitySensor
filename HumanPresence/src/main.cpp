#include <Arduino.h>
#include <ld2410.h>

#define RX_PIN 20
#define TX_PIN 21
#define BAUDRATE 256000
#define DELAY_MS 500

ld2410 radar;
uint32_t lastReading = 0;

void setup()
{
  Serial.begin(115200);
  Serial.setTxTimeoutMs(0);
  Serial.println("Starting...");

  Serial1.begin(BAUDRATE, SERIAL_8N1, RX_PIN, TX_PIN);

  while (!radar.begin(Serial1))
  {
    Serial.println("Sensor not connected!");
    delay(1000);
  }
  Serial.println("Sensor connected.");
}

void loop()
{
  radar.read(); // Always read frames from the sensor
  if (radar.isConnected() && millis() - lastReading > DELAY_MS)
  {
    lastReading = millis();
    if (radar.presenceDetected())
    {
      if (radar.stationaryTargetDetected())
      {
        Serial.print("Distance: ");
        Serial.print(radar.stationaryTargetDistance());
        Serial.println("cm - Stationary");
      }
      if (radar.movingTargetDetected())
      {
        Serial.print("Distance: ");
        Serial.print(radar.movingTargetDistance());
        Serial.println("cm - Moving");
      }
    }
    else
    {
      Serial.println("No target");
    }
  }
}
