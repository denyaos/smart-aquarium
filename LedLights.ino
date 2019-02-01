void setupLeds() {
  leds.addSchedulePoint(new ScheduledLedPoint(ScheduleTime(13), 0));
  leds.addSchedulePoint(new ScheduledLedPoint(ScheduleTime(13, 30), 50));
  leds.addSchedulePoint(new ScheduledLedPoint(ScheduleTime(19), 50));
  leds.addSchedulePoint(new ScheduledLedPoint(ScheduleTime(20, 30), 40));
  leds.addSchedulePoint(new ScheduledLedPoint(ScheduleTime(22), 0));
}

void serveLedLights() {
  leds.process(getCurrentTime());
}
