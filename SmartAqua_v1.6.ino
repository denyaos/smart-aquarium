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
ScheduledLeds leds(2); // pin #2

RtcDS3231<TwoWire> Rtc(Wire);

void setup () {
  //delay(5000);
  Serial.begin(115200);
  Serial.println("\nSmart Aquarium v1.6");
  Serial.println("\n\nsetup BEGIN");
  setupWifi();
  setupRelays();
  setupRTC();
  setupLeds();
  setupButtons();
  Serial.println("setup END\n\n");
}

void loop () {
  relays.checkRelaysState(getCurrentTime());
  serveRequests();
  serveLedLights();
  serveButtons();
  delay(20);
}
