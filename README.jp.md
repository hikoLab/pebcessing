#Pebcessing

Pebcessingは現在α版です。まだ実験中なので、仕様はfixしておらず、多くの未完成の機能があります。

##概要

Pebcessingは、[Pebble smartwach](https://getpebble.com/ "Pebble") のアプリをシンプルにコーディングできるProcessingライクなフレームワークです。
特にノンプログラマーやデザイナー、アーティストといった方が、プログラミングでオリジナルのPebbleのwatchfaceを作ったり、スマートウォッチのためのヴィジュアルデザインの実験ができるようにPebcessingは開発されています。

Pebcessingは、ヴィジュアルデザインや新しいメディアアート、教育といった分野で使われている、オープンソースのプログラミング言語[Processing](https://processing.org/ "Processing")の影響を強く受けており、Processingに類似した命令を使うことができます。


##例

例えば、以下のコードでPebbleアプリを作ることができます。実行するとシンプルなアニメーションが描画されます。

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
![example1](https://raw.githubusercontent.com/hikoLab/pebcessing/images/images/screenshot_random_rect.png) (*この画像は、Pebbleエミュレータのスクリーンショットです。*)

次の例は、Pebble Timeのためのデジタル時計です。color()で色を作ることができます。

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

HSBカラーを使うこともできます。

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

translate()やrotate()を使った例です。translate()やrotate()を使うと、座標を移動したり回転させることができます。現在、基本的な図形だけが回転できます。Basaltプラットフォーム(Pebble Time)では、画像も回転できます。

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


## Processingとの違い

ProcessingはJavaベースのプログライング言語ですが、一方、PebcessingはC言語です。そのため、両者にはいくつか差異があります。

例えば、Pebcessingでは、C言語はオブジェクト指向の言語でないので"class"を使うことができず、また、文字列の処理のために"String"の代わりに"char *"を使う必要があります。

また、Pebbleの仕様のために、Processingとは同じではない関数があります。

Pebcessingで利用できる関数については[リファレンス(英語)](http://hikolab.github.io/pebcessing/index.html "Pebcessing Reference")を参照してください。

## ビルド方法

### ローカルでビルドする場合

もし、既にPebble SDK 3.0をセットアップしているのであれば、このリポジトリをダウンロード/クローンして、`pebble build`を実行することでビルドできます。ビルド後、エミュレータかPebbleの実機にインストールするためには、以下のいずれかのコマンドを実行してください。
* `pebble install --emulator aplite`
* `pebble install --emulator basalt`
* `pebble install --phone [your IP address]`

### CloudPebbleでビルドする場合

[CloudPebble](https://cloudpebble.net/ "CloudPebble")でもビルドできます。アプリをビルドするための手順は、以下の通りです。

1. [CloudPebble](https://cloudpebble.net/ "CloudPebble")にログインする。
2. "ALL YOUR PROJECTS"ページにある"IMPORT"ボタンをクリック。
3. "IMPORT FROM GITHUB"を選択し、以下のように項目を入力する。
  * PROJECT NAME: (任意のプロジェクト名)
  * GITHUB PROJECT: https://github.com/hikoLab/pebcessing
4. "IMPORT"をクリック。

ビルドして、エミュレータ上でアプリを実行してみましょう。Pebbleの実機にアプリをインストールすることも可能です。

## 使い方

基本的に、編集するファイルは、"sketch.c"と"setting.h"の2つのファイルです。

(続く)

## リファレンス

[Pebcessingリファレンス (英語)](http://hikolab.github.io/pebcessing/index.html "Pebcessing Reference")

*リファレンスは現在、作成中です。*

## ライセンス

ソースコードは、MITライセンスのもとで公開されます。

リファレンスは、Creative Commonsのもとで公開されます。このリファレンスは、[Processing.org](https://processing.org/people/)によって開発されている[Processsingリファレンス](https://processing.org/reference/)をベースに作成しています。
