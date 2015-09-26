#include "pebcessing_lib.h"


// This struct type is used for pushMatrix(), popMatrix().
typedef struct {
  float x;
  float y;
  float angle;
} transform_param_t;

// pushMatrix() can use push 32 times.
#define TRANSFORM_STACK_MAX 32

static bool no_stroke_flag = false;
static bool no_fill_flag = false;
static GColor fill_color;
static GColor stroke_color;
#ifdef PBL_COLOR
static uint8_t stroke_weight = 1;
#endif
static GFont draw_font;
static GTextAlignment text_alignment = GTextAlignmentLeft;
static int color_mode = COLOR_MODE_RGB;
static float color_v1_max = 255;
static float color_v2_max = 255;
static float color_v3_max = 255;
static int ellipse_mode = DRAW_OPTION_CENTER;
static int rect_mode = DRAW_OPTION_CORNER;
static long int pblp5_init_time;

static transform_param_t transform_stack[TRANSFORM_STACK_MAX];
static int transform_stack_index = 0;
static float translate_x = 0;
static float translate_y = 0;
static float rotate_angle = 0;
static int32_t calculated_rotate_angle = 0;

static GPath *line_path = NULL;
static const GPathInfo LINE_PATH_INFO = {.num_points = 2, .points = (GPoint[]){{0, 0}, {0, 0}}};
static GPath *triangle_path = NULL;
static const GPathInfo TRIANGLE_PATH_INFO = {.num_points = 3, .points = (GPoint[]){{0, 0}, {0, 0}, {0, 0}}};
static GPath *quad_path = NULL;
static const GPathInfo QUAD_PATH_INFO = {.num_points = 4, .points = (GPoint[]){{0, 0}, {0, 0}, {0, 0}, {0, 0}}};

static GBitmap **img_array = NULL;
static int img_array_length = 0;
static int img_array_index = 0;

static void convert_params_by_rect_mode(int *x, int *y, int *w, int *h);
static void convert_transformed_pos(int *x, int *y);
static void convert_black_color(uint8_t *color);
static uint8_t get_color_from_hsb(float hue, float saturation, float brightness);


/* --------------------------
   Global variables
   -------------------------- */

int g_pblp5_width;                          // Alias of "width"
int g_pblp5_height;                         // Alias of "height"
unsigned long int g_pblp5_frame_count = 0;  // Alias of "frameCount"

#ifdef ENABLE_KEY_EVENT
ButtonId g_pblp5_key_code;  // Alias of "keyCode"
#endif

GBitmap *g_pblp5_canvas_frame_buffer = NULL;  // A frame buffer for loadPixles(), updatePixels()
uint8_t *g_pblp5_raw_pixels;                  // An array for fast access to the frame buffer like Processing's "pixels"
uint16_t g_pblp5_row_size_bytes;              // This variable is needed for access to g_raw_pixels.
GContext *g_pblp5_context;


// For convenience
#define ctx   g_pblp5_context


/* --------------------------
   Environment
   -------------------------- */

inline void pblp5_frameRate(float frame_rate)
{
  pblp5_set_frame_rate(frame_rate);
}

inline void pblp5_loop()
{
  pblp5_enable_loop();
}

inline void pblp5_noLoop()
{
  pblp5_disable_loop();
}

inline void pblp5_redraw()
{
  pblp5_request_update_canvas();
}

// do nothing
inline void pblp5_size(int w, int h)
{
  return;
}

/* --------------------------
   2D Primitives
   -------------------------- */

void pblp5_point(int x, int y)
{
  if (!no_stroke_flag) {
    convert_transformed_pos(&x, &y);
    graphics_draw_pixel(ctx, GPoint(x, y));
  }
}

void pblp5_line(int x1, int y1, int x2, int y2)
{
  if (no_stroke_flag) return;

  if (rotate_angle == 0) {
    graphics_draw_line(ctx, GPoint(x1 + translate_x, y1 + translate_y), GPoint(x2 + translate_x, y2 + translate_y));
  }
  else {
    line_path->points[0].x = x1;
    line_path->points[0].y = y1;
    line_path->points[1].x = x2;
    line_path->points[1].y = y2;

    gpath_rotate_to(line_path, calculated_rotate_angle);
    gpath_move_to(line_path, GPoint(translate_x, translate_y));

    gpath_draw_outline(ctx, line_path);
  }
}

