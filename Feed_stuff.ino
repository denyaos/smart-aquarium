bool feedingModeEnabled = false;

void enableFeedMode() {
  int feedTime = 15; //minutes

  int lampRelayIndex = 0;
  int lampRelayState = true; // ON
  
  int ledFeedLevel = 30; //%
  
  int pumpRelayIndex = 1;
  int pumpRelayState = false; // OFF

  relays.switchForMinutes(lampRelayIndex, lampRelayState, feedTime);
  relays.switchForMinutes(pumpRelayIndex, pumpRelayState, feedTime);
  leds.freezeLedLevel(ledFeedLevel, feedTime);
}

void toggleFeedingMode() {
  if (!feedingModeEnabled) {
      enableFeedMode();      
      feedingModeEnabled = true;
      Serial.println("Feeding mode enabled by button.");
  } else if (feedingModeEnabled) {
      disableFeedMode();
      feedingModeEnabled = false;
      Serial.println("Feeding mode disabled by button.");
  }
}

void disableFeedMode() {
  int lampRelayIndex = 0;
  int pumpRelayIndex = 1;
  
  relays.unfreezeNow(lampRelayIndex);
  relays.unfreezeNow(pumpRelayIndex);
  leds.unfreeze();
}
