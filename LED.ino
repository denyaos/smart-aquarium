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
  calculateLedPower(currentSecondOfDay);
  setLedPower();
}

int percentToPower(int percent) {
  return map(percent, 0, 100, LED_MIN, LED_MAX);
}

int calculateLedPower(int currentSecondOfDay) {
  int diffSeconds = min(max(0, currentSecondOfDay - ledOnTime), max(0, ledOffTime - currentSecondOfDay));
  currentLedPower =  min(LED_MAX * diffSeconds / fadePeriod, LED_MAX);
  if (freezeUntil > millis()) {
    currentLedPower = max(percentToPower(ledFeedLevelInPercent), currentLedPower);
  }
}

void setLedPower() {
  static int power = 0;
  currentLedPower > power ? power++ : power--; 
  analogWrite(ledPin, min(power, percentToPower(ledMaxRateInPercent)));
}

void ledStatusLoop() {
  Serial.print("LED status: ");
  Serial.print(100 * currentLedPower / LED_MAX);
  Serial.println("%");
}

