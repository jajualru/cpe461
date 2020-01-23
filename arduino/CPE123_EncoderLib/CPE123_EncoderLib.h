
#ifndef CPE123_ENCODERLIB_H
#define CPE123_ENCODERLIB_H

#include "Arduino.h"

void encoderSetup(int rightEncoderPin1, int rightEncoderPin2, int leftEncoderPin1, int leftEncoderPin2);

unsigned long rightEncoderCount();
unsigned long leftEncoderCount();

void resetEncoders();
void resetRightEncoder();
void resetLeftEncoder();

void leftEncoderISR();
void rightEncoderISR();

#endif