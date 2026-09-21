#include "managers/bible.h"
#include "managers/display.h"
#include "screens/bible_screen.h"

enum class BibleState {
    BOOKS, CHAPTERS, VERSES
};

constexpr int ITEMS_PER_PAGE = 7;
constexpr int CONTENT_X = 10;
constexpr int CONTENT_TOP = 55;
constexpr int CONTENT_WIDTH = 220;
constexpr int CONTENT_BOTTOM = 280; 

BibleState currentState;

void BibleScreen::begin(DisplayManager& display, BibleManager& bible) {
    /*
    Initialize and load bible screen (books).

    Args:
        DisplayManager (object): Load and display UI on ILI9341
        BibleManager (object): Mount LittleFS to retrieve all bible books.
        
    Returns: 
        None

    */    
    // Set current state to BOOKS (origin)
    currentState = BibleState::BOOKS;

    // Set up LittleFS mount and retrieve all bible books.
    if (bible.begin()) {
        Serial.println("Bible Initialized");
        loadBooks(display, bible);
        Serial.printf("Loaded %d books\n", menuItems.size());
    } else {
        Serial.println("Bible initialization failed.");
    }
            

}

void BibleScreen::enter(DisplayManager& display) {
    display.drawPage(&menuItems, selectedIndex, "Bible", currentPage);
}

String BibleScreen::getHeaderTitle(BibleManager& bible) {

    switch (currentState) {
        case BibleState::BOOKS:
            return "Bible Books";
        case BibleState::CHAPTERS:
            return "Book: " + bible.getCurrentBookTitle();
        case BibleState::VERSES:
            return bible.getCurrentBookTitle() + " " + String(selectedChapter);
    }

    return "";
}

void BibleScreen::moveUp(DisplayManager& display, BibleManager& bible) {
    /*
    This method is responsible for paginating between pages of 
    variable `ITEMS_PER_PAGE` moving up.

    Args:
        DisplayManager (object): Used to draw pages or menu rows in move up function.
    
    Returns:
        None
    */
    
    // If user is on the first item, return None.
    if (selectedIndex <= 0) {
        return; 
    }  

    // Save user current spot on selected item and page index.
    int oldSelection = selectedIndex;
    int oldPage = currentPage;

    // Decrement var to move index up one.
    selectedIndex--;

    currentPage = selectedIndex / ITEMS_PER_PAGE;

    if (currentPage != oldPage) {
        display.drawPage(&menuItems, selectedIndex, getHeaderTitle(bible), currentPage);       // Draw new page
    } else {
        display.drawMenuRow(menuItems, oldSelection, false);    // Old selection
        display.drawMenuRow(menuItems, selectedIndex, true);    // New selection
    }
}

void BibleScreen::moveDown(DisplayManager& display, BibleManager& bible) {
    /*
    This method is responsible for paginating between items but
    moving downward.

    Args:
        DisplayManager (object): Used to draw pages and move index down.

    Returns:
        None
    */

    // If current index is at the last element, return None.
    if (selectedIndex >= static_cast<int>(menuItems.size()) - 1) {
        return;
    }

    // Save old spot in var
    int oldSelection = selectedIndex;
    int oldPage = currentPage;

    // Move current index to next element (down)
    selectedIndex++;

    currentPage = selectedIndex / ITEMS_PER_PAGE;

    if (currentPage != oldPage) {
        display.drawPage(&menuItems, selectedIndex, getHeaderTitle(bible), currentPage);    // Draw new page
    } else {
        display.drawMenuRow(menuItems, oldSelection, false);        // Old selection
        display.drawMenuRow(menuItems, selectedIndex, true);        // New selection
    }
}

