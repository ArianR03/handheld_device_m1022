#include <Arduino.h>
#include <managers/display.h>

namespace {

    constexpr int SCREEN_WIDTH = 240;
    constexpr int SCREEN_HEIGHT = 320;

    constexpr int HEADER_HEIGHT = 40;
    constexpr int FOOTER_HEIGHT = 25;

    constexpr int MENU_X = 10;
    constexpr int MENU_Y = HEADER_HEIGHT + 5;
    constexpr int MENU_WIDTH = SCREEN_WIDTH - 20;

    constexpr int ROW_HEIGHT = 32;

    constexpr int ITEMS_PER_PAGE = 7;

};

void DisplayManager::begin() {
    tft.init();

    // Calibrate touchscreen with pre-calibrated values
    uint16_t calData[5] = { 271, 3443, 456, 3429, 4 };
    tft.setTouch(calData);

    tft.fillScreen(TFT_BLACK);
}

void DisplayManager::clearScreen() {
    tft.fillScreen(TFT_BLACK);
}

void DisplayManager::clearSelection() {
    tft.fillRect(0, 280, SCREEN_WIDTH, 40, TFT_BLACK);
}

void DisplayManager::clearArea(int x, int y, int w, int h) {
    tft.fillRect(x, y, w+10, h, TFT_BLACK);
}

void DisplayManager::drawVerseCanvas(DisplayManager& display, BibleScreen& bible, const String& verseRef, const String& verseText) {

    uint16_t verseX = 5;
    uint16_t verseY = 50;
    uint16_t verseW = 240 - (2 * verseX);
    uint16_t verseH = 235; 

    // Draw canvas border for verse
    tft.drawRect(verseX, verseY, verseW, verseH, TFT_WHITE);

    // Draw "Daily Verse" header
    uint16_t headerWidth = tft.textWidth("Daily Verse");
    uint16_t headerHeight = tft.fontHeight();

    int centerHeaderX = 7.5 + ((verseW - headerWidth) / 2);
    int centerHeaderY = 65;

    tft.drawString("Daily Verse", centerHeaderX, centerHeaderY);

    // Draw daily verse w/ proper formatting
    std::vector<String> wrapped = bible.wrapText(display, verseText, verseW-20);
    wrapped.push_back(" ");
    wrapped.push_back(verseRef);

    int lineHeight = tft.fontHeight() + 3;
    int y = 90;

    for (auto& line : wrapped) {
        
        uint16_t verseWidth = tft.textWidth(line);
        uint16_t verseHeight = tft.fontHeight();

        int centerVerseLine = 7.5 + ((verseW - verseWidth) / 2);
        
        tft.drawString(line, centerVerseLine, y);
        y += lineHeight;
    }
}
  
void DisplayManager::drawHeader(const String& textHeader1, const String& textHeader2) {
    
    tft.fillRect(1, 1, SCREEN_WIDTH - 2, HEADER_HEIGHT - 2, TFT_BLACK);
    
    tft.drawRect(0,0,165,40, TFT_WHITE);    // NOTE: Good Morning/Afternoon/Night comment
    tft.drawRect(168, 0, 67, 40, TFT_WHITE); // NOTE: Date Time or Page Num
    
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);

    uint16_t textW = tft.textWidth(textHeader1);
    uint16_t textH = tft.fontHeight();

    int centerX = 0 + ((177 - textW) / 2);
    int centerY = 0 + ((40 - textH) / 2);

    tft.drawString(textHeader1, centerX, centerY);

    textW = tft.textWidth(textHeader2);
    textH = tft.fontHeight();

    centerX = 168 + ((67 - textW) / 2);
    centerY = 0 + ((40 - textH) / 2);

    tft.drawString(textHeader2, centerX, centerY);
}

void DisplayManager::drawPage(const std::vector<String>* items, int selectedIndex, String title, int currentPage) {
    
    int totalPages = 0;

    if (items != nullptr && !items->empty()) {
        totalPages = (items->size() + ITEMS_PER_PAGE -1) / ITEMS_PER_PAGE;
    }

    String headerPage = String(currentPage + 1) + "/" + String(totalPages);

    drawHeader(title, headerPage);
    
    tft.fillRect(0, MENU_Y, SCREEN_WIDTH, ITEMS_PER_PAGE * ROW_HEIGHT, TFT_BLACK);
    
    int firstRecord = currentPage * ITEMS_PER_PAGE;
    int lastRecord = firstRecord + ITEMS_PER_PAGE;

    if (lastRecord > static_cast<int>(items->size())) {
        lastRecord = items->size();
    }
    
    for (int i = firstRecord; i < lastRecord; i++) {
        drawMenuRow(*items, i, i == selectedIndex);
    }
}

void DisplayManager::drawMenuRow(const std::vector<String>& items, int recordIndex, bool selected) {

    int rowOnPage = recordIndex % ITEMS_PER_PAGE;

    int rowY = MENU_Y + rowOnPage * ROW_HEIGHT;

    uint16_t backgroundColor = selected ? TFT_BLUE : TFT_BLACK;

    tft.fillRect(MENU_X, rowY, MENU_WIDTH, ROW_HEIGHT-2, backgroundColor);

    tft.drawRect(MENU_X, rowY, MENU_WIDTH, ROW_HEIGHT-2, TFT_DARKGREY);
    tft.setTextColor(TFT_WHITE, backgroundColor);

    uint16_t textW = tft.textWidth(items[recordIndex]);
    uint16_t textH = tft.fontHeight();

    int centerX = MENU_X + ((MENU_WIDTH - textW) / 2);
    int centerY = rowY + ((ROW_HEIGHT - textH) / 2);

    tft.drawString(items[recordIndex], centerX, centerY);

}

void DisplayManager::drawSelectionOption(const char* option, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    /*
     * This method is to draw the menu selections that the user
     * will be able to choose from. This also allows for an ease
     * of adding different options when navigating between screens.
     * 
     * NOTE: This method is recursively called, one at a time.
     * 
     * Params:
     *  option (const char*): Label of the option
     *  x (signed int): Start x position of option.
     *  y (signed int): Start y position of option.
     *  w (signed int): Width of box
     *  h (signed int): Height of box
     *  color (unsigned int): Color text/box border.
     * 
     * Output Display: [Home, Bible, MP3, Paint]
     */

    tft.drawRect(x,y,w,h,color);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);

    uint16_t textW = tft.textWidth(option);
    uint16_t textH = tft.fontHeight();

    int centerX = x + ((w - textW) / 2);
    int centerY = y + ((h - textH) / 2);
    
    tft.drawString(option, centerX, centerY);
}

int DisplayManager::textWidth(const String& text) {
    return tft.textWidth(text);
}

int DisplayManager::fontHeight() {
    return tft.fontHeight();
}

void DisplayManager::drawString(const String& text, uint16_t x, uint16_t y) {
    tft.drawString(text, x, y);
}