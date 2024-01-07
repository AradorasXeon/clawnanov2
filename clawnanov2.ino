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

#define START_UP_CALIB_TIME_MS  2500
#define TIMER_CHECK_RECEIVED_MS 100 
#define TIMER_LOOP_MS           250    //needs further testing with lower value, now it is this high because of debugging


MoveMaster msgMove;
MillisTimer timerCheckReceived(TIMER_CHECK_RECEIVED_MS);
MillisTimer timerLoop(TIMER_LOOP_MS);
MillisTimer timer2seconds(2000);


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

  int time = millis();
  while(millis()-time < START_UP_CALIB_TIME_MS)
  {
    //maybe some skip possibilities here later
    //probably if something pressed then set a boolean
    //don't forget to start gameplay music as well, if no calibration is done
  }
  doCalibration();
}

void loop() 
{
  refreshControllState();
  msgMove.sendMessageToSlave();
  timerLoop.doDelay();

  /* 
  FillStripPartSlow(0, 255, 0, LED_LEFT_MIN, LED_LEFT_MAX);
  FillStripPartSlow(255, 255, 0, LED_DOWN_MIN, LED_DOWN_MAX);
  FillStripPartSlow(0, 255, 255, LED_RIGHT_MIN, LED_RIGHT_MAX);
  FillStripPartSlow(255, 0, 255, LED_UP_MIN, LED_UP_MAX);
  FillStripPart(1, 1, 255, 0, LED_LAST);
  */
}

void refreshControllState() //later I should add some LED controll here
{
  msgMove.setDefaultControllState();

  if(digitalRead(BUTTON))
  {
    msgMove.setButton();
  }

  if(digitalRead(LEFT))
  {
    msgMove.setLeft();
  }

  if(digitalRead(RIGHT))
  {
    msgMove.setRight();
  }

  if(digitalRead(UP))
  {
    msgMove.setUp();
  }

  if(digitalRead(DOWN))
  {
    msgMove.setDown();
  }

}

void doCalibration()
{
  //Inits calibration
  //doCheckCalibState(msgMove, &MoveMaster::calibInit, Claw_Calibration::CLAW_CALIB_INIT);

  //for test reasons I skipp doCheckCalibState
  msgMove.calibInit();
  msgMove.sendMessageToSlave();
  timerCheckReceived.doDelay();

  //First we calibrate the top side
  //doCheckCalibState(msgMove, &MoveMaster::calibStratTop, Claw_Calibration::CLAW_CALIB_TOP_STATE_IN_PROGRESS);
  msgMove.calibStratTop();
  msgMove.sendMessageToSlave();
  timerCheckReceived.doDelay();

  FillStripPart(150, 100, 0, 0, LED_LAST);

  while(!msgMove.wasButtonPressed())
  {
    refreshControllState();
    msgMove.sendMessageToSlave();
    timerCheckReceived.doDelay();
  }

  //We are done with top calib 
  //doCheckCalibState(msgMove, &MoveMaster::calibFinishTop, Claw_Calibration::CLAW_CALIB_TOP_DONE);
  msgMove.calibFinishTop();
  msgMove.sendMessageToSlave();
  timerCheckReceived.doDelay();

  //Second we calibrate the top side
  //doCheckCalibState(msgMove, &MoveMaster::calibStartDown, Claw_Calibration::CLAW_CALIB_DOWN_STATE_IN_PROGRESS);
  msgMove.calibStartDown();
  msgMove.sendMessageToSlave();
  timerCheckReceived.doDelay();

  timer2seconds.doDelay();

  FillStripPart(0, 100, 150, 0, LED_LAST);

  msgMove.setDefaultControllState();
  while(!msgMove.wasButtonPressed())
  {
    refreshControllState();
    msgMove.sendMessageToSlave();
    timerCheckReceived.doDelay();
  }

  //We are done with down calib 
  //doCheckCalibState(msgMove, &MoveMaster::calibFinishDown, Claw_Calibration::CLAW_CALIB_DOWN_DONE);
  msgMove.calibFinishDown();
  msgMove.sendMessageToSlave();
  timerCheckReceived.doDelay();

  //Send finish calib signal
  //doCheckCalibState(msgMove, &MoveMaster::calibDone, Claw_Calibration::CLAW_CALIB_FINISHED);
  msgMove.calibDone();
  msgMove.sendMessageToSlave();
  timerCheckReceived.doDelay();

  //should put here an auto pull up the claw later, and make it prallel with the LED show

  rainbowCycle(2);

  FillStripPart(1, 1, 1, 0, LED_LAST);

}

//Currently not in use might try it later

/// @brief Makes sure that the message went through to the slave, only returns if the slave got the message
/// @param moveObject the MoveMaster class 
/// @param function to call on the moveObject
/// @param calibStateToCheck the state to check, should relate to the function 
void doCheckCalibState(MoveMaster moveObject, void (MoveMaster::*function)(), Claw_Calibration calibStateToCheck )
{
  while(!moveObject.isReadCalibStateContains(calibStateToCheck))
  {
    moveObject.setDefaultControllState(); //while back and forth comm is on going controller should be default
    (moveObject.*function)();
    moveObject.sendMessageToSlave();
    timerCheckReceived.doDelay();
    moveObject.readMessageFromSlave();
  }
}