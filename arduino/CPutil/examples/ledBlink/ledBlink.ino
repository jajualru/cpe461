#include <CPutil.h>

// Define your pins

const int redLedPin = 38;

// Create your hardware

Led redLed(redLedPin);

void setup() 
{
  
  // Set up serial monitor and print out program info
  Serial.begin(9600);
  setupMessage(__FILE__, "Blinks an LED");
  delay(500);
  
  // Initialize your hardware
  redLed.off();

}

void loop() 
{
  // put your main code here, to run repeatedly:

   blinkLed();
 
}

void blinkLed()
{
  static MSTimer ledTimer;
  static int ledOnFlag = false;

  if (ledTimer.done() == true)
  {
    // reset the timer to 1 second
    ledTimer.set(1000);

    // turn on/off the led
    if (ledOnFlag == false)
    {
      redLed.on();
      ledOnFlag = true;
    }
    else
    {
      redLed.off();
      ledOnFlag = false;
    }
  }
   
}

