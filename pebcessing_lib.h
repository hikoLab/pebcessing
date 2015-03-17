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


#define M_PI (3.14159265358979323846f)

enum ColorMode { COLOR_MODE_RGB, COLOR_MODE_HSB };


/* --------------------------
   Processing-like functions
   -------------------------- */
void frameRate(float frame_rate);
void loop();
void noLoop();
void redraw();
void size(float w, float h);
void point(float x, float y);
void line(float x1, float y1, float x2, float y2);
void ellipse(float x, float y, float w, float h);
void circle(float x, float y, float radius);
void rect(float x1, float y1, float x2, float y2);
void quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
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
void text(const char *str, float x, float y);
void textInRect(const char *str, float x, float y, float w, float h);
void textAlign(int alignX);

/* --------------------------
   Functions for use in Pebcessing's internal routine
   -------------------------- */
void init_pebcessing_lib();
void set_draw_state();
