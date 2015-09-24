#include "pebcessing_core.h"

const int DEFAULT_FRAME_RATE = 1;

static Layer *canvas_layer = NULL;
static AppTimer *update_canvas_timer = NULL;
static float frame_rate = -1;
static bool loop_flag = true;

#ifdef ENABLE_KEEP_PREVIOUS_FRAME
static GBitmap *pre_frame_buffer = NULL;
#endif


#ifdef ENABLE_KEY_EVENT

static void button_pressed_handler(ClickRecognizerRef recognizer, void *context)
{
  // Window *window = (Window *)context;
  ButtonId id = click_recognizer_get_button_id(recognizer);
  g_pblp5_key_code = id;
  keyPressed();
}

static void button_released_handler(ClickRecognizerRef recognizer, void *context)
{
  // Window *window = (Window *)context;
  ButtonId id = click_recognizer_get_button_id(recognizer);
  g_pblp5_key_code = id;
  keyReleased();
}

static void click_config_provider(Window *window)
{
  window_raw_click_subscribe(BUTTON_ID_BACK, button_pressed_handler, button_released_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_UP, button_pressed_handler, button_released_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_SELECT, button_pressed_handler, button_released_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_DOWN, button_pressed_handler, button_released_handler, NULL);
}

#endif


static void update_canvas(Layer *layer, GContext *ctx)
{
  g_pblp5_context = ctx;

#ifdef ENABLE_KEEP_PREVIOUS_FRAME
  // Draw the previous frame buffer.
  if (pre_frame_buffer != NULL) {
    graphics_draw_bitmap_in_rect(ctx, pre_frame_buffer, gbitmap_get_bounds(pre_frame_buffer));
  }
#endif

  // Initialize the draw state.
  pblp5_init_draw_state();

  if (g_pblp5_frame_count == 0) {
    // At the time of fisrt updating canvas, process the sketch's setup().
    setup();

    if (frame_rate == -1) {
      pblp5_set_frame_rate(DEFAULT_FRAME_RATE);
    }

    g_pblp5_frame_count = 1;
  }

  // Process the sketch's draw().
  draw();

  g_pblp5_frame_count++;


#ifdef ENABLE_KEEP_PREVIOUS_FRAME
  // Copy the frame buffer to pre_frame_buffer.
  GBitmap *fb = graphics_capture_frame_buffer(ctx);

  if (fb == NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Error: graphics_capture_frame_buffer()");
    return;
  }

  if (pre_frame_buffer != NULL) {
    gbitmap_destroy(pre_frame_buffer);
  }

  GRect fb_bounds = gbitmap_get_bounds(fb);
#ifdef PBL_PLATFORM_APLITE
  pre_frame_buffer = __gbitmap_create_blank(GSize(fb_bounds.size.w, fb_bounds.size.h));
#elif PBL_PLATFORM_BASALT
  pre_frame_buffer = gbitmap_create_blank(GSize(fb_bounds.size.w, fb_bounds.size.h), GBitmapFormat8Bit);
#endif
  if (pre_frame_buffer == NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Error: gbitmap_create_blank()");
    return;
  }

  memcpy(gbitmap_get_data(pre_frame_buffer), gbitmap_get_data(fb), fb_bounds.size.h * gbitmap_get_bytes_per_row(fb));

  if (!graphics_release_frame_buffer(ctx, fb)) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Error: graphics_release_frame_buffer()");
    return;
  }

#endif
}

static void update_canvas_timer_callback(void *data)
{
  layer_mark_dirty(canvas_layer);

  if (loop_flag && frame_rate > 0) {
    update_canvas_timer = app_timer_register(1000 / frame_rate, update_canvas_timer_callback, NULL);
  }
}

#if defined(ENABLE_SECOND_EVENT) || defined(ENABLE_MINUTE_EVENT) || defined(ENABLE_HOUR_EVENT) || defined(ENABLE_DAY_EVENT)
static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
#ifdef ENABLE_DAY_EVENT
  if (units_changed & DAY_UNIT) {
    dayEvent();
  }
