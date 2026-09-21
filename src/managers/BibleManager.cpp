#include "managers/bible.h"

bool BibleManager::begin() {
    /*
    Initialize the Bible Manager by mounting the LittleFS file system and loading the list of Bible books from a specific path.
    This function serves to prepare the BibleManager for use by ensuring that the necessary resources
    are available and that the list of books is successfully loaded into memory.

    Params:
        None

    Returns:
        bool: Returns true if the LittleFS file system mounted and generates list of books.
              Returns false if the LittleFS file system was not mounted and returns error.
    */

    // Initialize the Bible Manager
    if (!LittleFS.begin()) {
        Serial.println("Failed to mount LittleFS.");
        return false;
    }

    // If LittleFS is mounted successfully, load the list of Bible books.
    return loadBibleBooks();
}

bool BibleManager::loadBibleBooks(const char* path) {
    /*
    This function loads the list of Bible books from a specific file path 
    stored in flash using the LittleFS file system. It reads each line from the 
    file and stores the book titles in a vector space for later retrieval.

    Params:
        const char* path: The file path to the text file containing the list of Bible books.

    Returns:
        bool: Retruns true if the books were loaded successfully, false otherwise.
              Returns false if the file could not be opened or if no books were found in the file.

    */
    // Clear out vector of books before loading new ones
    books.clear();

    File file = LittleFS.open(path, "r");

    // If file doesn't exist, return false and print an error msg to serial monitor.
    if (!file) {
        Serial.println("Failed to open books file.");
        return false;
    }
    
    // Load each line from the file into the books vector, trim whitespace and skip empty lines.
    while (file.available()) {
        String bookTitle = file.readStringUntil('\n');
        
        // Remove leading and trailing whitespace from the book title
        bookTitle.trim();
        
        // Add the book title to the vector if it's not empty
        if (!bookTitle.isEmpty()) {
            books.push_back(bookTitle);
        }
    }

    file.close();

    // Log the number of books loaded
    Serial.printf("Loaded %d books.\n", books.size());

    // True if books were loaded successfully, false otherwise
    return !books.empty();
}

size_t BibleManager::getBookCount() const {
    /*
    Returns the number of books retrieved from `loadBibleBooks()` and stored in the `books` vector.

    Params:
        None
    
    Returns:
        size_t: The number of books currently stored in the `books` vector. 
    */
    return books.size();
}

const String& BibleManager::getBookTitle(size_t index) const {
    /*
    Returns the title of the book at the specified index.

    Params:
        size_t index: The index of the book to retrieve.

    Returns:
        const String&: The title of the book at the specified index, or an empty string if the index is out of bounds.
    */

    // Static empty string to return in case of an out-of-bounds index
    static String emptyString;

    // If the index is out-of-bounds, return the empty string; otherwise, return the book title at the specific index.
    if (index >= books.size()) {
        return emptyString; // Return an empty string if index is out of bounds
    }
    return books[index];
}

String BibleManager::makeBookPath(const String& bookTitle) const {
    /*
    Returns the path to a book's directory based on its title. This 
    function constructs the path by appending the book title to a base path.

    Params:
        const String& bookTitle: The title of the book for which to generate the path.
    Returns:
        String: The constructed path to the books directory.
    */
    return "/bible/bible-niv/" + bookTitle + ".txt";
}

bool BibleManager::openBook(size_t index) {
    /*
    Opens a book by its index and prepares for chapter selection. This function sets
    the current book title and path, scans for the number of chapters in the book, and initalizes
    the chapter count and current chapter number.

    Params:
        size_t index: The index of the book to open.

    Returns:
        bool: Returns true if the book was opened successfully, false otherwise.
              Returns false if the index is out of bounds or if no chapters were found for the book.
    */

    // Check if the index is valid; if not, print an error message and return false.
    if (index >= books.size()) {
        Serial.println("Invalid book index.");
        return false;
    }
    
    // Retrieve the current book title and generate the path to the books directory.
    currentBookTitle = books[index];
    currentBookPath = makeBookPath(currentBookTitle);

    // Scan the book directory to determine the number of chapters available and store in `chapterCount`.
    chapterCount = scanChapterCount(currentBookPath.c_str());

    currentChapterNumber = 1; // Start with the first chapter
    currentVerses.clear(); // Clear any previously loaded verses

    // If no chapters were found, print an error msg and return false; otherwise, print the book title and chapter count and return true.
    if (chapterCount <= 0) {
        Serial.printf("No chapters found for book: %s\n", currentBookTitle.c_str());
        return false;
    }

    Serial.printf("Opened book: %s with %d chapters.\n", currentBookTitle.c_str(), chapterCount);
    return true;
}

