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
  float x = random(0, width);
  float y = random(0, height);
  float r = random(10, 25);
  ellipse(x, y, r, r);
}

/*
void secondEvent()
{
}
*/

/*
void minuteEvent()
{
}
*/

/*
void hourEvent()
{
}
*/

/*
void dayEvent()
{
}
*/

/*
void keyPressed()
{
}

void keyReleased()
{
}
*/
