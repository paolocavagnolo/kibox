#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
FASTLED_USING_NAMESPACE

#define NUM_LEDS 214
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

CRGB RED_custom = CRGB(255, 0, 0);       // 210,   0,  60
CRGB BLUE_custom = CRGB(15, 0, 255);    // 15,    0,  105
CRGB YELLOW_custom = CRGB(255, 204, 0);     // 255, 204,  0

void setup() {
  // put your setup code here, to run once:
  delay(3000);
  FastLED.addLeds<LED_TYPE, 2, COLOR_ORDER>(leds, NUM_LEDS);//.setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, 4, COLOR_ORDER>(leds, NUM_LEDS);//.setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(255);

  fill_solid(leds, NUM_LEDS, CRGB::Black);

  FastLED.show();
}

void loop() {

  for (int j=0; j<5; j++) {
    color_beam();
  }

  flashes();

  
}

void flashes() {
  for (int i = 0; i < 5; i++) {
    fill_solid(leds, NUM_LEDS, BLUE_custom);
    delay(100);

    for (int j = 0; j < 40; j++) {
      nscale8(leds, NUM_LEDS, 235);
      FastLED.show();
    }
    if ((i == 1) || (i == 4)) {
      delay(200);
    }
  }

  for (int i = 0; i < 5; i++) {
    fill_solid(leds, NUM_LEDS, RED_custom);
    delay(100);

    for (int j = 0; j < 40; j++) {
      nscale8(leds, NUM_LEDS, 235);
      FastLED.show();
    }
    if ((i == 1) || (i == 4)) {
      delay(200);
    }
  }

  for (int i = 0; i < 5; i++) {
    fill_solid(leds, NUM_LEDS, YELLOW_custom);
    delay(100);

    for (int j = 0; j < 40; j++) {
      nscale8(leds, NUM_LEDS, 235);
      FastLED.show();
    }
    if ((i == 1) || (i == 4)) {
      delay(200);
    }
  }

}

void color_beam() {
  for (int i = 0; i < NUM_LEDS; i++) {

    leds[i] = BLUE_custom;

    FastLED.show();

    nscale8(leds, NUM_LEDS, 245);
  }

  for (int i = 0; i < NUM_LEDS; i++) {

    leds[i] = RED_custom;

    FastLED.show();

    nscale8(leds, NUM_LEDS, 245);
  }

  for (int i = 0; i < NUM_LEDS; i++) {

    leds[i] = YELLOW_custom;

    FastLED.show();

    nscale8(leds, NUM_LEDS, 245);
  }

}
