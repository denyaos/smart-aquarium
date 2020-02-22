const int feedBtnPin = 14; // D5 = GPIO
unsigned long second = 1000; 

int shortLongThreshold = 300;

void setupButtons() {
  pinMode(feedBtnPin, INPUT);
}

void buttonLoop() {
  serveFeedButton();  
}


void serveFeedButton() { 

  static unsigned long btnPressedAt = 0;
  if (digitalRead(feedBtnPin) == HIGH) {
    if (btnPressedAt == 0) {
      btnPressedAt = millis();  
    }
  } else {
    if (btnPressedAt > 0) {
      
      if ((millis() - btnPressedAt) < shortLongThreshold) {
        btnShortPress();
      } else {
        btnLongPress();
      }

      btnPressedAt = 0;
    }
  }
}


void btnShortPress() {
  toggleFeedingMode();
}

void btnLongPress() {
  requestTimeSync();
}