#endif

#ifdef ENABLE_HOUR_EVENT
  if (units_changed & HOUR_UNIT) {
    hourEvent();
  }
#endif

#ifdef ENABLE_MINUTE_EVENT
  if (units_changed & MINUTE_UNIT) {
    minuteEvent();
  }
#endif

#ifdef ENABLE_SECOND_EVENT
  if (units_changed & SECOND_UNIT) {
    secondEvent();
  }
#endif
}
#endif

void init_pebcessing(Window *window, Layer *parent_layer)
{
  GRect bounds = layer_get_bounds(parent_layer);
  canvas_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_add_child(parent_layer, canvas_layer);

  layer_set_update_proc(canvas_layer, update_canvas);

  g_pblp5_width = bounds.size.w;
  g_pblp5_height = bounds.size.h;

  pblp5_init_lib();

#ifdef ENABLE_KEY_EVENT
  window_set_click_config_provider(window, (ClickConfigProvider)click_config_provider);
#endif

#if defined(ENABLE_SECOND_EVENT) || defined(ENABLE_MINUTE_EVENT) || defined(ENABLE_HOUR_EVENT) || defined(ENABLE_DAY_EVENT)

  TimeUnits tick_units = 0;

#ifdef ENABLE_SECOND_EVENT
  tick_units |= SECOND_UNIT;
#endif

#ifdef ENABLE_MINUTE_EVENT
  tick_units |= MINUTE_UNIT;
#endif

#ifdef ENABLE_HOUR_EVENT
  tick_units |= HOUR_UNIT;
#endif

#ifdef ENABLE_DAY_EVENT
  tick_units |= DAY_UNIT;
#endif

  tick_timer_service_subscribe(tick_units, tick_handler);

#endif

  layer_mark_dirty(canvas_layer);
}

void deinit_pebcessing(void)
{
#ifdef ENABLE_TEAR_DOWN
  teardown();
#endif

  pblp5_deinit_lib();

  if (g_pblp5_canvas_frame_buffer != NULL) {
    gbitmap_destroy(g_pblp5_canvas_frame_buffer);
  }

#if defined(ENABLE_SECOND_EVENT) || defined(ENABLE_MINUTE_EVENT) || defined(ENABLE_HOUR_EVENT) || defined(ENABLE_DAY_EVENT)  
  tick_timer_service_unsubscribe();
#endif

#ifdef ENABLE_KEEP_PREVIOUS_FRAME
  if (pre_frame_buffer != NULL) {
    gbitmap_destroy(pre_frame_buffer);
  }
#endif
  
  layer_destroy(canvas_layer);
}

void pblp5_set_frame_rate(float _frame_rate)
{
  float pre_frame_rate = frame_rate;
  frame_rate = _frame_rate;

  if (frame_rate < 0) {
    frame_rate = 0;
  }

  if (loop_flag && frame_rate > 0 && frame_rate != pre_frame_rate) {
    if (update_canvas_timer != NULL) {
      app_timer_cancel(update_canvas_timer);
      update_canvas_timer = NULL;
    }

    update_canvas_timer = app_timer_register(1000 / frame_rate, update_canvas_timer_callback, NULL);
  }
}

void pblp5_request_update_canvas()
{
  layer_mark_dirty(canvas_layer);
}

void pblp5_enable_loop()
{
  if (!loop_flag && frame_rate > 0 && update_canvas_timer == NULL) {
    update_canvas_timer = app_timer_register(1000 / frame_rate, update_canvas_timer_callback, NULL);
  }

  loop_flag = true;
}

void pblp5_disable_loop()
{
  if (update_canvas_timer != NULL) {
    app_timer_cancel(update_canvas_timer);
    update_canvas_timer = NULL;
  }

  loop_flag = false;
}
