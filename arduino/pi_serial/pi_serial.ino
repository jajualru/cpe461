#include "packets.h" // TODO

#define DEBUG     (true)

uint8_t packetState = ps_default;

void setup() {
  // debug serial
  Serial.begin(115200);

  // raspi serial (TX1 and RX1 pins)
  Serial1.begin(9600);

  // initialize builtin LED
  pinMode(LED_BUILTIN, OUTPUT);
}

// loop through main functions
void loop() {
  serial();

  // TODO: add other functions here
}

void serial() {
  // check for incoming data
  if(!(Serial1.available())) {
    return;
  }

  // get data
  uint8_t val = Serial1.read();

  if(packetState == 0) {
    // check op code and update state if necessary
    if(val == pop_led_high) {
      digitalWrite(LED_BUILTIN, HIGH);
      printDebug("LED high");
    }
    else if(val == pop_led_low) {
      digitalWrite(LED_BUILTIN, LOW);
      printDebug("LED low");
    }
    // TODO: add other packets here
    else {
      // unrecognized packet
      printDebug("Unrecognized packet: ");
      printDebug(String(val));
    }
  }
  else {
    // TODO: add other states here
  }
}

void printDebug(String str) {
  if(DEBUG) {
    Serial.println(str);
  }
}
