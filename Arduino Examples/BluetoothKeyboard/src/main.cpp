#include <Arduino.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("Unity Keyboard", "LoopOn");

void setup()
{
  Serial.begin(115200);
  Serial.setTxTimeoutMs(0);

  Serial.println("Starting...");
  bleKeyboard.begin();
}

void loop()
{
  if (bleKeyboard.isConnected())
  {
    Serial.print("Typing...");
    delay(500); // Required for stability between Serial and BleKeyboard
    bleKeyboard.print("Hello there!");
    delay(1000);
    bleKeyboard.println(" This is Unity.");
    Serial.println(" Done");
  }
  else
  {
    Serial.println("Keyboard not connected");
  }
  Serial.println("Waiting 5 seconds...");
  delay(5000);
}
