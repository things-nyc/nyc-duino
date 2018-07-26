# nyc-duino

A simple demo board for the TTN NYC Hardware Office Hours!

## Soldering (optional)

Simple introduction to soldering by attaching the DHT11/22 and a 10k thru-hole pullup resistor. Male or female header can also be added to support Arduino shields.

## Arduino

### Installing

```
https://www.arduino.cc/en/Main/Software
```

### Libraries Needed

Sketch -> Include Library -> Manage Libraries

* Adafruit Unified Sensor - by Adafruit
* DHT sensor library - by Adafruit
* TheThingsNetwork - by The Things Network
* CayenneLPP - by The Things Network

## The Things Network

The Things Network is our LoRaWAN network server.

### Create Account

### Create Application

### Add Cayenne Integration

Also choose Cayenne LPP from Payload Formats tab.

### Add Device

Upload DeviceInfo sketch from TheThingsNetwork library to get DevEUI.

## Firmware

Download nycDuino-demo.ino from this repo.

Use the AppEUI and AppKey from TTN dashboard to personalize the Arduino sketch on lines 6 and 7.

Tools -> Board -> Arduino Leonardo

Upload.

## Cayenne

### Create Account

### Add Device

LoRa -> The Things Network -> Cayenne LPP
