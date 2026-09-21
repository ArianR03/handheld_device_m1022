#ifndef BIBLE_SCREEN_H
#define BIBLE_SCREEN_H

#include <Arduino.h>
#include <vector>

// #include "managers/display.h"
// #include "managers/bible.h"
#include "managers/buttons.h"

class DisplayManager;
class BibleManager;

class BibleScreen {
public:
    void begin(DisplayManager& display, BibleManager& bible);
    void enter(DisplayManager& display);
    bool update(DisplayManager& display, BibleManager& bible, Button button);
    std::vector<String> wrapText(DisplayManager& display, const String& text, int maxWidth);

private:

    std::vector<String> menuItems;
    std::vector<std::vector<String>> versePages;

    int versePage = 0;

    int selectedIndex;
    int currentPage;

    size_t selectedBook = 0;
    uint16_t selectedChapter = 1;   

    void loadBooks(DisplayManager& display, BibleManager& bible);
    void loadChapters(BibleManager& bible);
    void buildVersePages(DisplayManager& display, BibleManager& bible);
    void drawVerses(DisplayManager& display, BibleManager& bible);

    bool updateBooks(DisplayManager& display, BibleManager& bible, Button button);
    bool updateChapters(DisplayManager& display, BibleManager& bible, Button button);
    bool updateVerses(DisplayManager& display, BibleManager& bible, Button button);

    String getHeaderTitle(BibleManager& bible);                     
    void render(DisplayManager& display);
    
    void moveUp(DisplayManager& display, BibleManager& bible);
    void moveDown(DisplayManager& display, BibleManager& bible);

    void select(DisplayManager& display, BibleManager& bible);
    void back(DisplayManager& display, BibleManager& bible);
    
};

#endif
