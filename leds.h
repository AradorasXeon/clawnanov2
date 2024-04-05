//Most of this is from an Adafruit_NeoPixel example

#ifndef LEDS_H_2023_01_05
#define LEDS_H_2023_01_05

#include "millisTimer.hpp"
#include <Adafruit_NeoPixel.h>

#define LED_PIN 3   //dark green wire in project



//nth LED, we can use these to light up appropaite LEDs for left, right, up, down movements
#define LED_LEFT_MIN 0
#define LED_LEFT_MAX 17
#define LED_DOWN_MIN 18
#define LED_DOWN_MAX 37
#define LED_RIGHT_MIN 38
#define LED_RIGHT_MAX 57
#define LED_UP_MIN 58
#define LED_UP_MAX 78

#define LED_LAST 78
#define LED_BRIGHTNESS 50

#define LED_DELAY_MS 1
#define LED_LONG_DELAY_MS 80
#define LED_BK_GND_LOW 60


// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_LAST, LED_PIN, NEO_GRB + NEO_KHZ800); // Constructor: number of LEDs, pin number, LED type
//88 LEDs @50 strength white about 700 mA
//88 LEDs @80 strength white about 1070 mA
//88 LEDs @100 strength white about 1290 mA
//100 LEDs @100 strength white about 1310 mA


MillisTimer timerLEDdelay(LED_DELAY_MS);
MillisTimer timerLEDlongDelay(LED_LONG_DELAY_MS);

uint8_t bkgnd[3];
int8_t bkgndIncrement[3];

void initBackGroundShow()
{
  bkgnd[0] = LED_BK_GND_LOW;
  bkgnd[1] = LED_BK_GND_LOW + 20;
  bkgnd[2] = LED_BK_GND_LOW + 45;

  bkgndIncrement[0] = 5;
  bkgndIncrement[1] = 1;
  bkgndIncrement[2] = 10;

}

/// @brief needs initBackGroundShow to be called at setup, this shall be called at every loop end
void LedBackGroundShow()
{
  for(uint16_t i=LED_LEFT_MIN; i<=LED_UP_MAX; i+=3) // i am lucky now because LED_UP_MAX is divisible with 3
  {
    strip.setPixelColor(i+0, strip.Color(bkgnd[0], bkgnd[1], bkgnd[2]));
    strip.setPixelColor(i+1, strip.Color(bkgnd[1], bkgnd[2], bkgnd[0]));
    strip.setPixelColor(i+2, strip.Color(bkgnd[2], bkgnd[0], bkgnd[1]));
  }
  strip.show();
  
  bkgnd[0] += bkgndIncrement[0];
  bkgnd[1] += bkgndIncrement[1];
  bkgnd[2] += bkgndIncrement[2];

  for(uint8_t i = 0; i<3; i++)
  {
    if(bkgnd[i] < LED_BK_GND_LOW) //this means we overflowed
    {
      bkgnd[i] = 255;
      bkgndIncrement[i] *= -1;
    }
  }

}



/// @brief Fills part of the actual Adafruit_NeoPixel led strip
/// @param r red
/// @param g green
/// @param b blue
/// @param startLed from this LED
/// @param endLed to this LED
void FillStripPart(uint8_t r, uint8_t g, uint8_t b, uint16_t startLed, uint16_t endLed)
{

  for(uint16_t i=startLed; i<=endLed; i++) 
  {
    strip.setPixelColor(i, strip.Color(r, g, b));
    timerLEDdelay.doDelay();
  }

  strip.show();
}

/// @brief Fills part of the actual Adafruit_NeoPixel led strip SLOWLY
/// @param r red
/// @param g green
/// @param b blue
/// @param startLed from this LED
/// @param endLed to this LED
void FillStripPartSlow(uint8_t r, uint8_t g, uint8_t b, uint16_t startLed, uint16_t endLed)
{
  for(uint16_t i=startLed; i<=endLed; i++) 
  {
    strip.setPixelColor(i, strip.Color(r, g, b));
    timerLEDlongDelay.doDelay();
    strip.show();
  }
}

void LightUpScene()
{
  for(uint16_t i=LED_LEFT_MIN; i<=LED_UP_MAX; i++) 
  {
    strip.setPixelColor(i, strip.Color(255, 255, 255));
  }
    strip.show();
}

void ErrorLed()
{
  while (1) //delibaretly not exiting from here, while debugging
  {
    FillStripPartSlow(255, 0, 0, 0, LED_LAST);
    timerLEDlongDelay.doDelay();
    FillStripPartSlow(1, 0, 0, 0, LED_LAST);
    timerLEDlongDelay.doDelay();
  }
  
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    MillisTimer::delayThisMuch(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    MillisTimer::delayThisMuch(wait);
  }
}

void rainbowCycleShort(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*2; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    MillisTimer::delayThisMuch(wait);
  }
}

#endif // !LEDS_H_2023_01_05