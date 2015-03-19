#pragma once

#include <pebble.h>
#include "pebcessing_lib.h"
#include "../sketch.h"

void init_pebcessing(Window *window, Layer *parent_layer);
void deinit_pebcessing(void);

void pblp5_set_frame_rate(float frame_rate);
void pblp5_request_update_canvas();
void pblp5_enable_loop();
void pblp5_disable_loop();
