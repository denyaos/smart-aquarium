int ledOnTime = 12; //hour
int ledOffTime = 22; //hour

int fadePeriod = 60; //minutes
int ledFeedLevelInPercent = 30; //%
int ledMaxRateInPercent = 90; // %

const int LED_MAX = 1023;
const int LED_MIN = 0;

int ledPin = 0; //GPIO0 = D3

int currentLedPower = 0;

void setupLeds() {
  //convert to seconds
  ledOnTime = ledOnTime * 3600;
  ledOffTime = ledOffTime * 3600;
  fadePeriod = fadePeriod * 60;
  Serial.print("led operation time: from ");
  Serial.print(ledOnTime);
  Serial.print(" to ");
  Serial.print(ledOffTime);
  Serial.println(" second of day");

  pinMode(ledPin, OUTPUT);
}


void ledLoop(const RtcDateTime& now) {

  int currentSecondOfDay = getSecondOfDay(now);
  currentLedPower = calculateLedPower(currentSecondOfDay);

  if (freezeUntil > millis()) {
    setLedPower(percentToPower(ledFeedLevelInPercent));
  } else if (currentSecondOfDay >= ledOnTime && currentSecondOfDay < ledOffTime) {
    setLedPower(min(currentLedPower, percentToPower(ledMaxRateInPercent)));
  } else {
    setLedPower(LED_MIN);
  }
}

int percentToPower(int percent) {
  return map(percent, 0, 100, LED_MIN, LED_MAX);
}

int calculateLedPower(int currentSecondOfDay) {
  int diffSeconds = min(currentSecondOfDay - ledOnTime, ledOffTime - currentSecondOfDay);
  return max(LED_MIN, min(LED_MAX * diffSeconds / fadePeriod, LED_MAX));
}

void setLedPower(int ledPower) {
  analogWrite(ledPin, ledPower);
}

void ledStatusLoop() {
  Serial.print("LED status: ");
  Serial.print(100 * currentLedPower / LED_MAX);
  Serial.println("%");
}

