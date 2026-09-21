#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include "managers/display.h"
#include "managers/rtc.h"

class HomeScreen {
public:
    // General update: rtc -> daily verse -> options 
    void update(DisplayManager& display, BibleManager& bible, BibleScreen& bibleScreen, RTCManager& rtc);
};

#endif