/*
  Cannot draw an ellipse easily in Pebble SDK
  Therefore ellipse() draws a circle.
*/
void pblp5_ellipse(int x, int y, int w, int h)
{
  if (w != h) {
    w = (w + h) / 2;
  }
  pblp5_circle(x, y, w);
}

void pblp5_circle(int x, int y, int w)
{
  int radius = 0;

  // 'CORNERS' mode is invalid because it's not possible to draw an ellipse with Pebble SDK.
  switch (ellipse_mode) {
    case DRAW_OPTION_CENTER:
      radius = w / 2;
      break;
    case DRAW_OPTION_RADIUS:
      radius = w;
      break;
    case DRAW_OPTION_CORNER:
      x = x + w / 2;
      y = y + w / 2;
      radius = w / 2;
      break;
    default:
      radius = w / 2;
  }

  convert_transformed_pos(&x, &y);

  if (!no_fill_flag) {
    graphics_fill_circle(ctx, GPoint(x, y), radius);
  }

  if (!no_stroke_flag) {
    graphics_draw_circle(ctx, GPoint(x, y), radius);
  }
}

void pblp5_rect(int a, int b, int c, int d)
{
  int x = a, y = b, w = c, h = d;

  convert_params_by_rect_mode(&x, &y, &w, &h);

  if (rotate_angle == 0) {
    x += translate_x;
    y += translate_y;

    if (!no_fill_flag) {
      graphics_fill_rect(ctx, GRect(x, y, w, h), 0, GCornerNone);
    }
    if (!no_stroke_flag) {
      graphics_draw_rect(ctx, GRect(x, y, w, h));
    }
  }
  else {
    quad(x, y, x + w, y, x + w, y + h, x, y + h);
  }
}

/*
  If quad macro is defined, the error "quad redefined" occurs because quad macro is already defined at "arm-cs-tools/arm-none-eabi/include/sys/types.h".
  So pblp5_quad() is defined directly as quad() if quad macro is already defined.
  It's not good code...
 */
#ifndef quad
void pblp5_quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
#else
void quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
#endif
{
  quad_path->points[0].x = x1;
  quad_path->points[0].y = y1;
  quad_path->points[1].x = x2;
  quad_path->points[1].y = y2;
  quad_path->points[2].x = x3;
  quad_path->points[2].y = y3;
  quad_path->points[3].x = x4;
  quad_path->points[3].y = y4;

  gpath_rotate_to(quad_path, calculated_rotate_angle);
  gpath_move_to(quad_path, GPoint(translate_x, translate_y));

  if (!no_fill_flag) {
    gpath_draw_filled(ctx, quad_path);
  }
  if (!no_stroke_flag) {
    gpath_draw_outline(ctx, quad_path);
  }
}

void pblp5_triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
  triangle_path->points[0].x = x1;
  triangle_path->points[0].y = y1;
  triangle_path->points[1].x = x2;
  triangle_path->points[1].y = y2;
  triangle_path->points[2].x = x3;
  triangle_path->points[2].y = y3;

  gpath_rotate_to(triangle_path, calculated_rotate_angle);
  gpath_move_to(triangle_path, GPoint(translate_x, translate_y));

  if (!no_fill_flag) {
    gpath_draw_filled(ctx, triangle_path);
  }
  if (!no_stroke_flag) {
    gpath_draw_outline(ctx, triangle_path);
  }
}

inline void pblp5_ellipseMode(int mode)
{
  ellipse_mode = mode;
}

inline void pblp5_rectMode(int mode)
{
  rect_mode = mode;
}


inline void pblp5_noSmooth()
{
#ifdef PBL_COLOR
  graphics_context_set_antialiased(ctx, false);
#endif
}

