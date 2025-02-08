#include <TM1637Display.h>
#include <WiFiManager.h>
#include <ArduinoLog.h>

#define CLK 22                    // The ESP32 pin GPIO22 connected to CLK
#define DIO 23                    // The ESP32 pin GPIO23 connected to DIO
#define WIFI_USERNAME "IoT"       // WiFi username
#define WIFI_PASSWORD "NcC-1701"  // WiFi password
#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET -3 * 3600  // GMT -3
#define UTC_OFFSET_DST 0

TM1637Display display = TM1637Display(CLK, DIO);  // Display
WiFiManager wifiManager;                          // WiFi manager

bool isSensorAvailable = false;
bool isConnected = false;
bool isHexEnabled = false;

void setup() {
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial, true);  // Log
  Log.notice("::setup::" CR);
  configureDisplay();
  connectToWifi();
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
}

void loop() {
  showTime();
  delay(1000);
}

void configureDisplay() {
  Log.notice("::configureDisplay::" CR);
  display.clear();
  display.setBrightness(3);  // 0 to 7
}

void connectToWifi() {
  Log.notice("::connectToWifi::" CR);
  isConnected = wifiManager.autoConnect(WIFI_USERNAME, WIFI_PASSWORD);

  if (isConnected) {
    Log.verbose("::connectToWifi::  WiFi connected -> IP address:  %p" CR, WiFi.localIP());
  } else {
    Log.error("::connectToWifi:: Failed to connect or hit timeout" CR);
  }
}

void showTime() {
  Log.notice("::showTime::" CR);
  isHexEnabled = !isHexEnabled;

  if (isConnected) {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      display.showNumberDecEx(8888, 0b00000000);  // Not able to get local time
      Log.error("::showTime:: [8888] Not able to get local time" CR);
      return;
    }

    int time = (timeinfo.tm_hour * 100) + timeinfo.tm_min;

    isHexEnabled ? display.showNumberDecEx(time, 0b01000000, true) : display.showNumberDecEx(time, 0b00000000, true);

  } else {
    display.showNumberDecEx(9999, 0b00000000);  // Not connected
    Log.error("::showTime:: [9999] Not connected" CR);
  }
}