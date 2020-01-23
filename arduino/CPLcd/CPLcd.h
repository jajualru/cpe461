
#ifndef CPLCD_H
#define CPLCD_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ROWS 16
#define COLUMNS 2

#define DEFAULT_I2C_ADDR 0x27

class CPLcd : public LiquidCrystal_I2C
{
  public:

  CPLcd(int i2c_addr = DEFAULT_I2C_ADDR);
  void init();
 
  void home();
  void writeChar(uint8_t row, uint8_t column, char achar);
  void writeWithWrap(const char * aString);
  void writeRow0(const char * aString = NULL);
  void writeRow1(const char * aString = NULL);
  void writeString(const char * aString);
  void writeRow(const char * aString = NULL);
};


#endif