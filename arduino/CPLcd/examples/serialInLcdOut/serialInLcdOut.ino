
// LCD example that uses the CPLcd library
// HMS - Oct. 2019


#include <CPutil.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <CPLcd.h>

CPLcd myLcd;

void setup() {
  
	// Set up serial monitor and print out program info
	Serial.begin(9600);
	setupMessage(__FILE__, "Testing out the LCD");
	delay(500);  
	
	myLcd.init();
	myLcd.writeWithWrap("Enter text in   serial console.");
	delay(2000);
		
	Serial.println("Enter text ending with a '.'");
}

void loop() 
{
  serialInLcdOut();
}

void serialInLcdOut()
{
	
	char aChar = 0;
	uint8_t numChars = 0;

	Serial.print("\nCol 0 (up to 16 chars, end with '.'): ");
	
	// block until user enters text, so it won't just clear)
	// the old text off the lcd screen.
	
	while (!Serial.available())
	{}
	
	myLcd.clear();
	myLcd.home();

	while (aChar != '.' && numChars < ROWS)
	{
		if (Serial.available())
		{
			numChars++;
			aChar = Serial.read();
			if (aChar != '.')
			{
				myLcd.print(aChar);
				Serial.print(aChar);
			}
		}
	}

	Serial.print("\nCol 1 (up to 16 chars, end with '.'):");
	aChar = 0;
	numChars = 0;
	myLcd.setCursor(0,1);
	while (aChar != '.' && numChars < ROWS)
	{
		if (Serial.available())
		{
			numChars++;
			aChar = Serial.read();
			if (aChar != '.')
			{
				myLcd.print(aChar);
				Serial.print(aChar);
			}
		}
	}

}