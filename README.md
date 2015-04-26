#Pebcessing

Pebcessing is currently in alpha. The specification is not fixed and there are many incomplete functions because it's under experiment.

##Overview

Pebcessing is a Processing-like framework with which to write simply a code for a [Pebble smartwach](https://getpebble.com/ "Pebble") application. I'm developing Pebcessing especially for non-programmer, designers, artists so that they can create original Pebble watchfaces by programming or can make experiments of visual design for a smart watch. 

Pebcessing is strongly inspired by [Processing](https://processing.org/ "Processing") which is an open source programming language for visual design, new media art, and education. Pebcessing has the function set that is similar to Processing.


##Examples

We can make a Pebble application with below code. Just a simple animation. 

```Processing
#include "sketch.h"
#include "pebcessing/def_for_sketch.h"

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
  float r = random(10, 30);
  rect(x, y, r, r);
}
```
![example1](https://raw.githubusercontent.com/hikoLab/pebcessing/images/images/screenshot_random_rect.png) (*This image is a screenshot of the Pebble emulator.*)

The next example is a digital watch for the Pebble Time. We can create colors with color().

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
  int w = sketchWidth;
  int h = sketchHeight;
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      fill(color(i * 85, j * 85, 255));
      rect(i * w / 4, j * h / 4, w / 4, h / 4);
    }
  }

  // C-style string
  char timeStr[6];
  snprintf(timeStr, 6, "%2d:%02d", hour(), minute());

  fill(WHITE);
  textAlign(CENTER);
  text(timeStr, w / 2, 35);
}

// minuteEvent() is called every minute.
// You have to define ENABLE_MINUTE_EVENT in settings.h to use minuteEvent().
void minuteEvent()
{
  redraw();
}
```
![example2](https://github.com/hikoLab/pebcessing/blob/images/images/screenshot_digital_watch.png)

We can also use HSB colors.

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
      ellipse((float)sketchWidth * i / n + 6, (float)sketchHeight * j / n + 6, 10, 10);
    }
  }
}
```
![example3](https://github.com/hikoLab/pebcessing/blob/images/images/screenshot_hsb_ex.png)

An example of translate(), rotate(). With translate() or rotate(), we can translate or rotate the coordinates. Currently, only primitive shapes are able to rotate. At the Basalt platform, Pebble Time, images are also able to rotate.

```Processing
#include "sketch.h"
#include "pebcessing/def_for_sketch.h"

void setup()
{
  colorMode(HSB);
  noStroke();
  noLoop();
}

void draw()
{
  background(WHITE);

  translate(sketchWidth / 2 + 15, 0);

  for(int i = 0; i < 18; i++){
    fill(color(i * 255 / 18, 255, 255));
    rect(0,0,30,30);
    fill(color(i * 255 / 18, 100, 255));
    rect(0,40,20,20);

    rotate(radians(20));
    translate(30, 0);
  }
}
```
![example4](https://github.com/hikoLab/pebcessing/blob/images/images/screenshot_rotate_ex.png)


##Differences with Processing

Processing is a Java-based programming language, while Pebcessing is C-language. So there are some differences between them. 

For instance, in Pebcessing, we cannot use "class" because C-language is not object oriented, and we have to use "char *" to process a character string, instead of "String".

And Pebcessing contains some functions that are not same as Processing because of the Pebble's specification.

See the reference to know available functions in Pebcessing. 

## How to build

### Build on your local computer

If you've already set up the Pebble SDK 3.0, you can build the downloaded / cloned repository with `pebble build`. And enter either of the following below commands to install it to the emulator or your Pebble watch.
* `pebble install --emulator aplite`
* `pebble install --emulator basalt`
* `pebble install --phone [your IP address]`

### Build on CloudPebble

You can build with [CloudPebble](https://cloudpebble.net/ "CloudPebble") as well. 
The procedure for building apps is as follows:

1. Log in to [CloudPebble](https://cloudpebble.net/ "CloudPebble").
2. Click "IMPORT" button at "ALL YOUR PROJECTS" page.
3. Select "IMPORT FROM GITHUB", and input in the fields as below.
  * PROJECT NAME: arbitrary name
  * GITHUB PROJECT: https://github.com/hikoLab/pebcessing
4. And then, click "IMPORT".

Let's build and run the app on the emulator. And it's possible to install it to your Pebble watch.

## How to use

Basically, the files to be edited are "sketch.c" and "setting.h". 

...

## Reference

[Pebcessing Reference](http://hikolab.github.io/pebcessing/index.html "Pebcessing Reference")

*This reference is in progress.*

## License
Code released under the MIT license. 

Reference released under Creative Commons. It's based on Processing's reference by Processing.org. 


