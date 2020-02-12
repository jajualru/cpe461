#include "HC_SR04.h"

#define ULT_INT_PIN     (2)
#define ULT_ECHO_PIN    (ULT_INT_PIN)
#define ULT_TRIG_PIN    (3)

HC_SR04 ult(ULT_TRIG_PIN, ULT_ECHO_PIN, ULT_INT_PIN);

void setup() {
  // init ultrasonic sensor
  ult.begin();

  // init serial
  Serial.begin(115200);
  while(!Serial) continue;

  // start first reading
  ult.start();
}

void loop() {
  // counting variable to prove non-blocking
  static unsigned int i = 0;
  
  // check for completed reading
  if(ult.isFinished()) {
    // print result and count
    Serial.print(ult.getRange());
    Serial.print("cm  ");
    Serial.println(i);

    // start next reading
    ult.start();
  }

  // increment count
  i++;
}