inline void pblp5_smooth()
{
#ifdef PBL_COLOR
  graphics_context_set_antialiased(ctx, true);
#endif
}

inline void pblp5_strokeWeight(int weight)
{
#ifdef PBL_COLOR
  stroke_weight = weight;
  graphics_context_set_stroke_width(ctx, weight);
#endif
}

/* --------------------------
   Time & Date
   -------------------------- */

int pblp5_year()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_year + 1900;
}

// return value : 1-12
int pblp5_month()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_mon + 1;
}

// return value : 1-31
int pblp5_day()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_mday;
}

// Return the day of the week
// return value : 0-6
int pblp5_wday()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_wday;
}

// return value : 0-23
int pblp5_hour()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_hour;
}

// return value : 0-59
int pblp5_minute()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_min;
}

// return value : 0-61 (60 or 61 are leap-seconds)
int pblp5_second()
{
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  return current_time->tm_sec;
}

long int pblp5_millis()
{
  time_t sec;
  uint16_t ms;
  time_ms(&sec, &ms);
  return sec * 1000 + ms - pblp5_init_time;
}

/* --------------------------
   Transform
   -------------------------- */

void pblp5_popMatrix()
{
  if (transform_stack_index > 0) {
    transform_stack_index--;
    translate_x = transform_stack[transform_stack_index].x;
    translate_y = transform_stack[transform_stack_index].y;
    rotate_angle = transform_stack[transform_stack_index].angle;

    calculated_rotate_angle = TRIG_MAX_ANGLE * rotate_angle / (2 * M_PI);
  }
  else {
    translate_x = 0;
    translate_y = 0;
    rotate_angle = 0;

    calculated_rotate_angle = 0;
  }
}

void pblp5_pushMatrix()
{
  if (transform_stack_index >= 32) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Error: pushMatrix() cannot use push more than 32 times");
    return;
  }
  else {
    transform_stack[transform_stack_index].x = translate_x;
    transform_stack[transform_stack_index].y = translate_y;
    transform_stack[transform_stack_index].angle = rotate_angle;
    transform_stack_index++;

    calculated_rotate_angle = TRIG_MAX_ANGLE * rotate_angle / (2 * M_PI);
  }
}

inline void pblp5_resetMatrix()
{
  translate_x = 0;
  translate_y = 0;
  rotate_angle = 0;
  calculated_rotate_angle = 0;
}

void pblp5_rotate(float angle)
{
  rotate_angle += angle;

  calculated_rotate_angle = TRIG_MAX_ANGLE * rotate_angle / (2 * M_PI);

  if (calculated_rotate_angle < 0 || calculated_rotate_angle > TRIG_MAX_ANGLE) {
    calculated_rotate_angle %= TRIG_MAX_ANGLE;
    if (calculated_rotate_angle < 0) {
      calculated_rotate_angle += TRIG_MAX_ANGLE;
    }

    rotate_angle = (float)calculated_rotate_angle * 2 * M_PI / TRIG_MAX_ANGLE;
  }
}

void pblp5_translate(float x, float y)
{
  float c = pblp5_cos(rotate_angle);
  float s = pblp5_sin(rotate_angle);
  translate_x += x * c - y * s;
  translate_y += x * s + y * c;
}

/* --------------------------
   Color - Setting
   -------------------------- */

void pblp5_background(uint8_t color)
{
#ifdef PBL_COLOR
  convert_black_color(&color);
  graphics_context_set_fill_color(ctx, (GColor8){.argb = color});
#else
  if ((GColor)color == GColorBlack) {
    graphics_context_set_fill_color(ctx, GColorBlack);
  }
  else {
    graphics_context_set_fill_color(ctx, GColorWhite);
  }
#endif
  graphics_fill_rect(ctx, GRect(0, 0, g_pblp5_width, g_pblp5_height), 0, GCornerNone);

  graphics_context_set_fill_color(ctx, fill_color);
}

inline void pblp5_colorMode(int mode)
{
#ifdef PBL_COLOR
  color_mode = mode;
#endif
}

