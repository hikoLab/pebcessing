#include <pebble.h>
#include "pebcessing/pebcessing.h"
#include "settings.h"

static Window *window = NULL;


static void init(void)
{
  window = window_create();

#ifdef SET_FULLSCREEN
  window_set_fullscreen(window, true);
#else
  window_set_fullscreen(window, false);
#endif

  window_stack_push(window, true);
}

static void deinit(void)
{
  window_destroy(window);
}

int main(void)
{
  init();

  init_pebcessing(window, window_get_root_layer(window));

  app_event_loop();

  deinit_pebcessing();

  deinit();

  return 0;
}
