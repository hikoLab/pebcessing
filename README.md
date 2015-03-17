#Pebcessing

Pebcessing is currently in alpha. The specification is not fixed and there are many incomplete functions because it's under experiment.

##Overview

Pebcessing is a Processing-like framework with which to write simply a code for a [Pebble smartwach](https://getpebble.com/ "Pebble") application. This is a wrapper of Pebble SDK 3.0.  I'm developing Pebcessing especially for non-programmer, designers, artists so that they can create original Pebble watchfaces by programming or can make experiments of visual design for a smart watch. 

Pebcessing is inspired by [Processing](https://processing.org/ "Processing") which is an open source programming language for visual design, new media art, and education. 


##Examples

We can make a Pebble application with below code. Just a simple animation. 

```Processing
#include "sketch.h"
#include "pebcessing/def_for_sketch.h"

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
  float x = random(-20, width);
  float y = random(-20, height);
  float r = random(10, 30);
  rect(x, y, r, r);
}
```
![example1](https://raw.githubusercontent.com/hikoLab/pebcessing/images/images/screenshot_random_rect.png)

The next example is a digital watch for the Pebble Time. We can use colors easily in Pebcessing. 

```Processing
#include "sketch.h"
#include "pebcessing/def_for_sketch.h"

void setup()
{
  PFont font = loadSystemFont(FONT_KEY_BITHAM_42_LIGHT); // Use system font
  textFont(font);
  noStroke();
  noLoop();
}

void draw()
{
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      fill(color(i * 85, j * 85, 255));
      rect(i * width / 4, j * height / 4, width / 4, height / 4);
    }
  }

  // C-style string
  char timeStr[6];
  snprintf(timeStr, 6, "%d:%d", hour(), minute());

  fill(255);
  text(timeStr, 22, 30);
}

// minuteEvent() is called every minute.
// You have to define ENABLE_MINUTE_EVENT in settings.h to use minuteEvent().
void minuteEvent()
{
  redraw();
}
```
![example2](https://github.com/hikoLab/pebcessing/blob/images/images/screenshot_digital_watch.png)

And we can also use HSB colors.

```Processing
#include "sketch.h"
#include "pebcessing/def_for_sketch.h"

void setup()
{
  colorMode(HSB); // Use HSB color mode
  noStroke();
  noLoop();
}

void draw()
{
  int n = 10;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      // Create a color from Hue, Saturation, Brightness
      int cl = color(255.0 * i / n, 255.0 - 255.0 * j / n, 255.0); 
      fill(cl);
      ellipse((float)width * i / n + 6, (float)height * j / n + 6, 5, 5);
    }
  }
}
```
![example3](https://github.com/hikoLab/pebcessing/blob/images/images/screenshot_hsb_ex.png)


##Differences with Processing

Processing is a Java-based programming language, while Pebcessing is C-language. So there are some differences between them. 

For instance, we cannot use "class" because C-language is not object oriented, and we have to use "char *" to process a character string, instead of "String".

And Pebcessing contains some functions that are not same as Processing because of the Pebble's specification.

See the reference to know available functions in Pebcessing. (*The reference is being prepared. Coming soon.*)

## How to build

If you've already set up the Pebble SDK 3.0, you can build this repository with `pebble build`. 

We can build with [CloudPebble](https://cloudpebble.net/ "CloudPebble") as well. 
The procedure for building apps is as follows:

1. Log in to [CloudPebble](https://cloudpebble.net/ "CloudPebble").
2. Click "IMPORT" button at "ALL YOUR PROJECTS" page.
3. Select "IMPORT FROM GITHUB", and input in the fields as below.
  * PROJECT NAME: arbitrary name
  * GITHUB PROJECT: https://github.com/hikoLab/pebcessing
4. And then, click "IMPORT".

Let's build and run the app on the emulator. 

## How to use

Basically, the files to be edited are "sketch.c" and "setting.h". 

...
