#include "CPutil.h"

// Define your hardware pins
const int buttonPin = 9;

// Create your hardware
Button myButton(buttonPin);;

void setup() {
  
  // Set up serial monitor and print out program info
  Serial.begin(9600);
  setupMessage(__FILE__, "Press the button...");
  delay(500);
    
}

void loop() {
 
  // Control code to turn on/off LED based on button
   buttonToSerial();
 
}

void buttonToSerial()
{
	static int buttonCount = 0;

	// Will only recognize its been pushed once per second
	if (myButton.wasPushed() == true)
	{
		buttonCount = buttonCount + 1;
		Serial.print(1);
	
		// The % gives you the reminder of a division, so if the number is divisable by 5 we get 0
		if (buttonCount % 5 == 0)
		{
			print2("\nButton Count is: ", buttonCount);
		}
	}
  

   
}