bool BibleScreen::update(DisplayManager& display, BibleManager& bible, Button button) {

    if (button != Button::NONE) {
        display.clearSelection();
    }    

    switch(currentState) {
        case BibleState::BOOKS:

            display.drawSelectionOption("Back",     0, 295, 59, 25, TFT_WHITE);
            display.drawSelectionOption("Previous", 60, 295, 59, 25, TFT_WHITE);
            display.drawSelectionOption("Next",     120, 295, 59, 25, TFT_WHITE);
            display.drawSelectionOption("Select",   180, 295, 59, 25, TFT_WHITE);

            return updateBooks(display, bible, button);

        case BibleState::CHAPTERS:

            display.drawSelectionOption("Back",     0, 295, 59, 25, TFT_WHITE);
            display.drawSelectionOption("Previous", 60, 295, 59, 25, TFT_WHITE);
            display.drawSelectionOption("Next",     120, 295, 59, 25, TFT_WHITE);
            display.drawSelectionOption("Select",   180, 295, 59, 25, TFT_WHITE);

            return updateChapters(display, bible, button);

        case BibleState::VERSES:

            display.drawSelectionOption("Back",     0, 295, 59, 25, TFT_WHITE);
            display.drawSelectionOption("Previous", 60, 295, 59, 25, TFT_WHITE);
            display.drawSelectionOption("Next",     120, 295, 59, 25, TFT_WHITE);

            if (selectedChapter < bible.getChapterCount()) {
                display.drawSelectionOption("Next CH.", 180, 295, 59, 25, TFT_WHITE);
            }
            
            return updateVerses(display, bible, button);

        default:
            return true;
    }

    return true;
}

void BibleScreen::loadBooks(DisplayManager& display, BibleManager& bible) {

    menuItems.clear();
    for (size_t i=0; i < bible.getBookCount(); i++) {
        menuItems.push_back(bible.getBookTitle(i));
    }
    selectedIndex = 0;
    currentPage = 0;

}

bool BibleScreen::updateBooks(DisplayManager& display, BibleManager& bible, Button button) {

    switch(button) {
        case Button::BTN1:
            // Return back to home page
            return false;
        case Button::BTN3:
            moveUp(display, bible);
            delay(250);
            break;
        case Button::BTN5:
            moveDown(display, bible);
            delay(250);
            break;
        case Button::BTN6:
            selectedBook = selectedIndex;

            Serial.printf("Selected %d book with title %s.\n", selectedBook, bible.getBookTitle(selectedBook));

            if (bible.openBook(selectedBook)) {
                loadChapters(bible);
                currentState = BibleState::CHAPTERS;

                display.drawPage(&menuItems, selectedIndex, getHeaderTitle(bible), currentPage);
                return true;
            }
            break;
        default:
            break; 
    }
    return true;
}

void BibleScreen::loadChapters(BibleManager& bible) {

    menuItems.clear();

    int chapterCount = bible.getChapterCount();

    for (int i = 1; i <= chapterCount; i++) {
        menuItems.push_back("Chapter " + String(i));
    }

    selectedIndex = 0;
    currentPage = 0;
}

bool BibleScreen::updateChapters(DisplayManager& display, BibleManager& bible, Button button) {

    switch(button) {
        case Button::BTN1: 
            loadBooks(display, bible);
            selectedIndex = 0;
            currentPage = 0;
            currentState = BibleState::BOOKS;
            display.drawPage(&menuItems, selectedIndex, getHeaderTitle(bible), currentPage);
            break;
        case Button::BTN3:
            moveUp(display, bible);
            delay(250);
            break;
        case Button::BTN5:
            moveDown(display, bible);
            delay(250);
            break;
        case Button::BTN6:
            selectedChapter = selectedIndex + 1;

            if (bible.loadChapter(selectedChapter)) {
                currentState = BibleState::VERSES;
                currentPage = 0;
                display.clearSelection();
                buildVersePages(display, bible);
                drawVerses(display, bible);
            }
            Serial.printf("Selected number %d chapter in %s.\n", selectedChapter, bible.getCurrentBookTitle());
            break;
        default:
            break;

    }
    return true;
}

