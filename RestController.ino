//bool debug_rest = true;
//
//void log_rest(String message1, String message2) {
//  if  (debug_rest) {
//    Serial.print("[RestController]: ");
//    Serial.print(message1);
//    Serial.print(" ");
//    Serial.println(message2);
//  }
//}
//
//String handleRequest(String request) {
//  String RELAY = "/relay";
//  String FEED = "/feed";
//  String LED = "/led";
//  
//  String method = request.substring(0, request.indexOf(" "));
//  log_rest("Request method:", method);
//  request = request.substring(method.length() + 1);
//  
//  String responseMessage;
//  if (request.startsWith(RELAY)) {
//    log_rest("Relay req.", request);
//    responseMessage = handleRelayRequest(request.substring(RELAY.length()));
//  } else if (request.startsWith(LED)) {
//    log_rest("Led req.", request);
//    responseMessage = handleLedRequest(request.substring(LED.length()));
//  } else if (request.startsWith(FEED)) {
//    log_rest("Feed req.", request);
//    responseMessage = handleFeedRequest(request.substring(FEED.length()));
//  } else {
//    responseMessage = "Malformed request, none roots.";
//    log_rest(responseMessage, request);
//  }
//  log_rest(responseMessage, "Client disonnected.");
//  return makeResponse(responseMessage);
//}
//
//String handleRelayRequest(String request) {
//  log_rest("Relay request.", request);
//  int x;
//
//  int relayIndex;
//  
//  if (request.startsWith("/1")) {
//    relayIndex = 0;
//    x = 2;
//  } else if (request.startsWith("/2")) {
//    relayIndex = 1;
//    x = 2;
//  } else {
//    log_rest("Malformed relay req.", request);
//    return "Malformed relay request";
//  }
//
//  request = request.substring(x);
//
//  if (request.startsWith("/unfreeze")) {
//    relays.unfreezeNow(relayIndex);
//    return "Relay #" + String(relayIndex) + " unfreezed.";
//  } else {
//    
//    bool targetState;
//    if (request.startsWith("/on")) {
//      targetState = true;
//      x = String("/on").length();
//    } else if (request.startsWith("/off")) {
//      targetState = false;
//      x = String("/off").length();
//    } else {
//      log_rest("Malformed relay req.", request);
//      return "Malformed relay request";
//    }
//
//    request = request.substring(x);
//
//    int minutes = request.substring(1).toInt();
//    relays.switchForMinutes(relayIndex, targetState, minutes);
//    return String("Relay #") + (relayIndex+1) + String(" switched ") + (targetState ? String("ON") : String("OFF")) + String(" for ") + minutes + String(" minute(s).");
//  }
//}
//
//String handleLedRequest(String request) {
//
//  log_rest("Led request.", request);
//  int x;
//  String freeze = "/freeze";
//  String stats = "/status";
//  String unfreeze = "/unfreeze";
//
//
//  if (request.startsWith(freeze)) {
//    request = request.substring(freeze.length()+1);
//    Serial.println(request);
//    String targetLevel = request.substring(0, request.indexOf("/"));
//    Serial.println(targetLevel);
//    request = request.substring(targetLevel.length());
//    Serial.println(request);
//    int mins = request.substring(1).toInt();
//    Serial.println(mins);
//    leds.freezeLedLevel(targetLevel.toInt(), mins);
//    return leds.getFreezedStatus();
//    
//  } else if (request.startsWith(unfreeze)) {
//    leds.unfreeze();
//    return String("LEDs unfreezed.");
//    
//  } else if (request.startsWith(stats)) {
//    int currentLevel = leds.getLedLevel();
//    log_rest("curr level: ", String(currentLevel));
//    String freezedStatus = leds.getFreezedStatus();
//    log_rest("freezed status: ", freezedStatus);
//    return String("Leds level is ") + currentLevel + String("%. ") + freezedStatus;
//    
//  } else {
//    log_rest("Malformed led req.", request);
//    return "Malformed led request.";
//  }
//  
//}
//
//String handleFeedRequest(String request) {
//  log_rest("Feed request.", request);
//  toggleFeedingMode();
//  return "Feed mode toggled by rest.";
//}
//
//String makeResponse(String message) {
//   // Prepare the response
//    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
//    s += message; 
//    return s;
//}
//