void pblp5_fill(uint8_t color)
{
#ifdef PBL_COLOR
  convert_black_color(&color);
  fill_color = (GColor8){.argb = color};
#else
  if (color == GColorBlack) {
    fill_color = GColorBlack;
  }
  else {
    fill_color = GColorWhite;
  }
#endif

  graphics_context_set_fill_color(ctx, fill_color);

  no_fill_flag = false;
}

inline void pblp5_noFill()
{
  no_fill_flag = true;
}

void pblp5_stroke(uint8_t color)
{
#ifdef PBL_COLOR
  convert_black_color(&color);
  stroke_color = (GColor8){.argb = color};
#else
  if (color == GColorBlack) {
    stroke_color = GColorBlack;
  }
  else {
    stroke_color = GColorWhite;
  }
#endif
  graphics_context_set_stroke_color(ctx, stroke_color);

  no_stroke_flag = false;
}

inline void pblp5_noStroke()
{
  no_stroke_flag = true;
}

/* --------------------------
   Color - Creating & Reading
   -------------------------- */

uint8_t pblp5_color(float v1, float v2, float v3)
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
  convert_black_color(&color);
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

inline int pblp5_red(uint8_t color)
{
#ifdef PBL_COLOR
  convert_black_color(&color);
  return ((color & 0b00110000) >> 4) * 85;
#else
  return color == GColorBlack ? 0 : 255;
#endif
}

inline int pblp5_green(uint8_t color)
{
#ifdef PBL_COLOR
  convert_black_color(&color);
  return ((color & 0b00001100) >> 2) * 85;
#else
  return color == GColorBlack ? 0 : 255;
#endif
}

inline int pblp5_blue(uint8_t color)
{
#ifdef PBL_COLOR
  convert_black_color(&color);
  return (color & 0b00000011) * 85;
#else
  return color == GColorBlack ? 0 : 255;
#endif
}

#ifdef PBL_COLOR
int pblp5_hue(uint8_t color)
{
  convert_black_color(&color);
  int r = ((color & 0b00110000) >> 4) * 85;
  int g = ((color & 0b00001100) >> 2) * 85;
  int b = (color & 0b00000011) * 85;

  int max_val = pblp5_max(r, pblp5_max(g, b));
  int min_val = pblp5_min(r, pblp5_min(g, b));

  float hue;
  if (max_val == min_val) {
    // undefined
    hue = 0;
  }
  else {
    int c = max_val - min_val;
    if (max_val == r) {
      hue = (float)(g - b) / c;
    }
    else if (max_val == g) {
      hue = (float)(b - r) / c + 2;
    }
    else {
      hue = (float)(r - g) / c + 4;
    }
    hue *= 60;
    if (hue < 0) {
      hue += 360;
    }
  }

  // The default range of hue is 0 - 255.
  return (int)(hue * 255 / 360);
}
#else
inline int pblp5_hue(uint8_t color)
{
  return 0;
}
#endif

#ifdef PBL_COLOR
int pblp5_saturation(uint8_t color)
{
  convert_black_color(&color);
  int r = ((color & 0b00110000) >> 4) * 85;
  int g = ((color & 0b00001100) >> 2) * 85;
  int b = (color & 0b00000011) * 85;

  int max_val = pblp5_max(r, pblp5_max(g, b));
  int min_val = pblp5_min(r, pblp5_min(g, b));

  int saturation;
  if (max_val == min_val) {
    saturation = 0;
  }
  else {
    saturation = (float)(max_val - min_val) * 255 / max_val;
  }
  return saturation;
}
#else
inline int pblp5_saturation(uint8_t color)
{
  return 0;
}
#endif

#ifdef PBL_COLOR
int pblp5_brightness(uint8_t color)
{
  convert_black_color(&color);
  int r = ((color & 0b00110000) >> 4) * 85;
  int g = ((color & 0b00001100) >> 2) * 85;
  int b = (color & 0b000011) * 85;

  return pblp5_max(r, pblp5_max(g, b));
}
#else
inline int pblp5_brightness(uint8_t color)
{
  return color == GColorBlack ? 0 : 255;
}
#endif

