#include "../settings.h"

/* --------------------------
   Alias for compatibility with Processing
   -------------------------- */
typedef bool        boolean;
typedef GFont       PFont;


/* --------------------------
   Alias for Pebcessing variables, functions
   -------------------------- */
#define sketchWidth       g_pblp5_width
#define sketchHeight      g_pblp5_height
#define frameCount  g_pblp5_frame_count

#ifdef ENABLE_KEY_EVENT
#define keyCode     g_pblp5_key_code
#endif


#define frameRate(x)          pblp5_frameRate(x)
#define loop()                pblp5_loop()
#define noLoop()              pblp5_noLoop()
#define redraw()              pblp5_redraw();
#define size(w, h)            pblp5_size(w, h)
#define point(x, y)           pblp5_point(x, y)
#define line(x1, y1, x2, y2)  pblp5_line(x1, y1, x2, y2)
#define ellipse(x, y, w, h)   pblp5_ellipse(x, y, w, h)
#define circle(x, y, w)       pblp5_circle(x, y, w)
#define rect(a, b, c, d)      pblp5_rect(a, b, c, d)
#define triangle(x1, y1, x2, y2, x3, y3)  pblp5_triangle(x1, y1, x2, y2, x3, y3)
#define ellipseMode(mode)     pblp5_ellipseMode(mode)
#define rectMode(mode)        pblp5_rectMode(mode)
#define noSmooth()            pblp5_noSmooth()
#define smooth()              pblp5_smooth()
#define strokeWeight(weight)  pblp5_strokeWeight(weight)
#define year()                pblp5_year()
#define month()               pblp5_month()
#define day()                 pblp5_day()
#define wday()                pblp5_wday()
#define hour()                pblp5_hour()
#define minute()              pblp5_minute()
#define second()              pblp5_second()
#define millis()              pblp5_millis()
#define popMatrix()           pblp5_popMatrix()
#define pushMatrix()          pblp5_pushMatrix()
#define resetMatrix()         pblp5_resetMatrix()   
#define rotate(angle)         pblp5_rotate(angle)
#define translate(x, y)       pblp5_translate(x, y)
#define background(color)     pblp5_background(color)
#define colorMode(mode)       pblp5_colorMode(mode)
#define fill(color)           pblp5_fill(color)
#define noFill()              pblp5_noFill()
#define stroke(color)         pblp5_stroke(color)
#define noStroke()            pblp5_noStroke()
#define color(r, g, b)        pblp5_color(r, g, b)
#define red(color)            pblp5_red(color)
#define green(color)          pblp5_green(color)
#define blue(color)           pblp5_blue(color)
#define hue(color)            pblp5_hue(color)
#define saturation(color)     pblp5_saturation(color)
#define brightness(color)     pblp5_brightness(color)
#define loadImage(resource_id)  pblp5_loadImage(resource_id)
#define image(img, x, y)      pblp5_image(img, x, y)
#define getPixel(x, y)        pblp5_getPixel(x, y)
#define loadPixels()          pblp5_loadPixels()
#define setPixel(x, y, color) pblp5_setPixel(x, y, color)
#define updatePixels()        pblp5_updatePixels()
#define constrain(value, low, high)  pblp5_constrain(value, low, high)
#define map(value, start1, stop1, start2, stop2)  pblp5_map(value, start1, stop1, start2, stop2)
#define max(value1, value2)   pblp5_max(value1, value2)
#define min(value1, value2)   pblp5_min(value1, value2)
#define sin(angle)            pblp5_sin(angle)
#define cos(angle)            pblp5_cos(angle)
#define radians(degrees)      pblp5_radians(degrees)
#define degrees(radians)      pblp5_degrees(radians)
#define random(low, high)     pblp5_random(low, high)
#define randomSeed(seed)      pblp5_randomSeed(seed)
#define loadFont(resource_id) pblp5_loadFont(resource_id)
#define loadSystemFont(font_key)  pblp5_loadSystemFont(font_key)
#define textFont(font)        pblp5_textFont(font)
#define text(str, x, y)       pblp5_text(str, x, y)
#define textInRect(str, x, y, w, h)  pblp5_textInRect(str, x, y, w, h)
#define textAlign(alignX)     pblp5_textAlign(alignX)
#define textContentSize(str)  pblp5_textContentSize(str)
#define textHeight(str)       pblp5_textHeight(str)
#define textWidth(str)        pblp5_textWidth(str)
#define textWidthInRect(str, w, h)  pblp5_textWidthInRect(str, w, h)
/*
  If quad macro is defined, the error "quad redefined" occurs because quad macro is already defined at "arm-cs-tools/arm-none-eabi/include/sys/types.h".
  So pblp5_quad() is defined directly as quad() if quad macro is already defined.
  It's not good code...
 */
