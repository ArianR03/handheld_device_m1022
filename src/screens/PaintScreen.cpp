#include "screens/paint_screen.h"

void PaintScreen::begin(DisplayManager& display, PaintManager& paint)
{
    drawCanvasPage(display, paint);
    display.drawSelectionOption("Back", 0, 295, 59, 25, TFT_WHITE);
    display.drawSelectionOption("", 60, 295, 59, 25, TFT_WHITE);
    display.drawSelectionOption("", 120, 295, 59, 25, TFT_WHITE);
    display.drawSelectionOption("Clear", 180, 295, 59, 25, TFT_WHITE);
}

void PaintScreen::drawCanvasPage(DisplayManager& display, PaintManager& paint) {

    display.tft.fillScreen(TFT_BLACK);

    paint.draw_canvas(display);

    paint.draw_color_palette(display, 20, 245);
    paint.draw_size_selector(display);

}

bool PaintScreen::update(DisplayManager& display, PaintManager& paint, Button button) { 
    
    uint16_t x, y;

    switch (button) {
        case Button::BTN1:
            return false;       // Return to home page
        case Button::BTN6:
            // Clear canvas
            paint.clear_canvas(display);
            break;
        default:
            break;
    }

    if (!display.tft.getTouch(&x, &y)) {
        return true;
    }

    if (y >= 235 && y <= 255) {
        paint.update_color(x, y, display);
        return true;
    }

    if (y >= 260 && y <= 285) {
        paint.update_size(x, y, display);
        return true;
    }

    paint.update_canvas(x, y, display);
    return true;
}