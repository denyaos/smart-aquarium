unsigned long lastSyncTime = 0;
int timeSyncInterval = 24; ///hours

void setupRTC() {
  
  Rtc.Begin();

  if (!Rtc.GetIsRunning()) {
    Rtc.SetIsRunning(true);
    Serial.println("RTC was not running, started");
  } else {
    Serial.println("RTC is running");  
  }

  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
  
}

bool timeIsValid() {
  if (!Rtc.IsDateTimeValid()) {
    Serial.println("Rtc.IsDateTimeValid() is false");
    return false;
  } else if (isItTimeToUpdateRTC()) {
    Serial.println("Time to sync rtc");
    return false;
  } else {
    return true;
  }
}

void rtcLoop() {
  
  if (!timeIsValid()) {
    
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("No WiFi connection, time acquision skipped");
      return;
    } else {
      Serial.println("Wifi connection present, trying to get time");
      udp.begin(localPort);
      WiFi.hostByName(ntpServerName, timeServerIP);//get a random server from the pool
      sendNTPpacket(timeServerIP); // send an NTP packet to a time server
      delay(1000);
      int cb = udp.parsePacket();
      if (!cb) {
        Serial.println("no packet received");
        return;
      } else {
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
        Serial.print("Received date/time : ");
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

        lastSyncTime = millis();
      }
    }
  }
}

bool isItTimeToUpdateRTC() {
  return lastSyncTime  == 0 || (millis() - lastSyncTime) > (timeSyncInterval * 3600 * 1000);
} 

int getSecondOfDay(const RtcDateTime& dt) {
  return dt.Hour() * 3600 + dt.Minute() * 60 + dt.Second();  
}

void rtcStatusLoop() {
  if (timeIsValid()) {
    RtcDateTime now = Rtc.GetDateTime();
    printDateTime(now);
     
    Serial.print("Second of day: ");
    Serial.println(getSecondOfDay(now));
  
  } else {
    Serial.println("RTC is not relevant");
  }
}

void requestTimeSync() {
  lastSyncTime = 0;
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
