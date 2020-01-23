#include "Arduino.h"
#include "CPutil.h"
#include "CPE123_EncoderLib.h"

// Updated Fall 2018

static volatile unsigned long leftEncoderCounter = 0;
static volatile unsigned long rightEncoderCounter = 0;

void encoderSetup(int rightEncoderPin1, int rightEncoderPin2, int leftEncoderPin1, int leftEncoderPin2)
{
    // sets up your encoder pin, interrupt and counter

    resetEncoders();    
    // Set up pins and attach interrupts
       
    pinMode(rightEncoderPin1, INPUT);
    attachInterrupt(digitalPinToInterrupt(rightEncoderPin1), rightEncoderISR, CHANGE);
    pinMode(rightEncoderPin2, INPUT);
    attachInterrupt(digitalPinToInterrupt(rightEncoderPin2), rightEncoderISR, CHANGE);
    
    pinMode(leftEncoderPin1, INPUT);
    attachInterrupt(digitalPinToInterrupt(leftEncoderPin1), leftEncoderISR, CHANGE);
    pinMode(leftEncoderPin2, INPUT);
    attachInterrupt(digitalPinToInterrupt(leftEncoderPin2), leftEncoderISR, CHANGE);
}

unsigned long rightEncoderCount()
{
  return rightEncoderCounter;
}

unsigned long leftEncoderCount()
{
  return leftEncoderCounter;
}

void resetEncoders()
{
  resetRightEncoder();
  resetLeftEncoder();
}

void resetRightEncoder()
{
  rightEncoderCounter = 0;
}

void resetLeftEncoder()
{
  leftEncoderCounter = 0;
}



void leftEncoderISR()
{
  // ISR for the left encoder
  //Maintains the encoder count 
  
  leftEncoderCounter = leftEncoderCounter + 1;
}

void rightEncoderISR()
{
  // ISR for the right encoder
  //Maintains the encoder count 
  
  rightEncoderCounter = rightEncoderCounter + 1;
}
