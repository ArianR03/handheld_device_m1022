#include "managers/paint.h"

uint16_t pen_color = TFT_WHITE;
uint16_t pen_thickness = 1;

uint16_t PaintManager::getPenColor() {
    return pen_color;
}

uint16_t PaintManager::getPenThickness() {
    return pen_thickness;
}

bool PaintManager::in_range (uint16_t value, uint16_t min, uint16_t max) {
    return (min <= value) && (value <= max);
}

uint32_t PaintManager::distance(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) {
    
    uint32_t i = (x0 > x1) ? (x0 - x1) : (x1 - x0);
    uint32_t j = (y0 > y1) ? (y0 - y1) : (y1 - y0);

    return sqrt(i*i + j*j);
}

// Functions to handle touch input
void PaintManager::draw_canvas(DisplayManager& display) { 
    display.tft.drawRect(CANVAS_X, CANVAS_Y, CANVAS_W, CANVAS_H, TFT_WHITE);
}

void PaintManager::clear_canvas(DisplayManager& display) {
    display.tft.fillRect(CANVAS_X, CANVAS_Y, CANVAS_W, CANVAS_H, TFT_BLACK);
    draw_canvas(display);
}

void PaintManager::draw_color_palette(DisplayManager& display, uint16_t offsetX, uint16_t offsetY) {
    
    uint16_t x, y, c;

    for (uint16_t i = 0; i < sizeof(PAINT_COORS)/sizeof(PAINT_COORS[0]); i++) {
        x = PAINT_COORS[i][0] + offsetX;
        y = PAINT_COORS[i][1] + offsetY;
        c = PAINT_COORS[i][2];

        display.tft.fillCircle(x, y, 8, c);
        display.tft.drawCircle(x, y, 10, TFT_WHITE);
    }
}

void PaintManager::draw_size_selector(DisplayManager& display) {

    uint16_t x, y, t;

    for (uint16_t i = 0; i < sizeof(THICKNESS_OPTIONS)/sizeof(THICKNESS_OPTIONS[0]); i++) {

        x = 70 + (i*30);
        y = 275;
        t = THICKNESS_OPTIONS[i];

        display.tft.fillCircle(x, y, t, pen_color);

        if (i == pen_thickness) {
            display.tft.drawCircle(x, y, t + 3, TFT_WHITE);
        } else {
            display.tft.drawCircle(x, y, t + 3, TFT_DARKGREY);
        }
    }
}

void PaintManager::update_canvas(uint16_t x, uint16_t y, DisplayManager& display) {

    uint16_t t = THICKNESS_OPTIONS[pen_thickness];

    if (in_range(x, CANVAS_X + t + 2, CANVAS_X + CANVAS_W - t -2) && 
        in_range(y, CANVAS_Y + t + 2, CANVAS_Y + CANVAS_H - t -2)) {
            display.tft.fillCircle(x, y, t, pen_color);

            lastX = x;
            lastY = y;
        }
}

void PaintManager::update_color(uint16_t x, uint16_t y, DisplayManager& display) {

    uint32_t x0, y0, d;

    for (uint16_t i = 0; i < sizeof(PAINT_COORS)/sizeof(PAINT_COORS[0]); i++) {

        x0 = PAINT_COORS[i][0] + 20;
        y0 = PAINT_COORS[i][1] + 245;
        d = distance(x0, y0, x, y);

        if (d <= 15) {
            pen_color = PAINT_COORS[i][2];
            draw_size_selector(display);
            break;
        }
    }
}

void PaintManager::update_size(uint16_t x, uint16_t y, DisplayManager& display) { 
    
    uint32_t x0, y0, d;

    for (uint16_t i = 0; i < sizeof(THICKNESS_OPTIONS)/sizeof(THICKNESS_OPTIONS[0]); i++) {

        x0 = 70 + (i*30);
        y0 = 275;

        d = distance(x0, y0, x, y);

        if (d <= 15) {
            pen_thickness = i;
            draw_size_selector(display);
            break;
        }
    }
}