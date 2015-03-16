#pragma once

#include <pebble.h>
#include "pebcessing/pebcessing_core.h"
#include "../settings.h"


extern int g_width;
extern int g_height;
extern unsigned long int g_frame_count;

#ifdef ENABLE_KEY_EVENT
extern ButtonId g_key_code;
#endif

extern GBitmap *g_canvas_frame_buffer;
extern uint8_t *g_raw_pixels;
extern uint16_t g_row_size_bytes;

extern GContext *g_ctx;


// Mathematical constants
#define PI (3.14159265358979323846f)
#define HALF_PI (PI / 2.0f)
#define QUARTER_PI (PI / 4.0f)
#define TWO_PI (2.0f * PI)
#define TAU (2.0f * PI)

// Constants for text alignment or shape draw mode
#define LEFT GTextAlignmentLeft
#define RIGHT GTextAlignmentRight
#define CENTER GTextAlignmentCenter
#define CORNER (CENTER + 0x100)
#define CORNERS (CENTER + 0x101)
#define RADIUS (CENTER + 0x102)

// Constants for key events
#define BACK BUTTON_ID_BACK
#define UP BUTTON_ID_UP
#define SELECT BUTTON_ID_SELECT
#define DOWN BUTTON_ID_DOWN

#ifdef PBL_COLOR

#define BLACK (0b00000000)
#define OXFORDBLUE  GColorOxfordBlueARGB8
#define DUKEBLUE  GColorDukeBlueARGB8
#define BLUE  GColorBlueARGB8
#define DARKGREEN GColorDarkGreenARGB8
#define MIDNIGHTGREEN GColorMidnightGreenARGB8
#define COBALTBLUE  GColorCobaltBlueARGB8
#define BLUEMOON  GColorBlueMoonARGB8
#define ISLAMICGREEN  GColorIslamicGreenARGB8
#define JAEGERGREEN GColorJaegerGreenARGB8
#define TIFFANYBLUE GColorTiffanyBlueARGB8
#define VIVIDCERULEAN GColorVividCeruleanARGB8
#define GREEN GColorGreenARGB8
#define MALACHITE GColorMalachiteARGB8
#define MEDIUMSPRINGGREEN GColorMediumSpringGreenARGB8
#define CYAN  GColorCyanARGB8
#define BULGARIANROSE GColorBulgarianRoseARGB8
#define IMPERIALPURPLE  GColorImperialPurpleARGB8
#define INDIGO  GColorIndigoARGB8
#define ELECTRICULTRAMARINE GColorElectricUltramarineARGB8
#define ARMYGREEN GColorArmyGreenARGB8
#define DARKGRAY  GColorDarkGrayARGB8
#define LIBERTY GColorLibertyARGB8
#define VERYLIGHTBLUE GColorVeryLightBlueARGB8
#define KELLYGREEN  GColorKellyGreenARGB8
#define MAYGREEN  GColorMayGreenARGB8
#define CADETBLUE GColorCadetBlueARGB8
#define PICTONBLUE  GColorPictonBlueARGB8
#define BRIGHTGREEN GColorBrightGreenARGB8
#define SCREAMINGREEN GColorScreaminGreenARGB8
#define MEDIUMAQUAMARINE  GColorMediumAquamarineARGB8
#define ELECTRICBLUE  GColorElectricBlueARGB8
#define DARKCANDYAPPLERED GColorDarkCandyAppleRedARGB8
#define JAZZBERRYJAM  GColorJazzberryJamARGB8
#define PURPLE  GColorPurpleARGB8
#define VIVIDVIOLET GColorVividVioletARGB8
#define WINDSORTAN  GColorWindsorTanARGB8
#define ROSEVALE  GColorRoseValeARGB8
#define PURPUREUS GColorPurpureusARGB8
#define LAVENDERINDIGO  GColorLavenderIndigoARGB8
#define LIMERICK  GColorLimerickARGB8
#define BRASS GColorBrassARGB8
#define LIGHTGRAY GColorLightGrayARGB8
#define BABYBLUEEYES  GColorBabyBlueEyesARGB8
#define SPRINGBUD GColorSpringBudARGB8
#define INCHWORM  GColorInchwormARGB8
#define MINTGREEN GColorMintGreenARGB8
#define CELESTE GColorCelesteARGB8
#define RED GColorRedARGB8
#define FOLLY GColorFollyARGB8
#define FASHIONMAGENTA  GColorFashionMagentaARGB8
#define MAGENTA GColorMagentaARGB8
#define ORANGE  GColorOrangeARGB8
#define SUNSETORANGE  GColorSunsetOrangeARGB8
#define BRILLIANTROSE GColorBrilliantRoseARGB8
#define SHOCKINGPINK  GColorShockingPinkARGB8
#define CHROMEYELLOW  GColorChromeYellowARGB8
#define RAJAH GColorRajahARGB8
#define MELON GColorMelonARGB8
#define RICHBRILLIANTLAVENDER GColorRichBrilliantLavenderARGB8
#define YELLOW  GColorYellowARGB8
#define ICTERINE  GColorIcterineARGB8
#define PASTELYELLOW  GColorPastelYellowARGB8
#define WHITE GColorWhiteARGB8

