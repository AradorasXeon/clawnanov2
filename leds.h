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

#endif // !LEDS_H_2023_01_05