/* --------------------------
   Image - Loading & Displaying
   -------------------------- */

PImage pblp5_loadImage(uint32_t resource_id)
{
  PImage ret_image;

  // Expand space for img_array if it's full.
  if (img_array_index == img_array_length) {
    GBitmap **tmp = realloc(img_array, sizeof(GBitmap *) * (img_array_length + 5));
    if (tmp == NULL) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Error: failed to realloc() at loadImage()");
      ret_image.index = -1;  // Negative value is the sign that image's invalid.
      return ret_image;
    }
    img_array = tmp;
    img_array_length += 5;
  }

  // Create a bitmap from the resource
  img_array[img_array_index] = gbitmap_create_with_resource(resource_id);
  if (img_array[img_array_index] == NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Error: failed to gbitmap_create_with_resource() at loadImage()");
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Image index: %d", img_array_index);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "The memory may be not enough.");
    ret_image.index = -1;  // Negative value is the sign that image's invalid.
    return ret_image;
  }

  ret_image.index = img_array_index;
  GRect rect = gbitmap_get_bounds(img_array[img_array_index]);
  ret_image.width = rect.size.w;
  ret_image.height = rect.size.h;

  img_array_index++;

  return ret_image;
}

void pblp5_image(PImage img, int x, int y)
{
  if (img.index >= 0) {
#ifdef PBL_COLOR
    if (rotate_angle == 0) {
      x += translate_x;
      y += translate_y;
      graphics_draw_bitmap_in_rect(ctx, img_array[img.index], GRect(x, y, img.width, img.height));
    }
    else {
      convert_transformed_pos(&x, &y);
      graphics_draw_rotated_bitmap(ctx, img_array[img.index], GPoint(0, 0), calculated_rotate_angle, GPoint(x, y));
    }
#else
    // graphics_draw_rotated_bitmap() does not exist on the Aplite.

    convert_transformed_pos(&x, &y);
    graphics_draw_bitmap_in_rect(ctx, img_array[img.index], GRect(x, y, img.width, img.height));
#endif
  }
}

/* --------------------------
   Image - Pixels
   -------------------------- */

uint8_t pblp5_getPixel(int x, int y)
{
  if (g_pblp5_canvas_frame_buffer != NULL) {
    if (0 <= x && x < g_pblp5_width && 0 <= y && y < g_pblp5_height) {
#ifdef PBL_COLOR
      uint8_t color = g_pblp5_raw_pixels[x + y * g_pblp5_row_size_bytes];
      convert_black_color(&color);
      return color;
#else
      uint8_t n = x % 8;
      return (g_pblp5_raw_pixels[x / 8 + y * g_pblp5_row_size_bytes] & (1 << n)) >> n;
#endif
    }
  }
  return 0;
}

void pblp5_loadPixels()
{
  g_pblp5_canvas_frame_buffer = graphics_capture_frame_buffer(ctx);

  if (g_pblp5_canvas_frame_buffer == NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Error: failed to graphics_capture_frame_buffer() at loadPixels()");
    return;
  }

  g_pblp5_raw_pixels = gbitmap_get_data(g_pblp5_canvas_frame_buffer);
  g_pblp5_row_size_bytes = gbitmap_get_bytes_per_row(g_pblp5_canvas_frame_buffer);
}

void pblp5_setPixel(int x, int y, uint8_t color)
{
  if (g_pblp5_canvas_frame_buffer != NULL) {
    if (0 <= x && x < g_pblp5_width && 0 <= y && y < g_pblp5_height) {
#ifdef PBL_COLOR
      convert_black_color(&color);
      g_pblp5_raw_pixels[x + y * g_pblp5_row_size_bytes] = color;
#else
      if (color == 0) {
        g_pblp5_raw_pixels[x / 8 + y * g_pblp5_row_size_bytes] &= ~(1 << (x % 8));
      }
      else {
        g_pblp5_raw_pixels[x / 8 + y * g_pblp5_row_size_bytes] |= 1 << (x % 8);
      }
#endif
    }
  }
}

