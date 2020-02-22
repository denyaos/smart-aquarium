#include <ScheduledLeds.h>
#include <Relays.h>
#include <ScheduledRelay.h>
#include <ScheduleTime.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

char ssid[] = "Kurion";
char pass[] = "parolwifi";
unsigned int localPort = 2390;
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];
WiFiUDP udp;
WiFiServer server(80);

Relays relays;
ScheduledLeds leds(0); // pin gpio0 (D3)

RtcDS3231<TwoWire> Rtc(Wire);

void setup () {
  
  Serial.begin(115200);
  Serial.println("\nSmart Aquarium");
  Serial.println("\n\nsetup BEGIN");
  
  setupWifi();
  setupRelay();
  setupRTC();
  setupLeds();
  setupButtons();
  
  Serial.println("setup END\n\n");
}

void loop () {
  
  if (Rtc.IsDateTimeValid()) {
    RtcDateTime now = Rtc.GetDateTime();
    relayLoop(now);
    ledLoop(now);
  }

  rtcLoop();
  requestLoop();
  buttonLoop();

  statusLoop();

  delay(20);
}


unsigned long lastStatusTime = 0;
void statusLoop() {
  if  ((millis() - lastStatusTime) > 10000) {

    Serial.println("\n~~~~Status~~~~");
    rtcStatusLoop();
    wifiStatusLoop();
    ledStatusLoop();
    relayStatusLoop();
    lastStatusTime = millis();  
    Serial.println("~~~~~~~~~~~~~~\n");
  }
 
}

/**
d0 -> GPIO16
d1 -> GPIO5
d2 -> GPIO4
d3 -> GPIO0
d4 -> GPIO2

d5 -> GPIO14
d6 -> GPIO12
d7 -> GPIO13
d8 -> GPIO15
*/



