#include <TM1637Display.h>
#include <WiFiManager.h>

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
  Serial.println("::setup::");

  configureDisplay();
  connectToWifi();
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
}

void loop() {
  showTime();
  delay(1000);
}

void configureDisplay() {
  Serial.println("::configureDisplay::");

  display.clear();
  display.setBrightness(3);  // 0 to 7
}

void connectToWifi() {
  Serial.println("::connectToWifi::");

  isConnected = wifiManager.autoConnect(WIFI_USERNAME, WIFI_PASSWORD);

  if (isConnected) {
    Serial.println("::connectToWifi:: WiFi connected");
    Serial.print("::connectToWifi:: IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("::connectToWifi:: Failed to connect or hit timeout");
  }
}

void showTime() {
  Serial.println("::showTime::");
  isHexEnabled = !isHexEnabled;

  if (isConnected) {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      display.showNumberDecEx(8888, 0b00000000);  // Not able to get local time
      Serial.println("::showTime:: [8888] Not able to get local time");
      return;
    }

    int hour = timeinfo.tm_hour;
    int minute = timeinfo.tm_min;

    int time = 0;

    if (hour == 0)
      time = minute;
    else
      time = (hour * 100) + minute;

    if (isHexEnabled) {
      if (hour == 0)
        display.showNumberDecEx(time, 0b01000000, true, 3);
      else
        display.showNumberDecEx(time, 0b01000000);
    } else {
      if (hour == 0)
        display.showNumberDecEx(time, 0b00000000, true, 3);
      else
        display.showNumberDecEx(time, 0b00000000);
    }
  } else {
    display.showNumberDecEx(9999, 0b00000000);  // Not connected
    Serial.println("::showTime:: [9999] Not connected");
  }
}