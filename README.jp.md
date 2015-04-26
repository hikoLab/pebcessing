#Pebcessing

Pebcessingは現在α版です。実験中の段階なので、まだ仕様はfixしておらず多くの未完成の機能があります。

##概要

Pebcessingは、[Pebble smartwach](https://getpebble.com/ "Pebble") のアプリをシンプルにコーディングできるフレームワークです。
シンプルな関数でPebbleの画面にグラフィックスを描画できるのが特徴です。プログラミングの初心者の方がオリジナルのwatchfaceを作ったり、デザイナーの方がスマートウォッチのためのビジュアルデザインの実験をする、といったことを手軽に行えるようにするために作られています。

Pebcessingは、ビジュアルデザインやメディアアート、教育といった分野で使われている、オープンソースのプログラミング言語[Processing](https://processing.org/ "Processing")の影響を強く受けています。Processingに類似した関数が用意されているので、Processingライクにプログラミングすることができます。

##例

例えば、以下のコードでPebbleアプリを作ることができます。実行するとシンプルなアニメーションが描画されます。

```Processing
#include "sketch.h"
#include "pebcessing/def_for_sketch.h"

// 初めに1度呼ばれる関数
void setup()
{
  frameRate(5); // フレームレートを設定。毎秒5回draw()を実行する。
  background(WHITE); // 背景を白で塗りつぶす
  fill(BLACK); // 塗りつぶしの色を黒に設定
  stroke(WHITE); // 線の色を白に設定
}

// 毎フレーム描画の度に呼ばれる関数
void draw()
{
  // ランダムな位置と大きさで四角形を描画する
  float x = random(-20, sketchWidth);
  float y = random(-20, sketchHeight);
  float w = random(10, 30);
  rect(x, y, w, w); // 四角形を描画
}
```
![example1](https://raw.githubusercontent.com/hikoLab/pebcessing/images/images/screenshot_random_rect.png) (*この画像は、Pebbleエミュレータのスクリーンショットです。*)

次の例は、Pebble Timeのためのデジタル時計です。color()で色を作ることができます。

```Processing
#include "sketch.h"
#include "pebcessing/def_for_sketch.h"

void setup()
{
  PFont font = loadSystemFont(FONT_KEY_BITHAM_42_LIGHT); // システムフォントをロードする
  textFont(font); // テキストを描画する際のフォントを指定
  noStroke(); // 線の描画はしない
  noLoop(); // draw()を繰り返し呼ばない
}

void draw()
{
  int w = sketchWidth; // 描画領域の幅
  int h = sketchHeight; // 描画領域の高さ
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      fill(color(i * 85, j * 85, 255)); // 塗りつぶしの色をcolor(R, G, B)に設定
      rect(i * w / 4, j * h / 4, w / 4, h / 4); // 四角形を描画
    }
  }

  // C言語のスタイルの文字列処理
  char timeStr[6];
  snprintf(timeStr, 6, "%2d:%02d", hour(), minute()); // 現在時刻で"時間:分"という文字列を作る

  fill(WHITE);
  textAlign(CENTER); // テキストの描画を中央揃えにする
  text(timeStr, w / 2, 35); // テキストを描画
}

// 毎分呼ばれる関数
// これを使うためには、setting.hでENABLE_MINUTE_EVENTをdefineする必要がある
void minuteEvent()
{
  redraw(); // 再描画。draw()が実行される。
}
```
![example2](https://github.com/hikoLab/pebcessing/blob/images/images/screenshot_digital_watch.png)

HSBカラーを使うこともできます。

```Processing
#include "sketch.h"
#include "pebcessing/def_for_sketch.h"

void setup()
{
  colorMode(HSB); // 色をHSBで指定する
  noStroke();
  noLoop();
}

void draw()
{
  int n = 10;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      int cl = color(255.0 * i / n, 255.0 - 255.0 * j / n, 255.0); // 色相、彩度、明度の値から色をつくる
      fill(cl);
      ellipse((float)sketchWidth * i / n + 6, (float)sketchHeight * j / n + 6, 10, 10); // 円を描画
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

  translate(sketchWidth / 2 + 15, 0); // 座標の原点を移動させる

  for(int i = 0; i < 18; i++){
    fill(color(i * 255 / 18, 255, 255));
    rect(0,0,30,30);
    fill(color(i * 255 / 18, 100, 255));
    rect(0,40,20,20);

    rotate(radians(20)); // 座標を20°回転させる
    translate(30, 0); // 座標の原点を移動させる
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