std::vector<String> BibleScreen::wrapText(DisplayManager& display, const String& text, int maxWidth) {
    /*
    Wraps the given text to fit within a specified width on the display. This function splits the text into words and constructs lines
    that do not exceed the maximum width, returning a vector of strings representing the wrapped lines.

    Params:
        DisplayManager& display: The display manager used to measure text width.
        const String& text: The text to wrap.
        int maxWidth: The maximum width in pixels for each line.

    Returns:
        std::vector<String>: A vector of strings representing the wrapped lines of text.
    */
    std::vector<String> wrappedLines;
    String currentLine;

    int spaceWidth = display.textWidth(" "); // Width of a space character

    int currentLineWidth = 0;

    int startIndex = 0;
    while (startIndex < text.length()) {
        int endIndex = text.indexOf(' ', startIndex);
        if (endIndex == -1) {
            endIndex = text.length(); // Last word
        }

        String word = text.substring(startIndex, endIndex);
        int wordWidth = display.textWidth(word);

        if (currentLineWidth + wordWidth <= maxWidth) {
            if (!currentLine.isEmpty()) {
                currentLine += " ";
                currentLineWidth += spaceWidth;
            }
            currentLine += word;
            currentLineWidth += wordWidth;
        } else {
            if (!currentLine.isEmpty()) {
                wrappedLines.push_back(currentLine);
            }
            currentLine = word;
            currentLineWidth = wordWidth;
        }

        startIndex = endIndex + 1; // Move past the space
    }

    if (!currentLine.isEmpty()) {
        wrappedLines.push_back(currentLine);
    }

    return wrappedLines;
}

void BibleScreen::buildVersePages(DisplayManager& display, BibleManager& bible) {

    versePages.clear();

    std::vector<String> allLines;
    size_t verseCount = bible.getVerseCount();
    int lineHeight = display.fontHeight() + 4;

    for (size_t i = 0; i < verseCount; i++) {
        const Verse& verse = bible.getVerse(i);
        String fullText = String(verse.number) + " " + verse.text;

        std::vector<String> wrapped = wrapText(display, fullText, CONTENT_WIDTH);
        for (auto& line : wrapped) {
            allLines.push_back(line);
        }
    }

    int linesPerPage = (CONTENT_BOTTOM - CONTENT_TOP) / lineHeight;
    if (linesPerPage < 1) linesPerPage = 1;

    for (size_t i = 0; i < allLines.size(); i += linesPerPage) {
        size_t end = std::min(i + static_cast<size_t>(linesPerPage), allLines.size());
        versePages.emplace_back(allLines.begin() + i, allLines.begin() + end);
    }

    if (versePages.empty()) {
        versePages.push_back({});
    }
}

void BibleScreen::drawVerses(DisplayManager& display, BibleManager& bible) {
    display.clearArea(CONTENT_X, CONTENT_TOP-10, CONTENT_WIDTH, CONTENT_BOTTOM - CONTENT_TOP);

    String header2 = String(currentPage + 1) + "/" + String(versePages.size());
    display.drawHeader(getHeaderTitle(bible), header2);

    int lineHeight = display.fontHeight() + 4;
    int y = CONTENT_TOP;

    for (auto& line : versePages[currentPage]) {
        display.drawString(line, CONTENT_X, y);
        y += lineHeight;
    }
}

bool BibleScreen::updateVerses(DisplayManager& display, BibleManager& bible, Button button) {
    
    switch (button) {
        case Button::BTN1:
            // Return to chapter selection
            loadChapters(bible);
            currentState = BibleState::CHAPTERS;
            display.drawPage(&menuItems, selectedIndex, getHeaderTitle(bible), currentPage);
            delay(250);
            break;
        case Button::BTN3:
            // Previous Page
            if (currentPage > 0) {
                currentPage--;
                drawVerses(display, bible);
            }
            delay(250);
            break;
        case Button::BTN5:
            // Next Page
            if (currentPage < static_cast<int>(versePages.size()) - 1) {
                currentPage++;
                drawVerses(display, bible);
            }
            delay(250);
            break;
        case Button::BTN6:
        {
            // Next Chapter
            uint16_t nextChapter = selectedChapter + 1;
            if (nextChapter <= bible.getChapterCount() && bible.loadChapter(nextChapter)) {
                selectedChapter = nextChapter;
                currentPage = 0;
                buildVersePages(display, bible);
                drawVerses(display, bible);
            }
            delay(250);
            break;
        }
        default:
            break;
    }

    return true;
}