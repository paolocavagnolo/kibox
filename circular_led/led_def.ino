#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
FASTLED_USING_NAMESPACE

#include "MultiMap.h"

#define DATA_PIN 3
#define NUM_LEDS 357
#define SECTOR 89
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

//#define S_RED     0xff0000
//#define S_ORANGE  0xffaa00
//#define S_LIGHT   0xfcba1a
//#define S_VIOLET  0xff00ff

#define S_RED 0xff0000
#define S_ORANGE 0xff7700
#define S_LIGHT 0xffb020
#define S_VIOLET 0xff0010

CRGB leds[NUM_LEDS];

// #ff0000
CRGBPalette16 full_sector = { S_VIOLET, S_VIOLET, S_ORANGE, S_ORANGE, S_ORANGE, S_ORANGE, S_RED, S_RED,
                              S_RED, S_RED, S_LIGHT, S_LIGHT, S_LIGHT, S_LIGHT, S_VIOLET, S_VIOLET };


// #ff0000
CRGBPalette16 red_sector = { 0xb30000, 0xbe0000, 0xc90000, 0xd40000, 0xdf0000, 0xea0000, 0xf50000, 0xff0000,
                             0xff0000, 0xff0b0b, 0xff1616, 0xff2121, 0xff2c2c, 0xff3737, 0xff4242, 0xff4d4d };

// #ff8000
CRGBPalette16 orange_sector = { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117,
                                0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };

// #ff0080
CRGBPalette16 violet_sector = { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33,
                                0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

// #8000ff
CRGBPalette16 yellow_sector = { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33,
                                0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };


long norm_dist[] = { 0, 5, 20, 50, 80, 95, 100, 95, 80, 50, 20, 5, 0 };                                                           // 13
long sinus[] = { 0, 316, 601, 827, 972, 1023, 972, 827, 601, 316, 0, -316, -601, -827, -972, -1023, -972, -827, -601, -316, 0 };  //21
long _log10[] = { -1000000, 460, 529, 570, 599, 621, 639, 655, 668, 680, 690 };                                                   // size 11
long _exp3[] = { 0, 1, 3, 9, 27, 81, 243, 729, 2187, 6561 };                                                                      // size 10
long _exp2[] = { 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };                                                                // size 12
long sawtooth[] = { 0, 1000, 0, -1000, 0, 1000, -1000, 0 };                                                                       // size 8

long map_normal[NUM_LEDS];
long map_sinus[NUM_LEDS];
long map_exp2[NUM_LEDS];
long map_exp3[NUM_LEDS];

unsigned long T = 0;

void setup() {
  delay(3000);  // 3 second delay for boot recovery, and a moment of silence
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  test_normal_distribution();
  test_sinus();
  test_exp2();
  test_exp3();

  // // 1 test all LED
  // for (int i = 0; i < NUM_LEDS; i++) {
  //   leds[i] = CRGB::White;
  //   FastLED.show();
  //   delay(20);
  // }

  // // 2 check power supply
  // delay(5000);


  // // 3 check 4 colori
  // for (int i = 0; i < 255; i++) {
  //   FastLED.setBrightness(i);
  //   fill_solid(leds, NUM_LEDS, ColorFromPalette(full_sector, 0, 255, LINEARBLEND));
  //   FastLED.show();
  //   delay(10);


  // }
  // delay(5000);

  // for (int i = 0; i < 255; i++) {
  //   FastLED.setBrightness(i);
  //   fill_solid(leds, NUM_LEDS, ColorFromPalette(full_sector, 64, 255, LINEARBLEND));
  //   FastLED.show();
  //   delay(10);

  // }
  // delay(5000);

  // for (int i = 0; i < 255; i++) {
  //   FastLED.setBrightness(i);
  //   fill_solid(leds, NUM_LEDS, ColorFromPalette(full_sector, 128, 255, LINEARBLEND));
  //   FastLED.show();
  //   delay(10);

  // }
  // delay(5000);

  // for (int i = 0; i < 255; i++) {
  //   FastLED.setBrightness(i);
  //   fill_solid(leds, NUM_LEDS, ColorFromPalette(full_sector, 172, 255, LINEARBLEND));
  //   FastLED.show();
  //   delay(10);

  // }
  // delay(5000);
  FastLED.setBrightness(255);
  T = millis();
}

uint32_t t = 0;
uint16_t j = 0;
uint16_t mm = 255;
bool fm = false;

uint8_t cc;

void loop() {
  // put your main code here, to run repeatedly:


  EVERY_N_MILLISECONDS(10) {
    c_loop();
    FastLED.show();
  }
}

void c_loop() {

  for (int i = 0; i < NUM_LEDS; i++) {

    cc = map(i, j, NUM_LEDS + j, 0, 255);
    cc = cc + map_sinus[i] * 0.2;
    //        cc = map_normal[i];
    //        cc = map_sinus[i];
    //        cc = map_exp2[i];
    //        cc = map_exp3[i];

    leds[i] = ColorFromPalette(full_sector, cc, 255, LINEARBLEND);
  }

  if (j >= 0) {
    j--;
  } else {
    j = NUM_LEDS;
  }

}

// Deepen the blues and greens
void pacifica_deepen_colors() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8(leds[i].blue, 145);
    leds[i].green = scale8(leds[i].green, 200);
    //leds[i] |= CRGB( 2, 5, 7);
  }
}

