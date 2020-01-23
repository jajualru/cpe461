

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <CPLcd.h>


CPLcd::CPLcd(int i2c_addr)
  : LiquidCrystal_I2C (i2c_addr,20,4)
{

}

void CPLcd::init()
{
  LiquidCrystal_I2C::init();
  begin(ROWS,COLUMNS);
  clear();
  backlight();
  home();
}

void CPLcd::home()
{
	setCursor(0,0);
}

void CPLcd::writeWithWrap(const char * aString)
{
	// this writes a string, wrapping to line 1
	// if needed - clears out the line its is writing
	// on.  (.print() is similar but does not blank out
	// the screen.)
	
	uint8_t len = 0;
	
	if (aString != NULL)
	{
		len = strlen(aString);
	}
	
	clear();
	writeRow0(aString);
	
	if (len > 16)
	{
		writeRow1(&aString[16]);
	}
}
void CPLcd::writeChar(uint8_t row, uint8_t column, char aChar)
{
	// allows you to specify where to write and protects from going 
	// of the line.
	
	if (column >= COLUMNS)
		column = 0;
	
	if (row >= ROWS)
		row = 0;
	
	setCursor(row, column);
	print(aChar);
}

void CPLcd::writeRow0(const char * aString)
{
	setCursor(0,0);
	writeRow(aString);
	
}

void CPLcd::writeRow1(const char * aString)
{
	setCursor(0, 1);
	writeRow(aString);
}

void CPLcd::writeString(const char * aString)
{
	// This just prints from current location, it does not 
	// worry about printing past the end of the line
	// Basically same as print() without the wrapping to next line
	
	uint8_t len = strlen(aString);
	uint8_t i = 0;
		
	if (aString == NULL)
	{
		return;
	}
	
	// Not much length protection but better than nothing
	// Do not know where we are starting in the row so length
	// is not much help... but at least its something.
	if (len > ROWS)
	{
		len = ROWS;
	}

	for (i = 0; i < len; i++)
	{
		print(aString[i]);
	}

}

void CPLcd::writeRow(const char * aString)
{
	// Writes aString to current row and then
	// clears out the rest of that row.
	// if aString is NULL will clear the entire row
	uint8_t len = 0;
	uint8_t i = 0;

	if (aString != NULL)
	{
		len = strlen(aString);
	}
	
	if (len > ROWS)
	{
		len = ROWS;
	}

	for (i = 0; i < len; i++)
	{
		print(aString[i]);
	}

	// if aString was null this is going to clear the entire row
	for (; i < ROWS; i++)
	{
		print(" ");
	}
}