void pblp5_updatePixels()
{
  if (g_pblp5_canvas_frame_buffer != NULL) {
    graphics_draw_bitmap_in_rect(ctx, g_pblp5_canvas_frame_buffer, gbitmap_get_bounds(g_pblp5_canvas_frame_buffer));

    if (!graphics_release_frame_buffer(ctx, g_pblp5_canvas_frame_buffer)) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Error: failed to graphics_release_frame_buffer() at updatePixels()");
    }
    g_pblp5_canvas_frame_buffer = NULL;
  }
}

/* --------------------------
   Trigonometry
   -------------------------- */

inline float pblp5_sin(float angle)
{
  return (float)sin_lookup((int32_t)(angle * TRIG_MAX_ANGLE / (2 * M_PI))) / TRIG_MAX_RATIO;
}

inline float pblp5_cos(float angle)
{
  return (float)cos_lookup((int32_t)(angle * TRIG_MAX_ANGLE / (2 * M_PI))) / TRIG_MAX_RATIO;
}

inline float pblp5_radians(float degrees)
{
  return degrees * M_PI / 180.0f;
}

inline float pblp5_degrees(float radians)
{
  return radians * 180.0f / M_PI;
}

/* --------------------------
   Random
   -------------------------- */

inline float pblp5_random(float low, float high)
{
  float rnd = (float)rand() / RAND_MAX;
  return rnd * (high - low) + low;
}

inline void pblp5_randomSeed(int seed)
{
  srand(seed);
}

/* --------------------------
   Calculation
   -------------------------- */

float pblp5_constrain(float value, float low, float high)
{
  if (value < low)
    return low;
  else if (value > high)
    return high;

  return value;
}

inline float pblp5_map(float value, float start1, float stop1, float start2, float stop2)
{
  return (value - start1) * (stop2 - start2) / (stop1 - start1) + start2;
}

inline float pblp5_max(float value1, float value2)
{
  return value1 > value2 ? value1 : value2;
}

inline float pblp5_min(float value1, float value2)
{
  return value1 < value2 ? value1 : value2;
}

/* --------------------------
   Typography
   -------------------------- */

inline GFont pblp5_loadFont(uint32_t resource_id)
{
  return fonts_load_custom_font(resource_get_handle(resource_id));
}

inline GFont pblp5_loadSystemFont(const char *font_key)
{
  return fonts_get_system_font(font_key);
}

inline void pblp5_textFont(GFont font)
{
  draw_font = font;
}

void pblp5_text(const char *str, int x, int y)
{
  // Calculate the horizontal alignment by myself.
  if (text_alignment == GTextAlignmentRight) {
    x -= pblp5_textWidth(str);
  }
  else if (text_alignment == GTextAlignmentCenter) {
    x -= pblp5_textWidth(str) / 2;
  }

  convert_transformed_pos(&x, &y);

  GTextAlignment tmp_text_alignment = GTextAlignmentLeft;
  graphics_context_set_text_color(ctx, fill_color);
  graphics_draw_text(ctx, str, draw_font, GRect(x, y, SHRT_MAX, SHRT_MAX), GTextOverflowModeWordWrap, tmp_text_alignment, NULL);
}

void pblp5_textInRect(const char *str, int x, int y, int w, int h)
{
  convert_params_by_rect_mode(&x, &y, &w, &h);

  convert_transformed_pos(&x, &y);

  graphics_context_set_text_color(ctx, fill_color);
  graphics_draw_text(ctx, str, draw_font, GRect(x, y, w, h), GTextOverflowModeWordWrap, text_alignment, NULL);
}

inline void pblp5_textAlign(int alignX)
{
  text_alignment = alignX;
}

inline GSize pblp5_textContentSize(const char *str)
{
  return graphics_text_layout_get_content_size(str, draw_font, GRect(0, 0, SHRT_MAX, SHRT_MAX), GTextOverflowModeWordWrap, text_alignment);
}

