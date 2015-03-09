#include "../settings.h"

/* --------------------------
   Alias for compatibility with Processing
   -------------------------- */
#define boolean     bool
#define PFont       GFont

/* --------------------------
   Alias for Pebcessing variables/functions
   -------------------------- */
#define width       g_width
#define height      g_height
#define frameCount  g_frame_count

#ifdef ENABLE_KEY_EVENT
#define keyCode     g_key_code
#endif

// To avoid to conflict with the compiler's built-in functions
#define sin         _sin
#define cos         _cos
