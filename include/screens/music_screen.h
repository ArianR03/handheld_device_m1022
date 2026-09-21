#ifndef MUSIC_SCREEN_H
#define MUSIC_SCREEN_H

#include "managers/display.h"
#include "managers/music.h"
#include "managers/rtc.h"

class MusicScreen {
public:

    void begin(DisplayManager& display, MusicManager& music, RTCManager& rtc);
    bool update(DisplayManager& display, MusicManager& music, Button button);
    
private:

    void drawCanvas(DisplayManager& display);
    void updateSongInfo(DisplayManager& display, MusicManager& music);
    void updateProgress(DisplayManager& display, MusicManager& music);
    void updatePlayState(DisplayManager& display, MusicManager& music);
    void updateShuffle(DisplayManager& display, MusicManager& music);
    void updateVolume(DisplayManager& display, MusicManager& music);

    bool handleShuffleTouch(DisplayManager& display, MusicManager& music);

};
#endif