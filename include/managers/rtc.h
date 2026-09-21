#ifndef RTC_H
#define RTC_H 

#include <Arduino.h>
#include "RTClib.h"

class RTCManager {
public:

    void initRTC();                // Initialize RTC board
    void syncWithCompiler();
    DateTime getNow();
    
    String retrieveTimePeriod();   // Morning, Afternoon, Evening, Night
    String retrieveCurrTime();
    String retrieveCurrDate();     // MM/DD/YYYY (f-string)
    int retrieveDayOfYear();
    
    // Special birthday events
    bool isBirthday();              // 10.22
    void checkBirthdaySong();        

private:
    RTC_PCF8523 rtc;
};

#endif