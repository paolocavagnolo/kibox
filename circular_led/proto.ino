#include <FastLED.h>

#define NUM_LEDS 39

CRGB leds[NUM_LEDS];

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,   255,  0,  0,   //red
 70,   240,  0,255,   //violet
140,   255,255,  0,   //bright yellow
255,   255,  0,  0 }; //blue

CRGBPalette16 myPal = heatmap_gp;

void setup() {
  FastLED.addLeds<WS2812B, 3, GRB>(leds, NUM_LEDS);
  
  fill_solid(leds,39, CRGB::Black);
  FastLED.show();
}

int S = 0;           //STATO MACCHINA
bool P = true;        //PRIMO INGRESSO
bool E = false;       //CONDIZIONE USCITA
unsigned long T = 0;  //TIMER UNIVERSALE
uint8_t C = 0;        //CONTATORE CICLI

uint8_t c = 0;        //colore
uint8_t d = 0;        //colore

uint8_t x1 = 0;
uint8_t x2 = 0;
uint8_t x3 = 0;
uint8_t x4 = 0;

void loop() {
  
  if (S == 0) { //SALITA E GIRO DI COLORI SOLID
    if (P) {
      for (uint8_t i=0;i<255;i++) {
        fill_solid(leds,39,CHSV(0,255,i));
        delay(15);
        FastLED.show(); 
      }
      P = false;
    }

    
    fill_solid(leds,39,CHSV(c,255,255));
    delay(70);
    FastLED.show(); 
    if (c<=255) {
      c++;
    }
    else {
      c=0;
    }

    // condizione uscita
    if (c == 255) {
      E = true;
    }

    if (E) {
      S++;
      P = true;
      E = false;
      T = millis();
      c=0;
    }
  }

  else if (S == 1) { //DIVIDI IN 2
    if (P) {
      fill_solid(leds,39,CHSV(0,255,255));
      FastLED.show(); 
      delay(1000);
      P = false;
    }

    fill(0,19,CHSV(c,255,255));
    fill(19,NUM_LEDS,CHSV(255-c,255,255));
    //shift();
    delay(80);
    FastLED.show(); 
    if (c<=255) {
      c++;
    }
    else {
      c=0;
    }

    // condizione uscita
    if ((millis() - T) > 30000) {
      E = true;
    }
    

    if (E) {
      S++;
      P = true;
      E = false;
      T = millis();
    }
  }
  
  else if (S == 2) { //DIVIDI IN 2
    if (P) {
      fill_solid(leds,39,CHSV(0,255,255));
      FastLED.show(); 
      delay(1000);
      d=128;
      c=0;
      P = false;
    }

    fill(0,13,CHSV(c,255,255));
    fill(13,26,CHSV(255-c,255,255));
    fill(26,NUM_LEDS,CHSV(d,255,255));
    delay(80);
    FastLED.show(); 
    if (c<=255) {
      c++;
    }
    else {
      c=0;
    }
    if (d<=255) {
      d++;
    }
    else {
      d=0;
    }

    // condizione uscita
    if ((millis() - T) > 30000) {
      E = true;
    }

    if (E) {
      S++;
      P = true;
      E = false;
      T = millis();
    }
  }
  else if (S == 3) { //DIVIDI IN 2
    if (P) {
      fill_solid(leds,39,CHSV(0,255,255));
      FastLED.show(); 
      delay(1000);
      d=128;
      c=0;
      P = false;
    }

    fill(0,10,CHSV(c,255,255));
    fill(10,20,CHSV(255-c,255,255));
    fill(20,30,CHSV(255-d,255,255));
    fill(30,NUM_LEDS,CHSV(d,255,255));
    delay(80);
    FastLED.show(); 
    if (c<=255) {
      c++;
    }
    else {
      c=0;
    }
    if (d<=255) {
      d++;
    }
    else {
      d=0;
    }

    if ((millis() - T) > 30000) {
      E = true;
    }

    if (E) {
      S=0;
      P = true;
      E = false;
      T = millis();
    }
  }

}

void fill(int a, int b, CRGB cc) {
  for (uint8_t i=a; i<b; i++) {
    leds[i] = cc;
  }
}

void shift() {
  CRGB x = leds[NUM_LEDS];

  for (uint8_t j=NUM_LEDS; j>0; j--) {
    leds[j] = leds[j-1];
  }

  leds[0] = x;
  
}
