#pragma once

#include <pebble.h>
#include "pebcessing/pebcessing_core.h"
#include "../settings.h"


extern int g_pblp5_width;
extern int g_pblp5_height;
extern unsigned long int g_pblp5_frame_count;

#ifdef ENABLE_KEY_EVENT
extern ButtonId g_pblp5_key_code;
#endif

extern GBitmap *g_pblp5_canvas_frame_buffer;
extern uint8_t *g_pblp5_raw_pixels;
extern uint16_t g_pblp5_row_size_bytes;

extern GContext *g_pblp5_context;


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
void pblp5_frameRate(float frame_rate);
void pblp5_loop();
void pblp5_noLoop();
void pblp5_redraw();
void pblp5_size(int w, int h);
void pblp5_point(int x, int y);
void pblp5_line(int x1, int y1, int x2, int y2);
void pblp5_ellipse(int x, int y, int w, int h);
void pblp5_circle(int x, int y, int w);
void pblp5_rect(int x1, int y1, int x2, int y2);
#ifndef quad
void pblp5_quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
#else
void quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
#endif
void pblp5_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
void pblp5_ellipseMode(int mode);
int pblp5_year();
int pblp5_month();
int pblp5_day();
int pblp5_wday();
int pblp5_hour();
int pblp5_minute();
int pblp5_second();
long int pblp5_millis();
uint8_t pblp5_color(float r, float g, float b);
void pblp5_background(uint8_t color);
void pblp5_colorMode(int mode);
void pblp5_fill(uint8_t color);
void pblp5_noFill();
void pblp5_stroke(uint8_t color);
void pblp5_noStroke();
uint8_t pblp5_getPixel(int x, int y);
void pblp5_loadPixels();
void pblp5_setPixel(int x, int y, uint8_t color);
void pblp5_updatePixels();
float pblp5_constrain(float value, float low, float high);
float pblp5_map(float value, float start1, float stop1, float start2, float stop2);
float pblp5_sin(float angle);
float pblp5_cos(float angle);
float pblp5_radians(float degrees);
float pblp5_degrees(float radians);
float pblp5_random(float low, float high);
GFont pblp5_loadFont(uint32_t resource_id);
GFont pblp5_loadSystemFont(const char *font_key);
void pblp5_textFont(GFont font);
void pblp5_text(const char *str, int x, int y);
void pblp5_textInRect(const char *str, int x, int y, int w, int h);
void pblp5_textAlign(int alignX);
GSize pblp5_textContentSize(const char *str);
int pblp5_textHeight(const char *str);
int pblp5_textWidth(const char *str);
int pblp5_textWidthInRect(const char *str, int w, int h);


/* --------------------------
   Functions for use in Pebcessing's internal routine
   -------------------------- */
void pblp5_init_lib();
void pblp5_set_draw_state();
