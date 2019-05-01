void enableFeedMode() {
  int feedTime = 10; //minutes

  int lampRelayIndex = 0;
  int lampRelayState = true; // ON

  int ledFeedLevel = 25; //%

  int pumpRelayIndex = 1;
  int pumpRelayState = false; // OFF

  relays.switchForMinutes(lampRelayIndex, lampRelayState, feedTime);
  relays.switchForMinutes(pumpRelayIndex, pumpRelayState, feedTime);
  leds.freezeLedLevel(ledFeedLevel, feedTime);
}

void toggleFeedingMode() {
  static bool feedingModeEnabled = false;
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
