#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <vector>

#include "stdint.h"
#include "managers/bible.h"
#include "managers/display.h"
#include "managers/rtc.h"
#include "screens/bible_screen.h"

class DisplayManager {
public:
    /// Initializes the display and prepares it for use. This function sets up the TFT display, initializes the RTC, and clears the screen to a default state.
    void begin();
    void clearSelection();
    void clearScreen();
    void clearArea(int x, int y, int w, int h);

    // Generic UI
    void drawHeader();        // Produces bar at the top with morning/afternoon/evening/night and date.
    void drawHeader(const String& textHeader1, const String& textHeader2);
    void drawVerseCanvas(DisplayManager& display, BibleScreen& bible, const String& verseRef, const String& verseText);
    void drawSelectionOption(const char* option, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    
    // Word Wrappping
    int textWidth(const String& text);
    int fontHeight();
    void drawString(const String& text, uint16_t x, uint16_t y);

    // Generic paginated menu
    void drawPage(const std::vector<String>* items, int selectedIndex, String title, int currentPage);
    void drawMenuRow(const std::vector<String>& items, int recordIndex, bool selected);
    
    TFT_eSPI tft = TFT_eSPI(); // Create an instance of the TFT_eSPI class
};


#endif