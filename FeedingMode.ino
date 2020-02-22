unsigned long freezeUntil = 0;
int feedTime = 10; //minutes

void toggleFeedingMode() {
  static bool feedingModeEnabled = false;
  if (feedingModeEnabled) {
    freezeUntil = millis();
    feedingModeEnabled = false;
    Serial.println("Feeding mode disabled by button.");
  } else {
    freezeUntil = millis() + feedTime * 1000 * 60;
    feedingModeEnabled = true;
    Serial.println("Feeding mode enabled by button.");
  }
}
