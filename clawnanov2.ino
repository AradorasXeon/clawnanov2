#define DEBUG
//#define MUSIC

#include "communication.hpp"
#include "communicationMusic.h"
#include "timer.h"
#include "millisTimer.hpp"
#include "leds.h"

#ifdef __AVR__
  #include <avr/power.h>
#endif

//comments here are my wire colors
#define CLAW_PIN 2  //not colored --> soldered directly to transistor
#define BUTTON 8    //yellow
#define LEFT 9      //blue
#define RIGHT 10    //purple
#define UP 11       //green
#define DOWN 12     //white

MoveMaster msgMove;

void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(CLAW_PIN, OUTPUT);
  
  pinMode(BUTTON, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(CLAW_PIN, LOW);

  msgMove = MoveMaster();

  strip.begin();
  strip.setBrightness(LED_BRIGHTNESS);
  FillStripPart(1, 1, 255, 0, LED_LAST);

  //Debug:
  #ifdef DEBUG
    Serial.begin(115200);
    Serial.println("BASE SETUP RAN");
  #endif // DEBUG

}

void loop() 
{
  FillStripPartSlow(0, 255, 0, LED_LEFT_MIN, LED_LEFT_MAX);
  FillStripPartSlow(255, 255, 0, LED_DOWN_MIN, LED_DOWN_MAX);
  FillStripPartSlow(0, 255, 255, LED_RIGHT_MIN, LED_RIGHT_MAX);
  FillStripPartSlow(255, 0, 255, LED_UP_MIN, LED_UP_MAX);
  FillStripPart(1, 1, 255, 0, LED_LAST);

}