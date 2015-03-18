#include "pebcessing_lib.h"


static bool no_stroke_flag = false;
static bool no_fill_flag = false;
static GColor fill_color;
static GColor stroke_color;
static GFont draw_font;
static GTextAlignment text_alignment = GTextAlignmentLeft;
static int color_mode = COLOR_MODE_RGB;
static float color_v1_max = 255;
static float color_v2_max = 255;
static float color_v3_max = 255;


/* --------------------------
   Global variables
   -------------------------- */

int g_width = 144;                      // Alias of "width"
int g_height = 168;                     // Alias of "height"
unsigned long int g_frame_count = 0;    // Alias of "frameCount"

#ifdef ENABLE_KEY_EVENT
ButtonId g_key_code;                    // Alias of "keyCode"
#endif

GBitmap *g_canvas_frame_buffer = NULL;  // A frame buffer for loadPixles(), updatePixels()
uint8_t *g_raw_pixels;                  // An array for fast access to the frame buffer like Processing's "pixels"
uint16_t g_row_size_bytes;              // This variable is needed for access to g_raw_pixels.
GContext *g_ctx;


static void conv_black_color(uint8_t *color);
static uint8_t get_color_from_hsb(float hue, float saturation, float brightness);


/* --------------------------
   Environment
   -------------------------- */

inline void frameRate(float frame_rate)
{
  set_frame_rate(frame_rate);
}

inline void loop()
{
  enable_loop();
}

inline void noLoop()
{
  disable_loop();
}

inline void redraw()
{
  request_update_canvas();
}

// do nothing
inline void size(float w, float h)
{
  return;
}

/* --------------------------
   2D Primitives
   -------------------------- */

inline void point(float x, float y)
{
  graphics_draw_pixel(g_ctx, GPoint(x, y));
}

inline void line(float x1, float y1, float x2, float y2)
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
  return current_time->tm_year + 1900;
}

// return value : 1-12
int month()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_mon + 1;
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

// return value : 0-61 (60 or 61 are leap-seconds)
int second()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_sec;
}

long int millis()
{
  time_t sec;
  uint16_t ms;
  time_ms(&sec, &ms);
  return sec * 1000 + ms;
}

/* --------------------------
   Color
   -------------------------- */

uint8_t color(float v1, float v2, float v3)
{
  if (v1 < 0)
    v1 = 0;
  else if (v1 > color_v1_max)
    v1 = color_v1_max;

  if (v2 < 0)
    v2 = 0;
  else if (v2 > color_v2_max)
    v2 = color_v2_max;

  if (v3 < 0)
    v3 = 0;
  else if (v3 > color_v3_max)
    v3 = color_v3_max;

#ifdef PBL_COLOR
  uint8_t color;
  if (color_mode == COLOR_MODE_RGB) {
    color = GColorFromRGB(v1, v2, v3).argb;
  }
  else if (color_mode == COLOR_MODE_HSB) {
    color = get_color_from_hsb(v1, v2, v3);
  }
  conv_black_color(&color);
  return color;
#else
  // Calculate gray scale of the RGB color
  if ((299 * v1 + 587 * v2 + 114 * v3) > 1000 * 255 / 2) {
    return GColorWhite;
  }
  else {
    return GColorBlack;
  }
#endif
}

void background(uint8_t color)
{
#ifdef PBL_COLOR
  conv_black_color(&color);
  graphics_context_set_fill_color(g_ctx, (GColor8){.argb=color});
#else
  if ((GColor)color == GColorBlack) {
    graphics_context_set_fill_color(g_ctx, GColorBlack);
  }
  else {
    graphics_context_set_fill_color(g_ctx, GColorWhite);
  }
#endif
  graphics_fill_rect(g_ctx, GRect(0, 0, g_width, g_height), 0, GCornerNone);

  graphics_context_set_fill_color(g_ctx, fill_color);
}

inline void colorMode(int mode)
{
#ifdef PBL_COLOR
  color_mode = mode;
#endif
}

