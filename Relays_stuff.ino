void setupRelays() {
  //todo: get setup settings from eeprom!
  int relayPin_1 = 16;
  int relayPin_2 = 0;
  
  relays.addRelay(new ScheduledRelay("Lights", relayPin_1, HIGH, ScheduleTime(12, 30), ScheduleTime(22, 0)));
  relays.addRelay(new ScheduledRelay("Pump", relayPin_2, HIGH, ScheduleTime(9, 0), ScheduleTime(22, 30)));
}
