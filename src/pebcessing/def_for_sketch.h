#include "../settings.h"

/* --------------------------
   Alias for compatibility with Processing
   -------------------------- */
#define boolean     bool
#define PFont       GFont


/* --------------------------
   Alias for Pebcessing variables, functions
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
#define sqrt        _sqrt


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
#define BLUE  GColorBlueARGB8
#define DARK_GREEN  GColorDarkGreenARGB8
#define MIDNIGHT_GREEN  GColorMidnightGreenARGB8
#define COBALT_BLUE GColorCobaltBlueARGB8
#define BLUE_MOON GColorBlueMoonARGB8
#define ISLAMIC_GREEN GColorIslamicGreenARGB8
#define JAEGER_GREEN  GColorJaegerGreenARGB8
#define TIFFANY_BLUE  GColorTiffanyBlueARGB8
#define VIVID_CERULEAN  GColorVividCeruleanARGB8
#define GREEN GColorGreenARGB8
#define MALACHITE GColorMalachiteARGB8
#define MEDIUM_SPRING_GREEN GColorMediumSpringGreenARGB8
#define CYAN  GColorCyanARGB8
#define BULGARIAN_ROSE  GColorBulgarianRoseARGB8
#define IMPERIAL_PURPLE GColorImperialPurpleARGB8
#define INDIGO  GColorIndigoARGB8
#define ELECTRIC_ULTRAMARINE  GColorElectricUltramarineARGB8
#define ARMY_GREEN  GColorArmyGreenARGB8
#define DARK_GRAY GColorDarkGrayARGB8
#define LIBERTY GColorLibertyARGB8
#define VERY_LIGHT_BLUE GColorVeryLightBlueARGB8
#define KELLY_GREEN GColorKellyGreenARGB8
#define MAY_GREEN GColorMayGreenARGB8
#define CADET_BLUE  GColorCadetBlueARGB8
#define PICTON_BLUE GColorPictonBlueARGB8
#define BRIGHT_GREEN  GColorBrightGreenARGB8
#define SCREAMIN_GREEN  GColorScreaminGreenARGB8
#define MEDIUM_AQUAMARINE GColorMediumAquamarineARGB8
#define ELECTRIC_BLUE GColorElectricBlueARGB8
#define DARK_CANDY_APPLE_RED  GColorDarkCandyAppleRedARGB8
#define JAZZBERRY_JAM GColorJazzberryJamARGB8
#define PURPLE  GColorPurpleARGB8
#define VIVID_VIOLET  GColorVividVioletARGB8
#define WINDSOR_TAN GColorWindsorTanARGB8
#define ROSE_VALE GColorRoseValeARGB8
#define PURPUREUS GColorPurpureusARGB8
#define LAVENDER_INDIGO GColorLavenderIndigoARGB8
#define LIMERICK  GColorLimerickARGB8
#define BRASS GColorBrassARGB8
#define LIGHT_GRAY  GColorLightGrayARGB8
#define BABY_BLUE_EYES  GColorBabyBlueEyesARGB8
#define SPRING_BUD  GColorSpringBudARGB8
#define INCHWORM  GColorInchwormARGB8
#define MINT_GREEN  GColorMintGreenARGB8
#define CELESTE GColorCelesteARGB8
#define RED GColorRedARGB8
#define FOLLY GColorFollyARGB8
#define FASHION_MAGENTA GColorFashionMagentaARGB8
#define MAGENTA GColorMagentaARGB8
#define ORANGE  GColorOrangeARGB8
#define SUNSET_ORANGE GColorSunsetOrangeARGB8
#define BRILLIANT_ROSE  GColorBrilliantRoseARGB8
#define SHOCKING_PINK GColorShockingPinkARGB8
#define CHROME_YELLOW GColorChromeYellowARGB8
#define RAJAH GColorRajahARGB8
#define MELON GColorMelonARGB8
#define RICH_BRILLIANT_LAVENDER GColorRichBrilliantLavenderARGB8
#define YELLOW  GColorYellowARGB8
#define ICTERINE  GColorIcterineARGB8
#define PASTEL_YELLOW GColorPastelYellowARGB8
#define WHITE GColorWhiteARGB8
#else
// Constants of colors for Aplite
// Between black and white, which color is closer to the color?
#define BLACK GColorBlack
#define OXFORD_BLUE GColorBlack
#define DUKE_BLUE GColorBlack
#define BLUE  GColorBlack
#define DARK_GREEN  GColorBlack
#define MIDNIGHT_GREEN  GColorBlack
#define COBALT_BLUE GColorBlack
#define BLUE_MOON GColorBlack
#define ISLAMIC_GREEN GColorBlack
#define JAEGER_GREEN  GColorBlack
#define TIFFANY_BLUE  GColorBlack
#define VIVID_CERULEAN  GColorWhite
#define GREEN GColorWhite
#define MALACHITE GColorWhite
#define MEDIUM_SPRING_GREEN GColorWhite
#define CYAN  GColorWhite
#define BULGARIAN_ROSE  GColorBlack
#define IMPERIAL_PURPLE GColorBlack
#define INDIGO  GColorBlack
#define ELECTRIC_ULTRAMARINE  GColorBlack
#define ARMY_GREEN  GColorBlack
#define DARK_GRAY GColorBlack
#define LIBERTY GColorBlack
#define VERY_LIGHT_BLUE GColorBlack
#define KELLY_GREEN GColorBlack
#define MAY_GREEN GColorWhite
#define CADET_BLUE  GColorWhite
#define PICTON_BLUE GColorWhite
#define BRIGHT_GREEN  GColorWhite
#define SCREAMIN_GREEN  GColorWhite
#define MEDIUM_AQUAMARINE GColorWhite
#define ELECTRIC_BLUE GColorWhite
#define DARK_CANDY_APPLE_RED  GColorBlack
#define JAZZBERRY_JAM GColorBlack
#define PURPLE  GColorBlack
#define VIVID_VIOLET  GColorBlack
#define WINDSOR_TAN GColorBlack
#define ROSE_VALE GColorBlack
#define PURPUREUS GColorBlack
#define LAVENDER_INDIGO GColorWhite
#define LIMERICK  GColorWhite
#define BRASS GColorWhite
#define LIGHT_GRAY  GColorWhite
#define BABY_BLUE_EYES  GColorWhite
#define SPRING_BUD  GColorWhite
#define INCHWORM  GColorWhite
#define MINT_GREEN  GColorWhite
#define CELESTE GColorWhite
#define RED GColorBlack
#define FOLLY GColorBlack
#define FASHION_MAGENTA GColorBlack
#define MAGENTA GColorBlack
#define ORANGE  GColorBlack
#define SUNSET_ORANGE GColorWhite
#define BRILLIANT_ROSE  GColorWhite
#define SHOCKING_PINK GColorWhite
#define CHROME_YELLOW GColorWhite
#define RAJAH GColorWhite
#define MELON GColorWhite
#define RICH_BRILLIANT_LAVENDER GColorWhite
#define YELLOW  GColorWhite
#define ICTERINE  GColorWhite
#define PASTEL_YELLOW GColorWhite
#define WHITE GColorWhite
#endif

// Constants for colorMode()
#define RGB COLOR_MODE_RGB
#define HSB COLOR_MODE_HSB

