# Handheld Embedded Device (Birthday Gift M1022)

A custom handeld embedded system built around the **ESP32-S3**, combining a touchscreen interface, physical controls, local storage, real-time clock, audio playback, and multiple custom applications into a single portable device.

---

## Overview

This project is a custom handheld device designed and developed from the ground up, including both the embedded firmware and hardware integration.

The device provides several applications through a custom touchscreen/physical-button interface:

- **Bible Reader** - Browse books, chapters, and verses stored locally on flash.
- **MP3 Player** - Play, pause, skip, shuffle, and control volume through a DF1201S audio module.
- **Paint** - Draw directly on the touchscreen canvas using selectable colors and pen sizes.
- **Home Screen** - Displays greeting, date, daily verse, and system navigation.
- **Real-Time Clock** - Maintain the device's date and time.
- **Physical Controls** - Analog button interface for navigation and device control.

---

## System Architecture

> **Status:** Under construction

## Firmware Architecture

The firmware is organized into **Managers** and **Screens**.

```text
├───include
│   ├───managers
│   │       bible.h
│   │       buttons.h
│   │       display.h
│   │       music.h
│   │       paint.h
│   │       rtc.h
│   │       
│   └───screens
│           bible_screen.h
│           home_screen.h
│           music_screen.h
│           paint_screen.h
│           
└───src
    │   main.cpp
    │   
    ├───managers
    │       BibleManager.cpp
    │       ButtonManager.cpp
    │       DisplayManager.cpp
    │       MusicManager.cpp
    │       PaintManager.cpp
    │       RTCManager.cpp
    │       
    └───screens
            BibleScreen.cpp
            HomeScreen.cpp
            MusicScreen.cpp
            PaintScreen.cpp
```

**Managers** abstract hardware and system functionality, while **Screens** manage UI rendering, navigation, and user interactions.

---

