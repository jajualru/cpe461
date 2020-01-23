
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
	
	
	myLcd.init();
	myLcd.print("Hello, World!");
	delay(500);  
		
}

void loop() 
{
	writeAChar();
}

void writeAChar()
{	
	// note the & 0x0f means use only the lower 4 bits so its values 0-15
	static unsigned char row = 0;
	
	print2("row: ", row);
	
	myLcd.writeChar(row, 1, '.');
	delay(500);
	
	// clear the row
	if (row == 15)
	{
		myLcd.writeRow1();
		delay(500);
	}
	
	row = (row + 1) & 0x0f;  // goes between 0 and 15
	
}