void test_normal_distribution() {
  // sort of normal distribution
  long norm_dist[] = { 0, 5, 20, 50, 80, 95, 100, 95, 80, 50, 20, 5, 0 };  // 13
  long in[13];
  for (int i = 0; i < 13; i++) in[i] = round(i * NUM_LEDS / 12);
  for (int i = 0; i < 13; i++) norm_dist[i] = map(norm_dist[i], 0, 100, 0, 255);
  for (int i = 0; i < NUM_LEDS; i++) {
    long y = multiMap<long>(i, in, norm_dist, 13);
    map_normal[i] = y;
  }
}


void test_sinus() {
  // one sinus wave, amplitude 1023
  long sinus[] = { 0, 316, 601, 827, 972, 1023, 972, 827, 601, 316, 0, -316, -601, -827, -972, -1023, -972, -827, -601, -316, 0 };  //21
  long in[21];
  for (int i = 0; i < 21; i++) in[i] = round(i * NUM_LEDS / 20);
  for (int i = 0; i < 21; i++) sinus[i] = map(sinus[i], -1023, 1023, 0, 255);
  for (int i = 0; i < NUM_LEDS; i++) {
    long y = multiMap<long>(i, in, sinus, 21);
    map_sinus[i] = y;
  }
}



void test_exp2() {
  // 2^x
  long _exp2[] = { 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };  // size 12
  long in[12];
  for (int i = 0; i < 12; i++) in[i] = round(i * NUM_LEDS / 11);
  for (int i = 0; i < 12; i++) _exp2[i] = map(_exp2[i], 0, 1024, 0, 255);
  for (int i = 0; i < NUM_LEDS; i++) {
    long y = multiMap<long>(i, in, _exp2, 12);
    map_exp2[i] = y;
  }
}


void test_exp3() {
  // 3^x
  long _exp3[] = { 0, 1, 3, 9, 27, 81, 243, 729, 2187, 6561 };  // size 10
  long in[10];
  for (int i = 0; i < 10; i++) in[i] = round(i * NUM_LEDS / 9);
  for (int i = 0; i < 10; i++) _exp3[i] = map(_exp3[i], 0, 6561, 0, 255);
  for (int i = 0; i < NUM_LEDS; i++) {
    long y = multiMap<long>(i, in, _exp3, 10);
    map_exp3[i] = y;
  }
}
