/* rgbRing --- drive a 24-LED ring of WS2812B RGB LEDs      2018-06-26 */
/* Copyright (c) 2018 John Honniball. All rights reserved.             */

#include <avr/pgmspace.h>
#include "FastLED.h"


#define NUM_LEDS    (24)
#define BRIGHTNESS  (64)

#define BUTTON_PIN  (2)
#define DATA_PIN    (3)


CRGB Leds[NUM_LEDS];

const long Frame01[NUM_LEDS] PROGMEM =
{
0xff0000, 0x00ff00, 0x0000ff, 0xffff00, 0x00ffff, 0xff00ff, 0xffffff, 0x000000, // 0-7
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, // 8-15
0xff0000, 0x00ff00, 0x0000ff, 0xffff00, 0x00ffff, 0xff00ff, 0xffffff, 0x000000  // 16-23
};

const long Frame02[NUM_LEDS] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0xff0000, 0x00ff00, 0x0000ff, 0xffff00, 0x00ffff, 0xff00ff, 0xffffff, 0x000000,
0xff0000, 0x00ff00, 0x0000ff, 0xffff00, 0x00ffff, 0xff00ff, 0xffffff, 0x000000
};

const CRGB RandomColours[12] = {
  CRGB::Red, CRGB::Yellow, CRGB::Lime, CRGB::White,   CRGB::Blue, CRGB::Yellow, 
  CRGB::Red, CRGB::White,  CRGB::Lime, CRGB::Magenta, CRGB::Lime, CRGB::White
};


void setup(void)
{
  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL,DATA_PIN>(Leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}


void loop(void)
{
  int ana;
  int led, level;
  int frame;
  int i;
  int hue;
  int dot = 0;
  int dir = 1;
  int mode = 0;
  int state = 0;

  for (frame = 0; ; frame++) {
    ana = analogRead(0);
    
    level = map(ana, 0, 1023, 0, 255);
    led = map(ana, 0, 1023, 0, NUM_LEDS - 1);

    switch (mode) {
    case 0:
      FastLED.showColor(CRGB::Yellow, level);
      break;
    case 1:
      FastLED.showColor(CHSV(level, 255, 255), BRIGHTNESS);
      break;
    case 2:
      FastLED.clear();
      Leds[dot] = CHSV(level, 255, 255);
      FastLED.show();
      Leds[dot] = CRGB::Black; // clear this LED for the next time around the loop

      if (frame & 1) {
        if (dot >= (NUM_LEDS - 1))
          dir = -1;
        else if (dot <= 0)
          dir = 1;

        dot += dir;
      }
      break;
    case 3:
      FastLED.clear();
      Leds[led] = CRGB::Red;
      FastLED.show();
      break;
    case 4:
      for (i = 0; i < NUM_LEDS; i++) {
        if ((frame / 12) & 1)
          Leds[i] = pgm_read_dword(&(Frame02[i]));
        else
          Leds[i] = pgm_read_dword(&(Frame01[i]));
      }
  
      FastLED.show();
      break;
    case 5:
      FastLED.showColor(RandomColours[dot], level);
      if ((frame % 10) == 0)
        if (++dot > 11)
          dot = 0;
      break;
    case 6:
      for (i = 0; i < NUM_LEDS; i++) {
        hue = map(i, 0, NUM_LEDS - 1, 0, 255);
        hue = (hue + level) & 255;
        
        Leds[i] = CHSV(hue, 255, 255);
      }

      FastLED.show();
      break;
    case 7:
      for (i = 0; i < NUM_LEDS; i += 3) {
        switch (state) {
        case 0:
          Leds[i]     = CRGB::Red;
          Leds[i + 1] = CRGB::Lime;
          Leds[i + 2] = CRGB::Blue;
          break;
        case 1:
          Leds[i]     = CRGB::Blue;
          Leds[i + 1] = CRGB::Red;
          Leds[i + 2] = CRGB::Lime;
          break;
        case 2:
          Leds[i]     = CRGB::Lime;
          Leds[i + 1] = CRGB::Blue;
          Leds[i + 2] = CRGB::Red;
          break;
        }
      }

      FastLED.show();

      
      if ((frame % 16) == 0) {
        if (state == 2)
          state = 0;
        else
          state++;
      }
      
      break;
    case 8:
      for (i = 0; i < NUM_LEDS; i += 3) {
        switch (state) {
        case 0:
          Leds[i]     = CHSV(level, 255, 255);
          Leds[i + 1] = CRGB::Black;
          Leds[i + 2] = CRGB::Black;
          break;
        case 1:
          Leds[i]     = CRGB::Black;
          Leds[i + 1] = CHSV(level, 255, 255);
          Leds[i + 2] = CRGB::Black;
          break;
        case 2:
          Leds[i]     = CRGB::Black;
          Leds[i + 1] = CRGB::Black;
          Leds[i + 2] = CHSV(level, 255, 255);
          break;
        }
      }

      FastLED.show();

      if ((frame % 4) == 0) {
        if (state == 2)
          state = 0;
        else
          state++;
      }
      break;
    }

    if (digitalRead(BUTTON_PIN) == LOW) {
      while (digitalRead(BUTTON_PIN) == LOW)
        ;
   
      if (mode < 8)
        mode++;
      else
        mode = 0;

      state = 0;
    }
    
    delay(20);
  }
}