void fill(uint8_t color)
{
#ifdef PBL_COLOR
  conv_black_color(&color);
  fill_color = (GColor8){.argb=color};
#else
  if (color == GColorBlack) {
    fill_color = GColorBlack;
  }
  else {
    fill_color = GColorWhite;
  }
#endif

  graphics_context_set_fill_color(g_ctx, fill_color);

  no_fill_flag = false;
}

inline void noFill()
{
  no_fill_flag = true;
}

void stroke(uint8_t color)
{
#ifdef PBL_COLOR
  conv_black_color(&color);
  stroke_color = (GColor8){.argb=color};
#else
  if (color == GColorBlack) {
    stroke_color = GColorBlack;
  }
  else {
    stroke_color = GColorWhite;
  }
#endif
  graphics_context_set_stroke_color(g_ctx, stroke_color);

  no_stroke_flag = false;
}

inline void noStroke()
{
  no_stroke_flag = true;
}

/* --------------------------
   Image - Pixels
   -------------------------- */

uint8_t getPixel(int x, int y)
{
  if (g_canvas_frame_buffer != NULL) {
    if (0 <= x && x < g_width && 0 <= y && y < g_height) {
#ifdef PBL_COLOR
      uint8_t color = g_raw_pixels[x + y * g_row_size_bytes];
      conv_black_color(&color);
      return color;
#else
      uint8_t n = x % 8;
      return (g_raw_pixels[x / 8 + y * g_row_size_bytes] & (1 << n)) >> n;
#endif
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

  g_raw_pixels = gbitmap_get_data(g_canvas_frame_buffer);
  g_row_size_bytes = gbitmap_get_bytes_per_row(g_canvas_frame_buffer);
}

void setPixel(int x, int y, uint8_t color)
{
  if (g_canvas_frame_buffer != NULL) {
    if (0 <= x && x < g_width && 0 <= y && y < g_height) {
#ifdef PBL_COLOR
      conv_black_color(&color);
      g_raw_pixels[x + y * g_row_size_bytes] = color;
#else
      if (color == 0) {
        g_raw_pixels[x / 8 + y * g_row_size_bytes] &= ~(1 << (x % 8));
      }
      else {
        g_raw_pixels[x / 8 + y * g_row_size_bytes] |= 1 << (x % 8);
      }
#endif
    }
  }
}

void updatePixels()
{
  if (g_canvas_frame_buffer != NULL) {

    graphics_draw_bitmap_in_rect(g_ctx, g_canvas_frame_buffer, gbitmap_get_bounds(g_canvas_frame_buffer));

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
inline float _sin(float angle)
{
  return (float)sin_lookup((int32_t)(angle * TRIG_MAX_ANGLE / (2 * M_PI))) / TRIG_MAX_RATIO;
}

// cos() conflicts the compiler's built-in funciton, so this function is renamed _cos().
inline float _cos(float angle)
{
  return (float)cos_lookup((int32_t)(angle * TRIG_MAX_ANGLE / (2 * M_PI))) / TRIG_MAX_RATIO;
}

inline float radians(float degrees)
{
  return degrees * M_PI / 180.0f;
}

inline float degrees(float radians)
{
  return radians * 180.0f / M_PI;
}

/* --------------------------
   Random
   -------------------------- */

inline float random(float low, float high)
{
  float rnd = (float)rand() / RAND_MAX;
  return rnd * (high - low) + low;
}

/* --------------------------
   Calculation
   -------------------------- */

float constrain(float value, float low, float high)
{
  if (value < low)
    return low;
  else if (value > high)
    return high;

  return value;
}

inline float map(float value, float start1, float stop1, float start2, float stop2)
{
  return (value - start1) * (stop2 - start2) / (stop1 - start1) + start2;
}

/* --------------------------
   Typography
   -------------------------- */

inline GFont loadFont(uint32_t resource_id)
{
  return fonts_load_custom_font(resource_get_handle(resource_id));
}

inline GFont loadSystemFont(const char *font_key)
{
  return fonts_get_system_font(font_key);
}

inline void textFont(GFont font)
{
  draw_font = font;
}

void text(const char *str, float x, float y)
{
  // Calculate the horizontal alignment by myself.
  if(text_alignment == GTextAlignmentRight){
    x -= textWidth(str);
  }else if(text_alignment == GTextAlignmentCenter){
    x -= textWidth(str) / 2;
  }

  GTextAlignment tmp_text_alignment = GTextAlignmentLeft;
  graphics_context_set_text_color(g_ctx, fill_color);
  graphics_draw_text(g_ctx, str, draw_font, GRect(x, y, SHRT_MAX, SHRT_MAX), GTextOverflowModeWordWrap, tmp_text_alignment, NULL);
}

void textInRect(const char *str, float x, float y, float w, float h)
{
  graphics_context_set_text_color(g_ctx, fill_color);
  graphics_draw_text(g_ctx, str, draw_font, GRect(x, y, w, h), GTextOverflowModeWordWrap, text_alignment, NULL);
}

inline void textAlign(int alignX)
{
  text_alignment = alignX;
}

inline int textWidth(const char *str)
{
  GSize size = graphics_text_layout_get_content_size(str, draw_font, GRect(0, 0, SHRT_MAX, SHRT_MAX), GTextOverflowModeWordWrap, text_alignment);
  return size.w;
}

inline int textWidthInRect(const char *str, float w, float h)
{
  GSize size = graphics_text_layout_get_content_size(str, draw_font, GRect(0, 0, w, h), GTextOverflowModeWordWrap, text_alignment);
  return size.w;
}

/* --------------------------
   Functions for use in Pebcessing's internal routine
   -------------------------- */

void init_pebcessing_lib()
{
  fill_color = GColorWhite;
  stroke_color = GColorBlack;

  draw_font = fonts_get_system_font(FONT_KEY_FONT_FALLBACK);
}

// Set the draw state before drawing the frame.
void set_draw_state()
{
  graphics_context_set_fill_color(g_ctx, fill_color);
  graphics_context_set_stroke_color(g_ctx, stroke_color);
}

#ifdef PBL_COLOR
// This function is implemented in an "ad hoc manner".
// Convert black color (Swap 0b11000000 <-> 0b00000000)
// The color of fill(0) is black in Processing, but GColorBlack is 0b11000000.
// So convert a value of a color by this function.
static void conv_black_color(uint8_t *color)
{
  if (*color == 0) {
    *color = (uint8_t)0b11000000;
  }
  else if (*color == 0b11000000) {
    *color = 0;
  }
}
#endif

#ifdef PBL_COLOR
static uint8_t get_color_from_hsb(float hue, float saturation, float brightness)
{
  float h = (hue / color_v1_max) * 360.0f / 60.0f;
  float s = saturation / color_v2_max;
  float v = brightness / color_v3_max;

  float r = 0;
  float g = 0;
  float b = 0;

  if (s == 0) {
    r = v;
    g = v;
    b = v;
  }
  else {
    int i = (int)h;
    float f = h - i;
    float p = v * (1.0f - s);
    float q;
    if (i % 2 == 0) {
      q = v * (1.0f - (1.0f - f) * s);
    }
    else {
      q = v * (1.0f - f * s);
    }

    switch (i) {
      case 0:
        r = v;
        g = q;
        b = p;
        break;
      case 1:
        r = q;
        g = v;
        b = p;
        break;
      case 2:
        r = p;
        g = v;
        b = q;
        break;
      case 3:
        r = p;
        g = q;
        b = v;
        break;
      case 4:
        r = q;
        g = p;
        b = v;
        break;
      case 5:
        r = v;
        g = p;
        b = q;
        break;
      default:
        break;
    }
  }

  return GColorFromRGB(r * 255, g * 255, b * 255).argb;
}
#endif