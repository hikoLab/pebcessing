#include "pebcessing_lib.h"


static bool no_stroke_flag = false;
static bool no_fill_flag = false;
static int fill_color = GColorBlack;
static int stroke_color = GColorBlack;
static GFont draw_font;
static GTextAlignment text_alignment = GTextAlignmentLeft;


/* --------------------------
   Global variables
   -------------------------- */

int g_width = 144;                    // Alias of "width"
int g_height = 168;                   // Alias of "height"
unsigned long int g_frame_count = 0;  // Alias of "frameCount"

#ifdef ENABLE_KEY_EVENT
ButtonId g_key_code;  // Alias of "keyCode"
#endif

GBitmap *g_canvas_frame_buffer = NULL;  // A frame buffer for loadPixles(), updatePixels()
uint8_t *g_raw_pixels;                  // An array for fast access to the frame buffer like Processing's "pixels"
uint16_t g_row_size_bytes;              // This variable is needed for access to g_raw_pixels.

GContext *g_ctx;


/* --------------------------
   Environment
   -------------------------- */

void frameRate(float frame_rate)
{
  set_frame_rate(frame_rate);
}

void loop()
{
  enable_loop();
}

void noLoop()
{
  disable_loop();
}

void redraw()
{
  request_update_canvas();
}

// do nothing
void size(float w, float h)
{
  return;
}

/* --------------------------
   2D Primitives
   -------------------------- */

void point(float x, float y)
{
  graphics_draw_pixel(g_ctx, GPoint(x, y));
}

void line(float x1, float y1, float x2, float y2)
{
  graphics_draw_line(g_ctx, GPoint(x1, y1), GPoint(x2, y2));
}

/*
  Cannot draw an ellipse easily in Pebble SDK
  Therefore ellipse() draws a circle.
*/
void ellipse(float x, float y, float w, float h)
{
  float radius = (w + h) / 2;
  circle(x, y, radius);
}

/*
  Not Processing function
  Draw a circle
*/
void circle(float x, float y, float radius)
{
  if (!no_fill_flag) {
    graphics_fill_circle(g_ctx, GPoint(x, y), radius);
  }

  if (!no_stroke_flag) {
    graphics_draw_circle(g_ctx, GPoint(x, y), radius);
  }
}

void rect(float x1, float y1, float x2, float y2)
{
  if (!no_fill_flag) {
    graphics_fill_rect(g_ctx, GRect(x1, y1, x2, y2), 0, GCornerNone);
  }

  if (!no_stroke_flag) {
    graphics_draw_rect(g_ctx, GRect(x1, y1, x2, y2));
  }
}

void quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
  // I'd like to improve the efficiency of the below code, if possible.

  GPoint points[4];
  points[0].x = x1;
  points[0].y = y1;
  points[1].x = x2;
  points[1].y = y2;
  points[2].x = x3;
  points[2].y = y3;
  points[3].x = x4;
  points[3].y = y4;

  GPathInfo path_info;
  path_info.num_points = 4;
  path_info.points = points;

  GPath *path = gpath_create(&path_info);

  if (!no_fill_flag) {
    gpath_draw_filled(g_ctx, path);
  }

  if (!no_stroke_flag) {
    gpath_draw_outline(g_ctx, path);
  }

  gpath_destroy(path);
}

void triangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
  // I'd like to improve the efficiency of the below code, if possible.

  GPoint points[3];
  points[0].x = x1;
  points[0].y = y1;
  points[1].x = x2;
  points[1].y = y2;
  points[2].x = x3;
  points[2].y = y3;

  GPathInfo path_info;
  path_info.num_points = 3;
  path_info.points = points;

  GPath *path = gpath_create(&path_info);

  if (!no_fill_flag) {
    gpath_draw_filled(g_ctx, path);
  }

  if (!no_stroke_flag) {
    gpath_draw_outline(g_ctx, path);
  }

  gpath_destroy(path);
}


/* --------------------------
   Time & Date
   -------------------------- */

int year()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_year;
}

// return value : 0-11
int month()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_mon;
}

// return value : 1-31
int day()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_mday;
}

/*
  Not Processing function
  Return the day of the week
*/
// return value : 0-6
int wday()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_wday;
}

// return value : 0-23
int hour()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_hour;
}

// return value : 0-59
int minute()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_min;
}

// return value : 0-61
int second()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_sec;
}

/*
int millis()
{
  return 0;
}
*/

/* --------------------------
   Color
   -------------------------- */

