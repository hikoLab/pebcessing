#include "sketch.h"
#include "pebcessing/def_for_sketch.h"

//  Let's write a sketch!

void setup()
{
  frameRate(5);
  background(WHITE);
  fill(BLACK);
  stroke(WHITE);
}

void draw()
{
  // Draw rectangles at random positions
  float x = random(-20, sketchWidth);
  float y = random(-20, sketchHeight);
  float w = random(10, 30);
  rect(x, y, w, w);
}

// The secondEvent() function is called every second.
// Define ENABLE_SECOND_EVENT in settings.h, if you use this function.
/*
void secondEvent()
{
}
*/

// The minuteEvent() function is called every minute.
// Define ENABLE_MINUTE_EVENT in settings.h, if you use this function.
/*
void minuteEvent()
{
}
*/

// The hourEvent() function is called every hour.
// Define ENABLE_HOUR_EVENT in settings.h, if you use this function.
/*
void hourEvent()
{
}
*/

// The dayEvent() function is called every day.
// Define ENABLE_DAY_EVENT in settings.h, if you use this function.
/*
void dayEvent()
{
}
*/

// The keyPressed() function is called when a button is pressed,
// and the keyReleased() function is called when a button is released.
// Define ENABLE_KEY_EVENT in settings.h, if you use these functions.
// These functions are valid only when this code is built as a watchapp.
/*
void keyPressed()
{
}

void keyReleased()
{
}
*/

// The teardown() function is called when this sketch is about to quit.
// You can clean up something before this sketch quits.
// Define ENABLE_TEAR_DOWN in settings.h, if you use this function.
/*
void teardown()
{
}
*/
