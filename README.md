
# [Unity Sensor Board](https://loopon.tech/products/unity-sensor)<!-- omit from toc -->
This repository contains the pre-installed ESPHome config and example Arduino code projects for the Unity sensor board.
Specifications of the board can be found at https://loopon.tech/products/unity-sensor

<p align="center">
  <img width="260" src="./images/UnityBoard.JPG">
</p>

# Table of Contents<!-- omit from toc -->
- [Getting Started](#getting-started)
  - [Connecting to WiFi](#connecting-to-wifi)
  - [Home Assistant](#home-assistant)
  - [Modifying ESPHome Config](#modifying-esphome-config)
- [Flashing the Unity Board](#flashing-the-unity-board)
- [Arduino Example Projects](#arduino-example-projects)
    - [1. Blink](#1-blink)
    - [2. Bluetooth Keyboard](#2-bluetooth-keyboard)
    - [3. Human Presence](#3-human-presence)
    - [4. Rainbow](#4-rainbow)
    - [5. Temperature, Humidity and Ambient Light](#5-temperature-humidity-and-ambient-light)
    - [6. WiFi Scanning](#6-wifi-scanning)


# Getting Started
The Unity board comes pre-installed with ESPHome. The configuration can be found [here](/ESPHome/loopon_unity.yaml).

## Connecting to WiFi
There are two ways to connect the Unity board to your WiFi network:

### Using Imrpov<!-- omit from toc -->
1. Power up the Unity board.
1. Go to https://www.improv-wifi.com/ on your phone or a computer with Bluetooth.
1. Click "Connect device to Wi-Fi".
1. Select the Unity device.
<p align="center">
  <img width="360" src="./images/ImprovSelectDevice.png">
</p>

5. Input your SSID and password.
1. Click Save.

### Using the Access Point<!-- omit from toc -->
1. Power up the Unity board.
1. On you phone or computer, search for the "Unity" WiFi network.
1. Once connected, you will be redirected to WiFi setup page. If you are not redirected, open a browser and go to http://192.168.4.1
1. Select you WiFi network SSID and input the password if needed.
1. Click Save.
<p align="center">
  <img src="./images/AndroidWiFiScan.png" width="25%" style="display:inline;">
<img src="./images/AndroidWiFiPortal.png" width="25%" style="display:inline;">
</p>

## Home Assistant
Once you have successfully connected to your WiFi network, you can integrate the device to Home Assistant.
You must have the ESPHome Add-on installed. If you do not have the Add-on then follow the 'Installing ESPHome Dashboard' guide [here](https://esphome.io/guides/getting_started_hassio.html).

### Discovery<!-- omit from toc -->
Home Assistant and the ESPHome Add-on will automatically detect the Unity device.

You can find the device in:
* Integrations: <a href="https://my.home-assistant.io/redirect/integrations/" target="_blank"><img src="https://my.home-assistant.io/badges/integrations.svg"></a>
<p align="center">
  <img height="200" src="./images/HomeAssistantUnityDiscover.png">
</p>

* ESPHome Dashboard: <a href="https://my.home-assistant.io/redirect/supervisor_ingress/?addon=5c53de3b_esphome" target="_blank"><img src="https://my.home-assistant.io/badges/supervisor_addon.svg"></a>
<p align="center">
  <img height="200" src="./images/EspHomeUnityDiscover.png">
</p>

Note: If the device does not show up then please try restarting Home Assistant.

### View Device<!-- omit from toc -->
You can now view the Unity board in <a href="https://my.home-assistant.io/redirect/devices/" target="_blank"><img src="https://my.home-assistant.io/badges/devices.svg"></a>
<p align="center">
  <img width="360" src="./images/HomeAssistantUnityDevice.png">
</p>

For best results, use the recommended settings in the screenshot below for the human presence sensor.
<p align="center">
  <img width="360" src="./images/HomeAssistantUnityDeviceConfiguration.png">
</p>

## Modifying ESPHome Config
You can modify the way the device behaves by compiling and flashing the Unity board.

### Using ESPHome Dashboard (Easy)<!-- omit from toc -->
*(Work in progress)*

### Using ESPHome CLI (Medium)<!-- omit from toc -->
With ESPHome installed on your computer ([guide](https://esphome.io/guides/installing_esphome.html)), you can clone and modify the [loopon_unity.yaml](./ESPHome/loopon_unity.yaml), compile and flash the Unity board.

To compile and flash use:
```sh
# Make sure Unity board is in Flash mode
esphome run ./ESPHome/loopon_unity.yaml --no-logs
# Reset device when done
```

To monitor logs use:
```sh
esphome logs ./ESPHome/loopon_unity.yaml --device=COM19 
```

You can optionally add the device port to the commands above:
```sh
 --device=COM19 
```

# Flashing the Unity Board
To flash the Unity board, it must be put in the correct boot mode.
You can do this by:
1. Holding down the boot button.
1. Momentarily pressing the reset button.
1. Then, releasing the boot button.

Once flashing has completed, press the reset button to restart in normal mode.

# Arduino Example Projects

### PlatformIO<!-- omit from toc -->
PlatformIO is a cross-platform IDE extension for Visual Studio Code.
Use PlatformIO to open, build and upload the example projects to the Unity board. 
The installation guide can be found [here](https://platformio.org/install/ide?install=vscode).

### 1. [Blink](/Arduino%20Examples/Blink)
This project will blink LEDs on all GPIO ports.

### 2. [Bluetooth Keyboard](/Arduino%20Examples/BluetoothKeyboard)
This project will simulate a Bluetooth keyboard and will type a few sentences when connected to a computer or mobile device. 

### Windows Setup<!-- omit from toc -->
1. Go to Settings -> Bluetooth & Devices -> Add device.
1. Select Bluetooth.
1. After the scan has completed, Unity Keyboard should appear. Click and add device.
1. Open Notepad.exe and watch it type.

### Android Setup<!-- omit from toc -->
1. Go to Settings -> Device connection -> Pair new device.
1. Select Unity Keyboard in the list.
1. Open a notepad app and watch it type.

### 3. [Human Presence](/Arduino%20Examples/HumanPresence)
This project uses the LD2410 sensor to read the distance of a stationary or moving human.

### 4. [Rainbow](/Arduino%20Examples/Rainbow)
This project cycles through colours of the rainbow using the onboard RGB LED.

### 5. [Temperature, Humidity and Ambient Light](/Arduino%20Examples/TempHumLight)
This project prints the readings from the HDC1080 (temperature/humidity) and the BH1750 (light) sensors.

### 6. [WiFi Scanning](/Arduino%20Examples/WiFiScan)
This project will scan and print WiFi access points.
