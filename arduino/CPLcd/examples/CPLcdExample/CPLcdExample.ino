
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
	setupMessage(__FILE__, "Using the CPLcd library");
	
	myLcd.init();
	
	delay(500);  
	
}

void loop() 
{
	lcdExamples();
	
}

void lcdExamples()
{
	
	// using the writeRow methods (note writeRow clears to end of line)
	// delays() is used so you can see it work)
	
	myLcd.writeRow1("Hello, you");
	delay(500);
	myLcd.writeRow0("Hello, World");
	delay(500);
	myLcd.writeRow1("Bye");
	delay(500);
	
	myLcd.clear();
	delay(500);
	
	myLcd.writeWithWrap("Today is the day that all good...");
	delay(1000);

	myLcd.clear();
	myLcd.writeChar(8, 1, 'Z');
	
	myLcd.home();
	myLcd.print('X');
	delay(1000);
	
	myLcd.clear();
				  
	// write in some spaces
	myLcd.writeChar(10, 1, 'x');
	delay(500);
	myLcd.writeChar(10, 0, 'a');
	delay(500);
	myLcd.writeChar(15, 1, 'e');
	delay(500);
	myLcd.writeChar(2, 1, '!');
	delay(500);
	myLcd.writeChar(15, 0, 'e');
	delay(500);
	myLcd.writeChar(3, 0, '!');
	delay(500);
	
	myLcd.writeRow0("Repeating");
	myLcd.writeRow1();
	delay(1000);
	myLcd.writeRow0();
}

