#include "packets.h" // TODO

void setup() {
  // debug serial
  Serial.begin(115200);

  // raspi serial (TX1 and RX1 pins)
  Serial1.begin(9600);

  // initialize builtin LED
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // read incoming data
  if(Serial1.available()) {
    uint8_t r = Serial1.read();
    if(r == p_led_high) {
      // set LED high
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("LED high");
    }
    else if(r == p_led_low) {
      // set LED low
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("LED low");
    }
    else {
      // else print char to screen
      Serial.println(r);
    }
  }
}
