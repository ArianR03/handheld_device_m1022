#include "screens/music_screen.h"

void MusicScreen::begin(DisplayManager& display, MusicManager& music, RTCManager& rtc) {
    
    display.clearScreen();

    // Display default header
    String timePeriod = rtc.retrieveTimePeriod();
    String date = rtc.retrieveCurrDate();

    if (rtc.isBirthday()) {
        timePeriod = "Happy Birthday";
    }
    
    String combined = timePeriod + " Mireya ";

    display.drawHeader(combined, date);

    display.drawSelectionOption("BACK", 0, 295, 59, 25, TFT_WHITE);
    display.drawSelectionOption("PREVIOUS", 60, 295, 59, 25, TFT_WHITE);
    updatePlayState(display, music);
    display.drawSelectionOption("NEXT", 180, 295, 59, 25, TFT_WHITE);

    drawCanvas(display);

    updateSongInfo(display, music);
    updateProgress(display, music);
    updateShuffle(display, music);
    updateVolume(display, music);

}

bool MusicScreen::update(DisplayManager& display, MusicManager& music, Button button) {

    handleShuffleTouch(display, music);

    switch (button) {
        case Button::BTN1:
            return false;
        case Button::BTN2:
            music.volumeUp();
            updateVolume(display, music);
            break;
        case Button::BTN3:
            music.previous();
            updateSongInfo(display, music);
            updateProgress(display, music);
            break;
        case Button::BTN4:
            music.volumeDown();
            updateVolume(display, music);
            break;
        case Button::BTN5:
            music.togglePlayPause();
            updatePlayState(display, music);
            break;
        case Button::BTN6:
            music.next();
            updateSongInfo(display, music);
            updateProgress(display, music);
            break;
    }

    if (music.isPlaying()) {
        updateProgress(display, music);
    }

    return true;
}

void MusicScreen::drawCanvas(DisplayManager& display) {

    uint16_t musicX = 5;
    uint16_t musicY = 50;
    uint16_t musicW = 240 - (2*musicX);
    uint16_t musicH = 235;

    display.tft.drawRect(musicX, musicY, musicW, musicH, TFT_WHITE);

    uint16_t headerWidth = display.tft.textWidth("Now Playing");
    uint16_t headerHeight = display.tft.fontHeight();

    int centerHeaderX = 5 + ((musicW - headerWidth) / 2);
    int centerHeaderY = 65;

    display.tft.drawString("Now Playing", centerHeaderX, centerHeaderY, 2);

}    

void MusicScreen::updateSongInfo(DisplayManager& display, MusicManager& music) {
    // Song title box
    uint16_t boxX = 20;
    uint16_t boxY = 100;
    uint16_t boxW = 200;
    uint16_t boxH = 30;

    // Clear and redraw box
    display.tft.fillRect(boxX, boxY, boxW, boxH, TFT_BLACK);
    display.tft.drawRect(boxX, boxY, boxW, boxH, TFT_WHITE);

    // Get song name
    String originalName = music.getSongName();
    String songName = originalName;

    uint16_t maxTextWidth = boxW - 10;

    // Make room for "..."
    while (display.tft.textWidth(songName + "...") > maxTextWidth) {
        if (songName.length() == 0) {
            break;
        }

        songName.remove(songName.length() - 1);
    }

    if (songName != originalName) {
        songName += "...";
    }

    uint16_t textWidth = display.tft.textWidth(songName);
    uint16_t textHeight = display.tft.fontHeight();

    int textX = boxX + (boxW - textWidth) / 2;
    int textY = boxY + (boxH - textHeight) / 2;

    display.tft.drawString(songName, textX, textY);
}

