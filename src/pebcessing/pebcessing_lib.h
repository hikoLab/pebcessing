#pragma once

#include <pebble.h>
#include "pebcessing/pebcessing_core.h"
#include "../settings.h"


extern int g_width;
extern int g_height;
extern unsigned long int g_frame_count;

#ifdef ENABLE_KEY_EVENT
extern ButtonId g_key_code;
#endif

extern GBitmap *g_canvas_frame_buffer;
extern uint8_t *g_raw_pixels;
extern uint16_t g_row_size_bytes;

extern GContext *g_ctx;


#define M_PI 3.14159265358979323846f
#define SHRT_MIN -32768
#define SHRT_MAX 32767
#define INT_MIN -2147483648
#define INT_MAX 2147483647


// Enumerated type for colorMode()
enum ColorMode { COLOR_MODE_RGB, COLOR_MODE_HSB };

// Enumerated type for text alignment or shape draw mode
enum DrawOption {
  DRAW_OPTION_LEFT = GTextAlignmentLeft,
  DRAW_OPTION_RIGHT = GTextAlignmentRight,
  DRAW_OPTION_CENTER = GTextAlignmentCenter,
  DRAW_OPTION_CORNER = GTextAlignmentCenter + 0x100,  // There is no particular meaning to 0x100.
  DRAW_OPTION_CORNERS,
  DRAW_OPTION_RADIUS
};


/* --------------------------
   Processing-like functions
   -------------------------- */
void frameRate(float frame_rate);
void loop();
void noLoop();
void redraw();
void size(int w, int h);
void point(int x, int y);
void line(int x1, int y1, int x2, int y2);
void ellipse(int x, int y, int w, int h);
void circle(int x, int y, int radius);
void rect(int x1, int y1, int x2, int y2);
void quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
void triangle(int x1, int y1, int x2, int y2, int x3, int y3);
int year();
int month();
int day();
int wday();
int hour();
int minute();
int second();
long int millis();
uint8_t color(float r, float g, float b);
void background(uint8_t color);
void colorMode(int mode);
void fill(uint8_t color);
void noFill();
void stroke(uint8_t color);
void noStroke();
uint8_t getPixel(int x, int y);
void loadPixels();
void setPixel(int x, int y, uint8_t color);
void updatePixels();
float constrain(float value, float low, float high);
float map(float value, float start1, float stop1, float start2, float stop2);
float _sin(float angle);
float _cos(float angle);
float radians(float degrees);
float degrees(float radians);
float random(float low, float high);
GFont loadFont(uint32_t resource_id);
GFont loadSystemFont(const char *font_key);
void textFont(GFont font);
void text(const char *str, int x, int y);
void textInRect(const char *str, int x, int y, int w, int h);
void textAlign(int alignX);
int textWidth(const char *str);
int textWidthInRect(const char *str, int w, int h);


/* --------------------------
   Functions for use in Pebcessing's internal routine
   -------------------------- */
void init_pebcessing_lib();
void set_draw_state();
