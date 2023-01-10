#include <Arduino.h>

#define DELAY_MS 1000

void setup()
{
  Serial.begin(115200);
  Serial.setTxTimeoutMs(0);
  Serial.println("Starting Blink...");

  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop()
{
  Serial.println("On");
  digitalWrite(0, 1);
  digitalWrite(1, 1);
  digitalWrite(3, 1);
  digitalWrite(4, 1);
  digitalWrite(7, 1);
  digitalWrite(10, 1);
  delay(DELAY_MS);

  Serial.println("Off");
  digitalWrite(0, 0);
  digitalWrite(1, 0);
  digitalWrite(3, 0);
  digitalWrite(4, 0);
  digitalWrite(7, 0);
  digitalWrite(10, 0);
  delay(DELAY_MS);
}
