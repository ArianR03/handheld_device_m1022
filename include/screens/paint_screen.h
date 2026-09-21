#ifndef PAINT_SCREEN_H
#define PAINT_SCREEN_H

#include <Arduino.h>

#include "managers/paint.h"
#include "managers/buttons.h"


class PaintScreen {
public:

    void begin(DisplayManager& display, PaintManager& paint);
    bool update(DisplayManager& display, PaintManager& paint, Button button);
    void drawCanvasPage(DisplayManager& display, PaintManager& paint);
};

#endif 