#else

// Between black and white, which color is closer to the color?
#define BLACK GColorBlack
#define OXFORDBLUE  GColorBlack
#define DUKEBLUE  GColorBlack
#define BLUE  GColorBlack
#define DARKGREEN GColorBlack
#define MIDNIGHTGREEN GColorBlack
#define COBALTBLUE  GColorBlack
#define BLUEMOON  GColorBlack
#define ISLAMICGREEN  GColorBlack
#define JAEGERGREEN GColorBlack
#define TIFFANYBLUE GColorBlack
#define VIVIDCERULEAN GColorWhite
#define GREEN GColorWhite
#define MALACHITE GColorWhite
#define MEDIUMSPRINGGREEN GColorWhite
#define CYAN  GColorWhite
#define BULGARIANROSE GColorBlack
#define IMPERIALPURPLE  GColorBlack
#define INDIGO  GColorBlack
#define ELECTRICULTRAMARINE GColorBlack
#define ARMYGREEN GColorBlack
#define DARKGRAY  GColorBlack
#define LIBERTY GColorBlack
#define VERYLIGHTBLUE GColorBlack
#define KELLYGREEN  GColorBlack
#define MAYGREEN  GColorWhite
#define CADETBLUE GColorWhite
#define PICTONBLUE  GColorWhite
#define BRIGHTGREEN GColorWhite
#define SCREAMINGREEN GColorWhite
#define MEDIUMAQUAMARINE  GColorWhite
#define ELECTRICBLUE  GColorWhite
#define DARKCANDYAPPLERED GColorBlack
#define JAZZBERRYJAM  GColorBlack
#define PURPLE  GColorBlack
#define VIVIDVIOLET GColorBlack
#define WINDSORTAN  GColorBlack
#define ROSEVALE  GColorBlack
#define PURPUREUS GColorBlack
#define LAVENDERINDIGO  GColorWhite
#define LIMERICK  GColorWhite
#define BRASS GColorWhite
#define LIGHTGRAY GColorWhite
#define BABYBLUEEYES  GColorWhite
#define SPRINGBUD GColorWhite
#define INCHWORM  GColorWhite
#define MINTGREEN GColorWhite
#define CELESTE GColorWhite
#define RED GColorBlack
#define FOLLY GColorBlack
#define FASHIONMAGENTA  GColorBlack
#define MAGENTA GColorBlack
#define ORANGE  GColorBlack
#define SUNSETORANGE  GColorWhite
#define BRILLIANTROSE GColorWhite
#define SHOCKINGPINK  GColorWhite
#define CHROMEYELLOW  GColorWhite
#define RAJAH GColorWhite
#define MELON GColorWhite
#define RICHBRILLIANTLAVENDER GColorWhite
#define YELLOW  GColorWhite
#define ICTERINE  GColorWhite
#define PASTELYELLOW  GColorWhite
#define WHITE GColorWhite

#endif



/* --------------------------
   Processing-like functions
   -------------------------- */
void frameRate(float frame_rate);
void loop();
void noLoop();
void redraw();
void size(float w, float h);
void point(float x, float y);
void line(float x1, float y1, float x2, float y2);
void ellipse(float x, float y, float w, float h);
void circle(float x, float y, float radius);
void rect(float x1, float y1, float x2, float y2);
void quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
int year();
int month();
int day();
int wday();
int hour();
int minute();
int second();
long int millis();
uint8_t color(float r, float g, float b);
void background(uint8_t color);
void fill(uint8_t color);
void noFill();
void stroke(uint8_t color);
void noStroke();
uint8_t getPixel(int x, int y);
void loadPixels();
void setPixel(int x, int y, uint8_t color);
void updatePixels();
float constrain(float value, float low, float high);
float map(float value, float start1, float stop1, float start2, float stop2);
float _sin(float angle);
float _cos(float angle);
float radians(float degrees);
float degrees(float radians);
float random(float low, float high);
GFont loadFont(uint32_t resource_id);
GFont loadSystemFont(const char *font_key);
void textFont(GFont font);
void text(const char *str, float x, float y);
void textInRect(const char *str, float x, float y, float w, float h);
void textAlign(int alignX);

/* --------------------------
   Functions for use in Pebcessing's internal routine
   -------------------------- */
void init_pebcessing_lib();
void set_draw_state();