#ifndef quad
#define quad(x1, y1, x2, y2, x3, y3, x4, y4)  pblp5_quad(x1, y1, x2, y2, x3, y3, x4, y4)
#endif


/* --------------------------
   Constants
   -------------------------- */

// Mathematical constants
#define PI (M_PI)
#define HALF_PI (M_PI / 2.0f)
#define QUARTER_PI (M_PI / 4.0f)
#define TWO_PI (2.0f * M_PI)
#define TAU (2.0f * M_PI)

// Constants for text alignment or shape draw mode
#define LEFT DRAW_OPTION_LEFT
#define RIGHT DRAW_OPTION_RIGHT
#define CENTER DRAW_OPTION_CENTER
#define CORNER DRAW_OPTION_CORNER
#define CORNERS DRAW_OPTION_CORNERS
#define RADIUS DRAW_OPTION_RADIUS


// Constants for key events
#ifdef ENABLE_KEY_EVENT
#define BACK BUTTON_ID_BACK
#define UP BUTTON_ID_UP
#define SELECT BUTTON_ID_SELECT
#define DOWN BUTTON_ID_DOWN
#endif

// Constants of colors
#ifdef PBL_COLOR
#define BLACK (0b00000000)
#define OXFORD_BLUE GColorOxfordBlueARGB8
#define DUKE_BLUE GColorDukeBlueARGB8
#define BLUE GColorBlueARGB8
#define DARK_GREEN GColorDarkGreenARGB8
#define MIDNIGHT_GREEN GColorMidnightGreenARGB8
#define COBALT_BLUE GColorCobaltBlueARGB8
#define BLUE_MOON GColorBlueMoonARGB8
#define ISLAMIC_GREEN GColorIslamicGreenARGB8
#define JAEGER_GREEN GColorJaegerGreenARGB8
#define TIFFANY_BLUE GColorTiffanyBlueARGB8
#define VIVID_CERULEAN GColorVividCeruleanARGB8
#define GREEN GColorGreenARGB8
#define MALACHITE GColorMalachiteARGB8
#define MEDIUM_SPRING_GREEN GColorMediumSpringGreenARGB8
#define CYAN GColorCyanARGB8
#define BULGARIAN_ROSE GColorBulgarianRoseARGB8
#define IMPERIAL_PURPLE GColorImperialPurpleARGB8
#define INDIGO GColorIndigoARGB8
#define ELECTRIC_ULTRAMARINE GColorElectricUltramarineARGB8
#define ARMY_GREEN GColorArmyGreenARGB8
#define DARK_GRAY GColorDarkGrayARGB8
#define LIBERTY GColorLibertyARGB8
#define VERY_LIGHT_BLUE GColorVeryLightBlueARGB8
#define KELLY_GREEN GColorKellyGreenARGB8
#define MAY_GREEN GColorMayGreenARGB8
#define CADET_BLUE GColorCadetBlueARGB8
#define PICTON_BLUE GColorPictonBlueARGB8
#define BRIGHT_GREEN GColorBrightGreenARGB8
#define SCREAMIN_GREEN GColorScreaminGreenARGB8
#define MEDIUM_AQUAMARINE GColorMediumAquamarineARGB8
#define ELECTRIC_BLUE GColorElectricBlueARGB8
#define DARK_CANDY_APPLE_RED GColorDarkCandyAppleRedARGB8
#define JAZZBERRY_JAM GColorJazzberryJamARGB8
#define PURPLE GColorPurpleARGB8
#define VIVID_VIOLET GColorVividVioletARGB8
#define WINDSOR_TAN GColorWindsorTanARGB8
#define ROSE_VALE GColorRoseValeARGB8
#define PURPUREUS GColorPurpureusARGB8
#define LAVENDER_INDIGO GColorLavenderIndigoARGB8
#define LIMERICK GColorLimerickARGB8
#define BRASS GColorBrassARGB8
#define LIGHT_GRAY GColorLightGrayARGB8
#define BABY_BLUE_EYES GColorBabyBlueEyesARGB8
#define SPRING_BUD GColorSpringBudARGB8
#define INCHWORM GColorInchwormARGB8
#define MINT_GREEN GColorMintGreenARGB8
#define CELESTE GColorCelesteARGB8
#define RED GColorRedARGB8
#define FOLLY GColorFollyARGB8
#define FASHION_MAGENTA GColorFashionMagentaARGB8
#define MAGENTA GColorMagentaARGB8
#define ORANGE GColorOrangeARGB8
#define SUNSET_ORANGE GColorSunsetOrangeARGB8
#define BRILLIANT_ROSE GColorBrilliantRoseARGB8
#define SHOCKING_PINK GColorShockingPinkARGB8
#define CHROME_YELLOW GColorChromeYellowARGB8
#define RAJAH GColorRajahARGB8
#define MELON GColorMelonARGB8
#define RICH_BRILLIANT_LAVENDER GColorRichBrilliantLavenderARGB8
#define YELLOW GColorYellowARGB8
#define ICTERINE GColorIcterineARGB8
#define PASTEL_YELLOW GColorPastelYellowARGB8
#define WHITE GColorWhiteARGB8
#else
// Constants of colors for Aplite
// Between black and white, which color is closer to the color?
#define BLACK GColorBlack
#define OXFORD_BLUE GColorBlack
#define DUKE_BLUE GColorBlack
#define BLUE GColorBlack
#define DARK_GREEN GColorBlack
#define MIDNIGHT_GREEN GColorBlack
#define COBALT_BLUE GColorBlack
#define BLUE_MOON GColorBlack
#define ISLAMIC_GREEN GColorBlack
#define JAEGER_GREEN GColorBlack
#define TIFFANY_BLUE GColorBlack
#define VIVID_CERULEAN GColorWhite
#define GREEN GColorWhite
#define MALACHITE GColorWhite
#define MEDIUM_SPRING_GREEN GColorWhite
#define CYAN GColorWhite
#define BULGARIAN_ROSE GColorBlack
#define IMPERIAL_PURPLE GColorBlack
#define INDIGO GColorBlack
#define ELECTRIC_ULTRAMARINE GColorBlack
#define ARMY_GREEN GColorBlack
#define DARK_GRAY GColorBlack
#define LIBERTY GColorBlack
#define VERY_LIGHT_BLUE GColorBlack
#define KELLY_GREEN GColorBlack
#define MAY_GREEN GColorWhite
#define CADET_BLUE GColorWhite
#define PICTON_BLUE GColorWhite
#define BRIGHT_GREEN GColorWhite
#define SCREAMIN_GREEN GColorWhite
#define MEDIUM_AQUAMARINE GColorWhite
#define ELECTRIC_BLUE GColorWhite
#define DARK_CANDY_APPLE_RED GColorBlack
#define JAZZBERRY_JAM GColorBlack
#define PURPLE GColorBlack
#define VIVID_VIOLET GColorBlack
#define WINDSOR_TAN GColorBlack
#define ROSE_VALE GColorBlack
#define PURPUREUS GColorBlack
#define LAVENDER_INDIGO GColorWhite
#define LIMERICK GColorWhite
#define BRASS GColorWhite
#define LIGHT_GRAY GColorWhite
#define BABY_BLUE_EYES GColorWhite
#define SPRING_BUD GColorWhite
#define INCHWORM GColorWhite
#define MINT_GREEN GColorWhite
#define CELESTE GColorWhite
#define RED GColorBlack
#define FOLLY GColorBlack
#define FASHION_MAGENTA GColorBlack
#define MAGENTA GColorBlack
#define ORANGE GColorBlack
#define SUNSET_ORANGE GColorWhite
#define BRILLIANT_ROSE GColorWhite
#define SHOCKING_PINK GColorWhite
#define CHROME_YELLOW GColorWhite
#define RAJAH GColorWhite
#define MELON GColorWhite
#define RICH_BRILLIANT_LAVENDER GColorWhite
#define YELLOW GColorWhite
#define ICTERINE GColorWhite
#define PASTEL_YELLOW GColorWhite
#define WHITE GColorWhite
#endif

// Constants for colorMode()
#define RGB COLOR_MODE_RGB
#define HSB COLOR_MODE_HSB
