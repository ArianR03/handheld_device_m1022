#ifndef BIBLE_H
#define BIBLE_H

#include "display.h"

#include <Arduino.h>
#include <LittleFS.h>
#include <vector>
#include <string>

struct Verse {
    uint16_t number;
    String text;
};

class BibleManager {
public:

    // Start Bible if selected
    bool begin();

    bool loadBibleBooks(const char* path = "/bible/books.txt");     // Retrieve all bible books from the file system and store in a vector.
    size_t getBookCount() const;                                    // Retrieve count of all books in the bible.
    const String& getBookTitle(size_t index) const;                 // Retrieve the title of a book at a given index.

    bool openBook(size_t index);                                    // Open a book by its index and prepare for chapter selection.
    int getChapterCount() const;                                    // Retrieve the number of chapters in the currently opened book.

    bool loadChapter(uint16_t chapterNumber);                       // Load a specific chapter of the currently opened book and prepare for verse retrieval.
    size_t getVerseCount() const;                                   // Retrieve the number of verses in the currently loaded chapter.
    const Verse& getVerse(size_t index) const;                      // Retrieve a specific verse by its index in the currently loaded chapter.

    const String& getCurrentBookTitle() const;                      // Retrieve the title of the currently opened book.
    uint16_t getCurrentChapterNumber() const;                       // Retrieve the number of the chapter currently loaded.

    std::vector<String> getDailyVerse(int dayIndex);

private: 
    std::vector<String> books;                                      // Store all book
    std::vector<Verse> currentVerses;                               // Store verses for the currently loaded chapter

    String currentBookPath;                                         // Store the path of the currently opened book
    String currentBookTitle;                                        // Store the title of the currently opened book

    int chapterCount = 0;                                                                       // Store the number of chapters in the currently opened book
    uint16_t currentChapterNumber = 0;                                                          // Store the number of the chapter currently loaded

    String makeBookPath(const String& bookTitle) const;                                         // Generate the path to a book's directory based on its title.
    int scanChapterCount(const char* path);                                                     // Scan the book directory to determine the number of chapters available.
};

#endif