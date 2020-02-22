void setupWifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address) {
  Serial.print("Sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

void requestLoop() {
//  static bool isServerRunning = false;
//  if (isServerRunning) {
//    WiFiClient client = server.available(); // Check if a client has connected
//    if (!client) {return;}
//    
//    Serial.println("new client");
//    while(!client.available()){ // Wait until the client sends some data
//      delay(5);
//      Serial.print(".");
//    }
//
//    String response = handleRequest(client.readStringUntil('\r'));
//  
//    client.flush();
//    // Send the response to the client
//    client.print(response);
//    delay(1);
//    Serial.println("Client disonnected");
//
//    // The client will actually be disconnected when the function returns and 'client' object is detroyed
//  
//  } else {
//    if (WiFi.status() == WL_CONNECTED) {
//      server.begin();
//      isServerRunning = true;
//      Serial.print("Waiting for requests at ");
//      Serial.println(WiFi.localIP());
//    } 
//  }
  
}


void wifiStatusLoop() {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("wifi connected. ip: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("wifi not connected");
    }
}

