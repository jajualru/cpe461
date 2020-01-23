#include "CPutil.h"

// Define your hardware pins


// Create your hardware


void setup() {
  
  // Set up serial monitor and print out program info
  Serial.begin(9600);
  setupMessage(__FILE__, "Reads from Serial and outputs its ASCII value (computer value)");
  delay(500);
  
  Serial.println("\nThis program outputs your letter in the value stored by the computer");
  Serial.println("This is called is ASCII value and is standard across all computers");
  Serial.println("e.g. C = 67 and a = 97... type any letter and see");
  
  // Initialize your hardware
    
}

void loop() {
 
  // Read/Write to serial connection
   readPrintSerial();
 
}

void readPrintSerial()
{
	
	int aLetter = 0;
	
	// Not this is a blocking loop - not good for any realtime processing
	// Wait until the user types something
	while (!Serial.available())
	{
		// Just loop and do nothing until the user types something
	}
	
	// A character is available for read... so read it.
	aLetter = Serial.read();
	print4("Your letter: ", (char) aLetter, " in ASCII: ", aLetter); 

   
}

