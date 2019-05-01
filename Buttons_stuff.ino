const int feedBtnPin = 14;

void setupButtons() {
  pinMode(feedBtnPin, INPUT);
}

void serveButtons() {
  serveFeedButton();  
}

unsigned long second = 1000; 

void serveFeedButton() { 
  static bool btnWasPressed = false;
  static unsigned long dontReadHighUntil = millis();
  if (digitalRead(feedBtnPin) == HIGH && !btnWasPressed && millis() > dontReadHighUntil) {
    btnWasPressed = true;
    dontReadHighUntil = millis() + second;
  } else {
    if (btnWasPressed){
      toggleFeedingMode();
      btnWasPressed = false;
    }
  }
}
