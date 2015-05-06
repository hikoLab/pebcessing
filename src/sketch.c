#include "sketch.h"
#include "pebcessing/def_for_sketch.h"

/*
  Let's write a sketch!
*/

void setup()
{
  frameRate(5);
  background(255);
  fill(0);
  stroke(255);
}

void draw()
{
  // Draw rectangles at random positions
  float x = random(-20, sketchWidth);
  float y = random(-20, sketchHeight);
  float r = random(10, 30);
  rect(x, y, r, r);
}

/*
// The secondEvent() function is called every second.
// Define ENABLE_SECOND_EVENT in settings.h, if you use this function.
void secondEvent()
{
}
*/

/*
// The minuteEvent() function is called every minute.
// Define ENABLE_MINUTE_EVENT in settings.h, if you use this function.
void minuteEvent()
{
}
*/

/*
// The hourEvent() function is called every hour.
// Define ENABLE_HOUR_EVENT in settings.h, if you use this function.
void hourEvent()
{
}
*/

/*
// The dayEvent() function is called every day.
// Define ENABLE_DAY_EVENT in settings.h, if you use this function.
void dayEvent()
{
}
*/

/*
// The keyPressed() function is called when a button is pressed,
// and the keyReleased() function is called when a button is released.
// Define ENABLE_KEY_EVENT in settings.h, if you use these functions.
// These functions are valid only when this code is built as a watchapp.
void keyPressed()
{
}

void keyReleased()
{
}
*/