int BibleManager::scanChapterCount(const char* path) {
    /*
    Scans the book directory to determine the number of chapters available. This function reads the book file line by line,
    extracts the chapter numbers, and keeps track of the highest chapter number found.

    Params:
        const char* path: The path to the book file to scan for chapters.

    Returns:
        int: The number of chapters found in the book file, or -1 if the file count not be opened.
    */

    File file = LittleFS.open(path, "r");

    if (!file) {
        Serial.printf("Failed to open book file: %s\n", path);
        return -1; // Indicate an error
    }

    file.readStringUntil('\n'); // Skip the first line (book title)
    
    int maxChapter = 0;

    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim(); 

        if (line.isEmpty()) {
            continue; // Skip empty lines
        }

        int firstPipe = line.indexOf('|');

        if (firstPipe < 0) {
            continue;
        }

        int chapter = line.substring(0, firstPipe).toInt();

        if (chapter > maxChapter) {
            maxChapter = chapter;
        }
    }
    file.close();
    return maxChapter;
}

int BibleManager::getChapterCount() const {
    // Retrieve the number of chapters in the currently opened book.
    return chapterCount;
}

bool BibleManager::loadChapter(uint16_t chapterNumber) {
    /*
    Loads a specific chapter of the currently opened book and prepares for verse retrieval. This function reads the book file line by line,
    extracts the verses for the specified chapter, and stores them in the `currentVerses` vector.

    Params:
        uint16_t chapterNumber: The number of the chapter to load.

    Returns:
        bool: Returns true if the chapter was loaded successfully, false otherwise.
              Returns false if the chapter number is invalid or if no verses were found for the chapter.
    */
    if (chapterNumber < 1 || chapterNumber > chapterCount) {
        Serial.printf("Invalid chapter number: %d. Must be between 1 and %d.\n", chapterNumber, chapterCount); 
        return false;
    }

    currentVerses.clear(); // Clear any previously loaded verses

    File file = LittleFS.open(currentBookPath, "r");

    if (!file) {
        Serial.printf("Failed to open book file: %s\n", currentBookPath.c_str());
        return false;
    }

    file.readStringUntil('\n'); // Skip header line

    while (file.available()) {
        String line = file.readStringUntil('\n');

        if (line.isEmpty()) {
            continue;
        }

        int firstPipe = line.indexOf('|');
        int secondPipe = line.indexOf('|', firstPipe + 1);

        if (firstPipe < 0 || secondPipe < 0) {
            continue; // Skip malformed lines
        }

        uint16_t lineChapter = line.substring(0, firstPipe).toInt();

        if (lineChapter < chapterNumber) {
            continue; // Skip verses from previous chapters
        } else if (lineChapter > chapterNumber) {
            break; // Stop reading if we've passed the desired chapter
        }

        uint16_t verseNumber = line.substring(firstPipe + 1, secondPipe).toInt();
        String verseText = line.substring(secondPipe + 1);

        verseText.trim(); // Remove leading/trailing whitespace

        currentVerses.push_back({verseNumber, verseText});
    }

    file.close();

    if (currentVerses.empty()) {
        Serial.printf("No verses found for chapter %d in book: %s\n", chapterNumber, currentBookTitle.c_str());
        return false;
    }

    currentChapterNumber = chapterNumber;

    Serial.printf("Loaded chapter %d with %d verses.\n", chapterNumber, currentVerses.size());
    return true;
}

size_t BibleManager::getVerseCount() const {
    // Retrieve the number of verses in the currently loaded chapter.
    return currentVerses.size();
}

const Verse& BibleManager::getVerse(size_t index) const {
    /*
    Returns the verse at the specified index in the currently loaded chapter.

    Params:
        size_t index: The index of the verse to retrieve.

    Returns:
        const Verse&: The verse at the specified index, or an empty verse if the index is out of bounds.
    */
    static Verse emptyVerse = {0, ""};

    if (index >= currentVerses.size()) {
        return emptyVerse; // Return an empty verse if index is out of bounds
    }
    return currentVerses[index];
}

const String& BibleManager::getCurrentBookTitle() const {
    // Retrieve the title of the currently opened book.
    return currentBookTitle;
}

uint16_t BibleManager::getCurrentChapterNumber() const {
    // Retrieve the number of the chapter currently loaded.
    return currentChapterNumber;
}

std::vector<String> BibleManager::getDailyVerse(int dayIndex) {

    std::vector<String> verse;

    if (!LittleFS.begin()) {
        Serial.println("Failed to mount LittleFS.");
        return verse;
    }

    File file = LittleFS.open("/daily_verse.txt", "r");

    if (!file) {
        Serial.println("Failed to open daily verse file.");
        return verse;
    }

    int currentIndex = 0;

    while (file.available()) {
        
        String line = file.readStringUntil('\n');
        line.trim();

        if (currentIndex == dayIndex) {
            int seperator = line.indexOf('|');

            if (seperator != -1) {
                verse.push_back(line.substring(0, seperator));
                verse.push_back(line.substring(seperator+1));
            }
            break;
        }
        currentIndex++;
    }
    file.close();

    return verse;
}