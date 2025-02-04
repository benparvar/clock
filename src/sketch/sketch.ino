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

void setup() {
  Serial.begin(115200);
  Serial.println("setup");
  
  configureDisplay();
  connectToWifi();
}

void loop() {
  display.showNumberDecEx(1530, 0b11100000, false, 4, 0);
}

void configureDisplay() {
  Serial.println("configureDisplay");

  display.clear();
  display.setBrightness(4);  // 0 to 7
}

void connectToWifi() {
  Serial.println("connectToWifi");

  isConnected = wifiManager.autoConnect(WIFI_USERNAME, WIFI_PASSWORD);

  if (isConnected) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Failed to connect or hit timeout");
  }
}