#include "managers/music.h"

MusicManager::MusicManager() 
    : _serial(1),
      _playing(false),
      _shuffle(true),
      _volume(10)
{
}

void MusicManager::begin() {

    _serial.begin(115200, SERIAL_8N1, 44, 43);
    
    if (!_player.begin(_serial)) {
        Serial.println("Failed to init DF1201S.");
        return;
    }

    Serial.println("Init DF1201S.");

    _player.setVol(_volume);

    _player.switchFunction(_player.MUSIC);

    _player.setPlayMode(_player.RANDOM);
}

void MusicManager::play() {
    _player.start();
    _playing = true;
}

void MusicManager::pause() {
    _player.pause();
    _playing = false;
}

void MusicManager::togglePlayPause() {
    if (_playing) {
        pause();
    } else {
        play();
    } 
}

void MusicManager::next() {
    _player.next();
    _playing = true;
}

void MusicManager::previous() {
    _player.last();
    _playing = true;
}

void MusicManager::volumeUp() {
    if (_volume < 30) {
        _volume++;
        _player.setVol(_volume);
    }
}

void MusicManager::volumeDown() {
    if (_volume > 0) {
        --_volume;
        _player.setVol(_volume);
    }
}

void MusicManager::setVolume(uint8_t volume) {
    _volume = constrain(volume, 0, 30);
    _player.setVol(_volume);
}

uint8_t MusicManager::getVolume() const {
    return _volume;
}

void MusicManager::toggleShuffle() {
    _shuffle = !_shuffle;

    if (_shuffle) {
        _player.setPlayMode(_player.RANDOM);
    } else {
        _player.setPlayMode(_player.ALLCYCLE);
    }
}

bool MusicManager::isShuffleEnabled() const {
    return _shuffle;
}

String MusicManager::getSongName() {
    return _player.getFileName();
}

uint16_t MusicManager::getCurrentTime() {
    return _player.getCurTime();
}

uint16_t MusicManager::getTotalTime() {
    return _player.getTotalTime();
}

bool MusicManager::isPlaying() const {
    return _playing;
}