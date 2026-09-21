#include <Arduino.h>
#include <TFT_eSPI.h>

#include "managers/display.h"
#include "managers/buttons.h"
#include "managers/bible.h"
#include "managers/music.h"
#include "managers/rtc.h"
#include "managers/paint.h"

#include "screens/home_screen.h"
#include "screens/bible_screen.h"
#include "screens/music_screen.h"
#include "screens/paint_screen.h"

DisplayManager display;
ButtonManager buttons;
BibleManager bible;
MusicManager music;
PaintManager paint;
RTCManager rtc;

HomeScreen homeScreen;
BibleScreen bibleScreen;
MusicScreen musicScreen;
PaintScreen paintScreen;

enum class AppState {
    HOME, BIBLE, MP3, PAINT
};

AppState appState = AppState::HOME;

void setup() {

    Serial.begin(115200);
    delay(2000);

    rtc.initRTC();

    music.begin();

    homeScreen.update(display, bible, bibleScreen, rtc);
    delay(50);

    // Check if today is birthday
    rtc.checkBirthdaySong();
    
}

void loop()
{      

    buttons.update();
    Button button = buttons.getButton();
    
    switch(appState) {
        case AppState::HOME:

            if (button == Button::BTN3) {
                appState = AppState::BIBLE;
                display.clearScreen();
                bibleScreen.begin(display, bible); 
                bibleScreen.enter(display);
                Serial.println("Bible selected.");
            }

            if (button == Button::BTN5) {
                appState = AppState::MP3;
                display.clearScreen();
                musicScreen.begin(display, music, rtc);
                Serial.println("MP3 selected.");
            }

            if (button == Button::BTN6) {
                // Paint
                appState = AppState::PAINT;
                display.clearScreen();
                paintScreen.begin(display, paint);
                Serial.println("Paint selected.");
            }

            break;

        case AppState::BIBLE:
            
            if (!bibleScreen.update(display, bible, button)) {
                Serial.println("Returning HOME.");
                appState = AppState::HOME;
                homeScreen.update(display, bible, bibleScreen, rtc);
            }
            break;
        
        case AppState::MP3:
            
            if (!musicScreen.update(display, music, button)) {
                Serial.println("Returning HOME.");
                appState = AppState::HOME;
                homeScreen.update(display, bible, bibleScreen, rtc);
            }
            break;
            
        case AppState::PAINT:
            if (!paintScreen.update(display, paint, button)) {
                Serial.println("Returning HOME.");
                appState = AppState::HOME;
                homeScreen.update(display, bible, bibleScreen, rtc);
            }

            break;
        } 
}