inline int pblp5_textHeight(const char *str)
{
  GSize size = graphics_text_layout_get_content_size(str, draw_font, GRect(0, 0, SHRT_MAX, SHRT_MAX), GTextOverflowModeWordWrap, text_alignment);
  return size.h;  
}

inline int pblp5_textWidth(const char *str)
{
  GSize size = graphics_text_layout_get_content_size(str, draw_font, GRect(0, 0, SHRT_MAX, SHRT_MAX), GTextOverflowModeWordWrap, text_alignment);
  return size.w;
}

inline int pblp5_textWidthInRect(const char *str, int w, int h)
{
  GSize size = graphics_text_layout_get_content_size(str, draw_font, GRect(0, 0, w, h), GTextOverflowModeWordWrap, text_alignment);
  return size.w;
}

/* --------------------------
   Functions for use in Pebcessing's internal routine
   -------------------------- */

void pblp5_init_lib()
{
  fill_color = GColorWhite;
  stroke_color = GColorBlack;

#ifdef PBL_COLOR
  stroke_weight = 1;
#endif

  draw_font = fonts_get_system_font(FONT_KEY_FONT_FALLBACK);

  line_path = gpath_create(&LINE_PATH_INFO);
  triangle_path = gpath_create(&TRIANGLE_PATH_INFO);
  quad_path = gpath_create(&QUAD_PATH_INFO);

  time_t sec;
  uint16_t ms;
  time_ms(&sec, &ms);
  pblp5_init_time =  sec * 1000 + ms;

  // Allocate space for bitmaps
  img_array_index = 0;
  img_array_length = 0;
  img_array = calloc(5, sizeof(GBitmap *));
  if (img_array == NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Error: failed to allocate space for GBitmap*");
  }
  else {
    img_array_length = 5;
  }
}

void pblp5_deinit_lib()
{
  for (int i = 0; i < img_array_index; i++) {
    gbitmap_destroy(img_array[i]);
  }
  free(img_array);

  gpath_destroy(quad_path);
  gpath_destroy(triangle_path);
  gpath_destroy(line_path);
}

// Initialize the draw state before drawing the frame.
void pblp5_init_draw_state()
{
  graphics_context_set_fill_color(ctx, fill_color);
  graphics_context_set_stroke_color(ctx, stroke_color);
  
#ifdef PBL_COLOR
  graphics_context_set_stroke_width(ctx, stroke_weight);

  // The GCompOpSet compositing mode is needed for transparency to work.
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
#endif

  translate_x = 0;
  translate_y = 0;
  rotate_angle = 0;
  calculated_rotate_angle = 0;
}

inline void convert_params_by_rect_mode(int *x, int *y, int *w, int *h)
{
  switch (rect_mode) {
    case DRAW_OPTION_CORNER:
      break;
    case DRAW_OPTION_CORNERS:
      *w = *w - *x;
      *h = *h - *y;
      break;
    case DRAW_OPTION_CENTER:
      *x = *x - *w / 2;
      *y = *y - *h / 2;
      break;
    case DRAW_OPTION_RADIUS:
      *x = *x - *w;
      *y = *y - *h;
      *w = 2 * *w;
      *h = 2 * *h;
      break;
  }
}

static void convert_transformed_pos(int *x, int *y)
{
  if (rotate_angle == 0) {
    *x += translate_x;
    *y += translate_y;
  }
  else {
    float c = pblp5_cos(rotate_angle);
    float s = pblp5_sin(rotate_angle);
    int tmp_x = (float)(*x) * c - (float)(*y) * s + translate_x;
    int tmp_y = (float)(*x) * s + (float)(*y) * c + translate_y;
    *x = tmp_x;
    *y = tmp_y;
  }
}

#ifdef PBL_COLOR
// This function is implemented in an "ad hoc manner".
// Convert black color (Swap 0b11000000 <-> 0b00000000)
// The color of fill(0) is black in Processing, but GColorBlack is 0b11000000.
// So convert a value of a color by this function.
static void convert_black_color(uint8_t *color)
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