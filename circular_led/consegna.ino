//
//  "Circular Led Animations"
//  Create by Paolo Cavagnolo for SGH
//  paolo.cavagnolo@gmail.com
//  march 2023
//

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
FASTLED_USING_NAMESPACE

#include "MultiMap.h"

#define DATA_PIN 	3
#define NUM_LEDS 	357
#define SECTOR 		89
#define LED_TYPE 	WS2812B
#define COLOR_ORDER GRB

#define S_RED 		0xff0000
#define S_ORANGE 	0xff7700
#define S_LIGHT 	0xffb020
#define S_VIOLET 	0xff0010

CRGB leds[NUM_LEDS];

// #ff0000
CRGBPalette16 full_sector = { S_VIOLET, S_VIOLET, S_ORANGE, S_ORANGE, S_ORANGE, S_ORANGE, S_RED, S_RED,
                              S_RED, S_RED, S_LIGHT, S_LIGHT, S_LIGHT, S_LIGHT, S_VIOLET, S_VIOLET };

long sinus[] = { 0, 316, 601, 827, 972, 1023, 972, 827, 601, 316, 0, -316, -601, -827, -972, -1023, -972, -827, -601, -316, 0 };  //21

long map_sinus[NUM_LEDS];


void setup() {

  delay(3000);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  test_sinus();

  FastLED.setBrightness(255);

}

void loop() {

  EVERY_N_MILLISECONDS(10) {
    c_loop();
    FastLED.show();
  }

}

uint16_t j = 0;
uint8_t cc;

void c_loop() {

  for (int i = 0; i < NUM_LEDS; i++) {

    cc = map(i, j, NUM_LEDS + j, 0, 255);
    cc = cc + map_sinus[i] * 0.2;

    leds[i] = ColorFromPalette(full_sector, cc, 255, LINEARBLEND);
  }

  if (j >= 0) {
    j--;
  } else {
    j = NUM_LEDS;
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

