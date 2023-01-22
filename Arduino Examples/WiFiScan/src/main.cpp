#include <Arduino.h>
#include "WiFi.h"

#define DELAY_MS 1000

void setup()
{
  Serial.begin(115200);
  Serial.setTxTimeoutMs(0);
  Serial.println("Starting...");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.setTxPower(WIFI_POWER_15dBm);
}

void loop()
{
  Serial.print("Starting WiFi scan...");
  int n = WiFi.scanNetworks();
  Serial.println(" Done");
  if (n == 0)
  {
    Serial.println("No networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found:");
    for (int i = 0; i < n; ++i)
    {
      Serial.print("  ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
}
