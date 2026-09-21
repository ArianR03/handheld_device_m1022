#include "managers/buttons.h"

void ButtonManager::update() {

    int choice = analogRead(ANALOG_PIN);    // Default: A0

    Button newButton;

    if (choice <= 500) newButton = Button::BTN1;                            
    else if (choice >= 500 && choice < 2000) newButton = Button::BTN2;
    else if (choice >= 2000 && choice < 2750) newButton = Button::BTN3;
    else if (choice >= 2750 && choice < 2900) newButton = Button::BTN4;
    else if (choice >= 2900 && choice < 3150) newButton = Button::BTN5;
    else if (choice >= 3150 && choice < 4000) newButton = Button::BTN6;
    else newButton = Button::NONE;

    currentButton = Button::NONE;

    // Only retrieve a button when new one appears
    if (lastButton == Button::NONE && newButton != Button::NONE) {
        currentButton = newButton;

        Serial.print("BUTTON PRESSED: ");

        switch (newButton) {
            case Button::BTN1: Serial.println("BTN1"); break;
            case Button::BTN2: Serial.println("BTN2"); break;
            case Button::BTN3: Serial.println("BTN3"); break;
            case Button::BTN4: Serial.println("BTN4"); break;
            case Button::BTN5: Serial.println("BTN5"); break;
            case Button::BTN6: Serial.println("BTN6"); break;
            default: break;
        }

        Serial.print("ADC: ");
        Serial.println(choice);
    } 

    lastButton = newButton;

}

Button ButtonManager::getButton() {
    return currentButton;
}