void MusicScreen::updateProgress(DisplayManager& display, MusicManager& music) {
    
    uint16_t currentTime = music.getCurrentTime();
    uint16_t totalTime = music.getTotalTime();
    
    // Convert urrent time to ss -> mm:ss
    uint16_t currentMinutes = currentTime / 60;
    uint16_t currentSeconds = currentTime % 60;

    // Convert total time to ss -> mm:ss
    uint16_t totMinutes = totalTime / 60;
    uint16_t totSeconds = totalTime % 60;

    char timeStamp[13];

    sprintf(
        timeStamp,
        "%02d:%02d / %02d:%02D",
        currentMinutes,
        currentSeconds,
        totMinutes,
        totSeconds
    );

    // Progress Bar
    uint16_t barX = 20;
    uint16_t barY = 150;
    uint16_t barW = 200;
    uint16_t barH = 10;

    // Clear old progress
    display.tft.fillRect(barX, barY, barW, barH, TFT_BLACK);

    // draw new progress outline
    display.tft.drawRect(barX, barY, barW, barH, TFT_WHITE);

    if (totalTime == 0) {
        return;
    }

    // Calculate bar fill
    uint16_t progress = ((uint32_t)currentTime * (barW - 4)) / totalTime;

    if (progress > 0) {
        display.tft.fillRect(
            barX + 2, 
            barY + 2,
            progress,
            barH - 4,
            TFT_WHITE
        );
    }

    uint16_t timeStampWidth = display.textWidth(timeStamp);
    uint16_t timeStampHeigt = display.tft.fontHeight();

    int centerTimeStampX = barX + (barW - timeStampWidth) / 2;
    int centerTimeStampY = 170;

    display.tft.fillRect(centerTimeStampX, centerTimeStampY, timeStampWidth, timeStampHeigt, TFT_BLACK);
    display.tft.drawString(timeStamp, centerTimeStampX, centerTimeStampY);

}

void MusicScreen::updatePlayState(DisplayManager& display, MusicManager& music) {
    display.tft.fillRect(120, 295, 59, 25, TFT_BLACK);
    
    if (music.isPlaying()) {
        display.drawSelectionOption("PAUSE", 120, 295, 59, 25, TFT_WHITE);
    } else {
        display.drawSelectionOption("PLAY", 120, 295, 59, 25, TFT_WHITE);
    }
}

bool MusicScreen::handleShuffleTouch(DisplayManager& display, MusicManager& music) {
    
    uint16_t x, y;
    
    if (!display.tft.getTouch(&x, &y)) {
        return false;
    }

    uint16_t shuffleX = 35;
    uint16_t shuffleY = 225;
    uint16_t shuffleW = 170;
    uint16_t shuffleH = 30;

    if (x >= shuffleX && x <= shuffleX + shuffleW
        && y >= shuffleY && y <= shuffleY + shuffleH) {
            music.toggleShuffle();
            Serial.print("SHUFFLE: ");
            Serial.println(music.isShuffleEnabled() ? "ON" : "OFF");

            updateShuffle(display, music);
            delay(1000);
            return true;
        }
    return false;
}

void MusicScreen::updateShuffle(DisplayManager& display, MusicManager& music) {

    String shuffleText = music.isShuffleEnabled() ? "SHUFFLE: ON" : "SHUFFLE: OFF"; 

    uint16_t shuffleX = 35;
    uint16_t shuffleY = 225;
    uint16_t shuffleW = 170;
    uint16_t shuffleH = 30;

    display.tft.fillRect(shuffleX, shuffleY, shuffleW, shuffleH, TFT_BLACK);
    display.tft.drawRect(shuffleX, shuffleY, shuffleW, shuffleH, TFT_WHITE);

    uint16_t shuffleTextWidth = display.textWidth(shuffleText);
    uint16_t shuffleTextHeight = display.fontHeight();

    int centerShuffleX = shuffleX + (shuffleW - shuffleTextWidth) / 2;
    int centerShuffleY = shuffleY + (shuffleH - shuffleTextHeight) / 2;

    display.tft.drawString(shuffleText, centerShuffleX, centerShuffleY);

}

void MusicScreen::updateVolume(DisplayManager& display, MusicManager& music) {

    uint16_t volX = 20;
    uint16_t volY = 195;
    uint16_t volW = 200;
    uint16_t volH = 10;

    uint8_t getCurrVol = music.getVolume();
    String volume = "Volume: " + String(getCurrVol);

    display.tft.fillRect(volX, volY, volW, volH, TFT_BLACK);

    uint16_t volTextWidth = display.textWidth(volume);
    uint16_t volTextHeight = display.fontHeight();

    int centerVolX = volX + (volW - volTextWidth) / 2;
    int centerVolY = volY + (volH - volTextHeight) / 2;

    display.tft.drawString(volume, centerVolX, centerVolY);

}