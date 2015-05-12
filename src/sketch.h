#pragma once

#include <pebble.h>
#include "pebcessing/pebcessing.h"


void setup();
void draw();

#ifdef ENABLE_SECOND_EVENT
void secondEvent();
#endif

#ifdef ENABLE_MINUTE_EVENT
void minuteEvent();
#endif

#ifdef ENABLE_HOUR_EVENT
void hourEvent();
#endif

#ifdef ENABLE_DAY_EVENT
void dayEvent();
#endif

#ifdef ENABLE_KEY_EVENT
void keyPressed();
void keyReleased();
#endif

#ifdef ENABLE_TEAR_DOWN
void teardown();
#endif

