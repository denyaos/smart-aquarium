const int feedBtnPin = 14;

void setupButtons() {
  pinMode(feedBtnPin, INPUT);
}

void serveButtons() {
  serveFeedButton();  
}

unsigned long debounceDelay = 50;
unsigned long feedBtnPressTime = 0;
bool feedBtnWasPressed = false;  
bool feedBtnPressHappenned = false;  

void serveFeedButton() {
  int read = digitalRead(feedBtnPin);
  
  if (read == HIGH) {
    if (feedBtnPressTime == 0) {
      feedBtnPressTime = millis();  
    } else if ((millis() - feedBtnPressTime) > debounceDelay) {
      feedBtnPressHappenned = true;
    }
  } else {
    feedBtnPressTime = 0;
    if (feedBtnPressHappenned) {
      toggleFeedingMode();
      feedBtnPressHappenned = false;
    }
  }
}
