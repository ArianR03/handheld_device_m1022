#include <Arduino.h>
#include "managers/rtc.h"

void RTCManager::syncWithCompiler() { rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); }

DateTime RTCManager::getNow() {
    return rtc.now();
}

void RTCManager::initRTC() {
    if (!rtc.begin()) {
        Serial.println("Failed to initialize PCF8523");
    }
    if (!rtc.initialized()) {
        Serial.println("RTC is not running.");
        syncWithCompiler();
    }
    rtc.start();
}

String RTCManager::retrieveTimePeriod() {
    DateTime now = rtc.now();

    uint8_t currHour = now.hour();

    if (currHour >= 5 && currHour < 12) {
        return "Good Morning";
    } else if (currHour >= 12 && currHour < 17) {
        return "Good Afternoon";
    } else if (currHour >= 17 && currHour < 21) {
        return "Good Evening";
    } else {
        return "Good Night";
    }
}

String RTCManager::retrieveCurrTime() {
    DateTime now = rtc.now();
    
    uint8_t currHour = now.hour();
    uint8_t currMin = now.minute();

    String time = String(currHour) + ":" + String(currMin);
    return time;
}

int RTCManager::retrieveDayOfYear() {
    DateTime now = rtc.now();

    DateTime startOfYear(now.year(), 1, 1, 0, 0, 0);
    uint32_t secondsPassed = now.unixtime() - startOfYear.unixtime();
    
    return (secondsPassed / 86400) + 1; 
}

String RTCManager::retrieveCurrDate() {
    DateTime now = rtc.now();
    return String(now.month()) + "/" + String(now.day()) + "/" + String(now.year());
}

bool RTCManager::isBirthday() {
    DateTime now = rtc.now();
    
    uint8_t month = now.month();
    uint8_t day = now.day();

    // Mireya's Birthday
    if (month == 10 && day == 22) {
        return true;
    } 
    return false;
}

void RTCManager::checkBirthdaySong() {
    
    if (isBirthday()) {
        int buzzer = 5;

        int notes[] = {                             // 25 elements, birthday melody.
            262, 262, 294, 262, 349, 330, 
            262, 262, 294, 262, 392, 349, 
            262, 262, 523, 440, 349, 330, 294, 
            466, 466, 440, 349, 392, 349
        };

        int durations[] = {                         // Trimmed to 25 elements duration to match notes array.
            250, 250, 500, 500, 500, 1000, 
            250, 250, 500, 500, 500, 1000, 
            250, 250, 500, 500, 500, 500, 500, 
            1000, 250, 250, 500, 500, 500
        };
        
        int numNotes = sizeof(notes) / sizeof(notes[0]);

        for (int i = 0; i < numNotes; i++) {
            tone(buzzer, notes[i]);
            delay(durations[i]);
            noTone(buzzer);
            delay(50);
        }
    }
}