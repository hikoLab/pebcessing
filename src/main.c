#include <pebble.h>
#include "pebcessing/pebcessing.h"


static Window *window = NULL;


static void init(void)
{
  window = window_create();

// Make the window fullscreen.
// All Windows are fullscreen-only on the Basalt platform.
#ifdef PBL_PLATFORM_APLITE
  window_set_fullscreen(window, true);
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
