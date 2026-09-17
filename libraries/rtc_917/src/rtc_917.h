#pragma once

#include <Arduino.h>

#ifdef RTC
#undef RTC
#endif

class RTCClass {
public:
  RTCClass();
  bool begin();
  void end();

  bool set(uint16_t year,
              uint8_t month,
              uint8_t day,
              uint8_t day_of_week,
              uint8_t hour,
              uint8_t minute,
              uint8_t second);
  int64_t getUnixTime();
  bool setUnixTime(int64_t unix_time);

  uint8_t getSecond();
  uint8_t getMinute();
  uint8_t getHour();
  uint8_t getDay();
  String getDayName();
  uint8_t getMonth();
  String getMonthName();
  uint16_t getYear();

private:
  const struct device *rtc_device;

};

extern RTCClass RTC;
