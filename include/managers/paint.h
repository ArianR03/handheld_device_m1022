#ifndef PAINT_H
#define PAINT_H

#include "display.h"
#include <Arduino.h>

// display constants

const uint16_t RED     = 0xF800;
const uint16_t GREEN   = 0x07E0;
const uint16_t BLUE    = 0x001F;
const uint16_t CYAN    = 0x07FF;
const uint16_t MAGENTA = 0xF81F;
const uint16_t YELLOW  = 0xFFE0;
const uint16_t WHITE   = 0xFFFF;
const uint16_t GRAY    = 0x520A;
const uint16_t BLACK   = 0x0000;

const uint16_t DISPLAY_WIDTH   = 240;
const uint16_t DISPLAY_HEIGHT  = 320;

// application constants

const uint16_t PAINT_WIDTH = 4;

const uint16_t CANVAS_X = 10;
const uint16_t CANVAS_Y = 10;
const uint16_t CANVAS_W = DISPLAY_WIDTH - (2 * CANVAS_X);
const uint16_t CANVAS_H = 220;

const uint16_t PAINT_RADIUS = 12;
const uint16_t PAINT_OFFSET_X = 40;
const uint16_t PAINT_OFFSET_Y = CANVAS_Y + CANVAS_H + 30;
const uint16_t PAINT_COORS[][3] = {
    {0, 0, TFT_RED},    
    {25, 0, TFT_GREEN},     
    {50, 0, TFT_BLUE},
    {75, 0, TFT_CYAN},  
    {100, 0, TFT_MAGENTA},  
    {125, 0, TFT_YELLOW},
    {150, 0, TFT_WHITE}, 
    {175, 0, TFT_VIOLET},     
    {200, 0, TFT_BLACK}
};

const uint16_t THICKNESS_OPTIONS[] = {3, 5, 7, 9};
const uint16_t THICKNESS_OPTION_COORS[][2] = {{160, PAINT_OFFSET_Y}, {195, PAINT_OFFSET_Y}, {230, PAINT_OFFSET_Y}, {265, PAINT_OFFSET_Y}};

class PaintManager {
public:

    uint16_t getPenColor();
    uint16_t getPenThickness();

    // Math helper functions
    bool in_range(uint16_t value, uint16_t min, uint16_t max);
    uint32_t distance(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);

    // Draw template   
    void draw_canvas(DisplayManager& display);
    void clear_canvas(DisplayManager& display);
    void draw_color_palette(DisplayManager& display, uint16_t offsetX, uint16_t offsetY);
    void draw_size_selector(DisplayManager& display);

    // Update selections
    void update_canvas(uint16_t x, uint16_t y, DisplayManager& display);
    void update_color(uint16_t x, uint16_t y, DisplayManager& display);
    void update_size(uint16_t x, uint16_t y, DisplayManager& display);
private:
    uint16_t lastX = 0;
    uint16_t lastY = 0;
};
#endif