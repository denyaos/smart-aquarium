int relayOnTime = 9;
int relayOffTime = 22;

bool relayEnabled = true;
int pumpRelayPin = 2; //GPIO2 = D4
bool relayOnSignal = LOW;

void setupRelay() {

  //convert hours to seconds
  relayOnTime = relayOnTime * 60 * 60;
  relayOffTime = relayOffTime * 60 * 60;
  Serial.print("pump operation time: from ");
  Serial.print(relayOnTime);
  Serial.print(" to ");
  Serial.print(relayOffTime);
  Serial.println(" second of day");

  pinMode(pumpRelayPin, OUTPUT);
}


void relayLoop(const RtcDateTime& now) {

  int currentSecondOfDay = getSecondOfDay(now);

  if (freezeUntil > millis()) {
     if (relayEnabled) {
      turnOffRelay();
    }
  } else if (currentSecondOfDay < relayOnTime) {
    if (relayEnabled) {
      turnOffRelay();
    }
  } else if (currentSecondOfDay < relayOffTime) {
    if (!relayEnabled) {
      turnOnRelay();
    }
  } else {
    if (relayEnabled) {
      turnOffRelay();
    }
  }
}

void turnOnRelay() {
  digitalWrite(pumpRelayPin, relayOnSignal ? HIGH : LOW);
  relayEnabled = true;
  Serial.println("pump relay enabled");
}

void turnOffRelay() {
  digitalWrite(pumpRelayPin, relayOnSignal ? LOW : HIGH);
  relayEnabled = false;
  Serial.println("pump relay disabled");
}

void relayStatusLoop() {
  Serial.print("RELAY status: ");
  Serial.println(relayEnabled ? "on" : "off");
}
