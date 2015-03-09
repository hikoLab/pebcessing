#pragma once

#include <pebble.h>
#include "pebcessing_lib.h"
#include "../sketch.h"

void init_pebcessing(Window *window, Layer *parent_layer);
void deinit_pebcessing(void);
void set_frame_rate(float frame_rate);
void request_update_canvas();
void enable_loop();
void disable_loop();
