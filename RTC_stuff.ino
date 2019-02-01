bool needToUpdateTime = false;
int timeSyncHour = 7;

void setupRTC() {
  Rtc.Begin();

  if (!Rtc.GetIsRunning()) {
    Rtc.SetIsRunning(true);
    Serial.println("RTC was not running, started...");
  }

  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
}

ScheduleTime getCurrentTime() {
  if (!Rtc.IsDateTimeValid() || isItTimeToUpdateRTC()) {
    Serial.print("\nRTC lost confidence in the DateTime!");
    if (!Rtc.IsDateTimeValid()) {
      Serial.print(" ...Rtc.IsDateTimeValid() is false.");  
    }
    Serial.println("");
    
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("  No WiFi connection yet, time acquision skipped.");
      delay(2000);
      if (Rtc.IsDateTimeValid()) {
        return ScheduleTime(Rtc.GetDateTime().Hour(), Rtc.GetDateTime().Minute(), Rtc.GetDateTime().Second());
      } else {
        return ScheduleTime();
      }
    }

    Serial.print("WiFi connected. IP address: ");
    Serial.print(WiFi.localIP());
    Serial.println("  Starting UDP");
    udp.begin(localPort);

    WiFi.hostByName(ntpServerName, timeServerIP);//get a random server from the pool
    sendNTPpacket(timeServerIP); // send an NTP packet to a time server
    delay(1000);
    int cb = udp.parsePacket();
    if (!cb) {
      Serial.print(".");
    }
    else {
      Serial.print("packet received, length = ");
      Serial.println(cb);
      
      udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]); //the timestamp starts at byte 40 of the received packet and is four bytes,
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]); // or two words, long. First, esxtract the two words: 
      unsigned long secsSince1900 = highWord << 16 | lowWord; // combine the four bytes (two words) into a long integer this is NTP time (seconds since Jan 1 1900)
      
      const unsigned long seventyYears = 2208988800UL; //Unix time starts on Jan 1 1970. In seconds, that's 2208988800
      const unsigned long hundredYears = 3155673600UL;
      unsigned long secsSince2000 = secsSince1900 - hundredYears;
      
      RtcDateTime utc = RtcDateTime(secsSince2000);

      //Define time offset (winter/summer time) 
      int offset = 2; // hours, offset for standard time
      int mont = utc.Month();
      int lastSunday = utc.Day() - utc.DayOfWeek();
      if ((mont > 3 && mont < 10) || (mont == 3 && lastSunday >= 25) || (mont == 10 && lastSunday < 25)) {
        offset = 3;
      }
      
      RtcDateTime dt = utc + (60 * 60 * offset);
      Serial.print("Current date/time is : ");
      printDateTime(dt);
      do {
        Serial.print("RTC is ");  
        if (!Rtc.IsDateTimeValid()) {
          Serial.println("not valid.");  
        } else {
          Serial.println("valid.");  
        }
        Serial.println("Nevertheless, setting date/time to RTC.");
        Rtc.SetDateTime(dt);
        Serial.print("now RTC is ");  
        if (!Rtc.IsDateTimeValid()) {
          Serial.println("not valid.");  
        } else {
          Serial.println("valid.");  
        }
        
      } while(!Rtc.IsDateTimeValid());
      needToUpdateTime = false;
    }
  } 
  return ScheduleTime(Rtc.GetDateTime().Hour(), Rtc.GetDateTime().Minute(), Rtc.GetDateTime().Second());
}

bool isItTimeToUpdateRTC() {
  needToUpdateTime |= Rtc.GetDateTime().Hour() == timeSyncHour;
  return needToUpdateTime;
} 

#define countof(a) (sizeof(a) / sizeof(a[0]))
void printDateTime(const RtcDateTime& dt) {
  char datestring[20];
  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  Serial.println(datestring);
}