void background(int color)
{
  if (color == 0) {
    graphics_context_set_fill_color(g_ctx, GColorBlack);
  }
  else {
    graphics_context_set_fill_color(g_ctx, GColorWhite);
  }
  graphics_fill_rect(g_ctx, GRect(0, 0, g_width, g_height), 0, GCornerNone);

  graphics_context_set_fill_color(g_ctx, fill_color);
}

void fill(int color)
{
  if (color == 0) {
    fill_color = GColorBlack;
  }
  else {
    fill_color = GColorWhite;
  }
  graphics_context_set_fill_color(g_ctx, fill_color);

  no_fill_flag = false;
}

void noFill()
{
  no_fill_flag = true;
}

void stroke(int color)
{
  if (color == 0) {
    stroke_color = GColorBlack;
  }
  else {
    stroke_color = GColorWhite;
  }
  graphics_context_set_stroke_color(g_ctx, stroke_color);

  no_stroke_flag = false;
}

void noStroke()
{
  no_stroke_flag = true;
}

/* --------------------------
   Image - Pixels
   -------------------------- */

int getPixel(int x, int y)
{
  if (g_canvas_frame_buffer != NULL) {
    if (0 <= x && x < g_width && 0 <= y && y < g_height) {
      uint8_t n = x % 8;      
      return (g_raw_pixels[x / 8 + y * g_row_size_bytes] & (1 << n)) >> n;
    }
  }
  return 0;
}

void loadPixels()
{
  g_canvas_frame_buffer = graphics_capture_frame_buffer(g_ctx);

  if (g_canvas_frame_buffer == NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Error: graphics_capture_frame_buffer() at loadPixels()");
    return;
  }

  g_raw_pixels = g_canvas_frame_buffer->addr;
  g_row_size_bytes = g_canvas_frame_buffer->row_size_bytes;
}

void setPixel(int x, int y, int color)
{
  if (g_canvas_frame_buffer != NULL) {
    if (0 <= x && x < g_width && 0 <= y && y < g_height) {
      if (color == 0) {
        g_raw_pixels[x / 8 + y * g_row_size_bytes] &= ~(1 << (x % 8));
      }
      else {
        g_raw_pixels[x / 8 + y * g_row_size_bytes] |= 1 << (x % 8);
      }
    }
  }
}

void updatePixels()
{
  if (g_canvas_frame_buffer != NULL) {
    graphics_draw_bitmap_in_rect(g_ctx, g_canvas_frame_buffer, GRect(0, 0, g_canvas_frame_buffer->bounds.size.w, g_canvas_frame_buffer->bounds.size.h));

    if (!graphics_release_frame_buffer(g_ctx, g_canvas_frame_buffer)) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Error: graphics_release_frame_buffer() at updatePixels()");
    }
    g_canvas_frame_buffer = NULL;
  }
}

/* --------------------------
   Trigonometry
   -------------------------- */

// sin() conflicts the compiler's built-in funciton, so this function is renamed _sin().
float _sin(float angle)
{
  return (float)sin_lookup((int32_t)(angle * TRIG_MAX_ANGLE / (2 * PI))) / TRIG_MAX_RATIO;
}

// cos() conflicts the compiler's built-in funciton, so this function is renamed _cos().
float _cos(float angle)
{
  return (float)cos_lookup((int32_t)(angle * TRIG_MAX_ANGLE / (2 * PI))) / TRIG_MAX_RATIO;
}

inline float radians(float degrees)
{
  return degrees * PI / 180.0f;
}

inline float degrees(float radians)
{
  return radians * 180.0f / PI;
}

/* --------------------------
   Random
   -------------------------- */

float random(float low, float high)
{
  float rnd = (float)rand() / RAND_MAX;
  return rnd * (high - low) + low;
}

/* --------------------------
   Typography
   -------------------------- */

GFont loadFont(const char *font_key)
{
  return fonts_get_system_font(font_key);
}

void textFont(GFont font)
{
  draw_font = font;
}

void text(const char *str, float x, float y, float w, float h)
{
  if (draw_font == NULL) {
    draw_font = fonts_get_system_font(FONT_KEY_FONT_FALLBACK);
  }

  graphics_context_set_text_color(g_ctx, fill_color);
  graphics_draw_text(g_ctx, str, draw_font, GRect(x, y, w, h), GTextOverflowModeWordWrap, text_alignment, NULL);
}

void textAlign(int alignX)
{
  text_alignment = alignX;
}

/* --------------------------
   Functions for use in Pebcessing's internal routine
   -------------------------- */

// Set the draw state before drawing the frame.
void set_draw_state()
{
  graphics_context_set_fill_color(g_ctx, fill_color);
  graphics_context_set_stroke_color(g_ctx, stroke_color);
}
