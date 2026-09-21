#ifndef MUSIC_H
#define MUSIC_H

#include <Arduino.h>
#include <DFRobot_DF1201S.h>
#include <HardwareSerial.h>

class MusicManager {
public:
    MusicManager();

    void begin();

    void play();
    void pause();
    void togglePlayPause();
    void next();
    void previous();

    void volumeUp();
    void volumeDown();
    void setVolume(uint8_t vol);
    uint8_t getVolume() const;

    void toggleShuffle();
    bool isShuffleEnabled() const;

    String getSongName();
    uint16_t getCurrentTime();
    uint16_t getTotalTime();

    bool isPlaying() const;

private:
    HardwareSerial _serial;
    DFRobot_DF1201S _player;

    bool _playing;
    bool _shuffle;
    uint8_t _volume;

};  

#endif