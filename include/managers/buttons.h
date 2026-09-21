#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

enum class Button {
    NONE,
    BTN1,
    BTN2,
    BTN3,
    BTN4,
    BTN5,
    BTN6
};

class ButtonManager {
public:
    void update();
    Button getButton();
private:
    static constexpr int ANALOG_PIN = A1;
    Button currentButton = Button::NONE;
    Button lastButton = Button::NONE;
};

#endif