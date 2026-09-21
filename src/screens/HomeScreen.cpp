#include <managers/display.h>
#include <screens/home_screen.h>


void HomeScreen::update(DisplayManager& display, BibleManager& bible, BibleScreen& bibleScreen, RTCManager& rtc) {

    display.begin();
    display.clearScreen();

    String timePeriod = rtc.retrieveTimePeriod();
    String date = rtc.retrieveCurrDate();

    if (rtc.isBirthday()) {
        timePeriod = "Happy Birthday";
    }

    String combined = timePeriod + " Mireya ";

    display.drawHeader(combined, date);

    std::vector<String> dailyVerse = bible.getDailyVerse(rtc.retrieveDayOfYear()-1);

    // Daily Bible Verse (0-365 days)
    display.drawVerseCanvas(display, bibleScreen, dailyVerse[0], dailyVerse[1]);

    display.drawSelectionOption("HOME", 0, 295, 59, 25, TFT_WHITE);
    display.drawSelectionOption("BIBLE", 60, 295, 59, 25, TFT_WHITE);
    display.drawSelectionOption("MUSIC", 120, 295, 59, 25, TFT_WHITE);
    display.drawSelectionOption("PAINT", 180, 295, 59, 25, TFT_WHITE);

}