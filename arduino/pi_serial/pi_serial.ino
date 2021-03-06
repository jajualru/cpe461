#include <avr/wdt.h>
#include "packets.h"
#include "pins.h"

#define DEBUG     (true)

struct Pin pins[NUM_PINS];

// set up board
void setup() {
  // debug serial
  Serial.begin(115200);
  printDebug("Booting up");

  // raspi serial (TX1 and RX1 pins)
  Serial1.begin(19200);

  // initialize builtin LED
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize pin structure
  setupPins();

  // send startup packet
  sendPacket(POP_STARTUP, NULL, 0);
}

// loop through main functions
void loop() {
  processSerial();

  // TODO: add other functions here
  //    - motor control
  //    - long-read sensor data
  //    - LCD communication
}

// initialize pin structure
void setupPins() {
  for(int i = 0; i < NUM_PINS; i++) {
    if(i >= 49) {
      // pins used for communication
      pins[i].state = PIN_S_LOCKED;
    }
    else {
      pins[i].state = PIN_S_UNUSED;
    }
  }
}

// process serial packets until no more are available
void processSerial() {
  static uint8_t p_data[PACKET_MAX_SIZE]; //Todo: rename to packetData
  static uint8_t p_len = 0;
  
  // process data until none available
  while(Serial1.available()) {
    // read next byte
    p_data[p_len] = Serial1.read();
    p_len++;
    
    // report new packet
    if(p_len == 1) {
      printDebug("New op code: " + String(p_data[0]));
    }

  // TODO: refactor each medium/long case into its own function
    switch(p_data[0]) {
      case POP_LED_HIGH:
        digitalWrite(LED_BUILTIN, HIGH);
        printDebug("LED high");
        
        p_len = 0; // end of packet
        break;
        
      case POP_LED_LOW:
        digitalWrite(LED_BUILTIN, LOW);
        printDebug("LED low");
        
        p_len = 0; // end of packet
        break;

      case POP_PING:
        // respond to ping
        sendPacket(POP_PING, NULL, 0);
        printDebug("Responded to ping");

        p_len = 0;
        break;

      case POP_PIN_INIT:
        // skip processing if incomplete packet
        if(p_len < 3) {
          break;
        }
        
        // check pin is unused
        if(pins[p_data[1]].state != PIN_S_UNUSED) {
          // return error message
          uint8_t err_code[2] = {ERR_INVALID_PIN, p_data[1]};
          sendPacket(POP_ERROR, err_code, 2);
          printDebug("Failed to initialize pin");
          
          p_len = 0;
          break;
        }

        // initialize pin
        pins[p_data[1]].state = p_data[2];
        pinMode(p_data[1], p_data[2]);

        // send ack
        sendPacket(POP_PIN_ACK, NULL, 0);
        
        p_len = 0; // end of packet
        break;

      case POP_DIGITAL_READ:
        // skip processing if incomplete packet
        if(p_len < 2) {
          break;
        }

        // TODO: save pin # to variable for readability
        
        // check for valid pin
        if(pins[p_data[1]].state != INPUT) {
          // return error message
          uint8_t err_code[2] = {ERR_INVALID_PIN, p_data[1]};
          sendPacket(POP_ERROR, err_code, 2);
          printDebug("Failed to initialize pin");
          
          p_len = 0;
          break;
        }

        // send response
        uint8_t value[2];
        value[0] = p_data[1];
        value[1] = digitalRead(p_data[1]);
        sendPacket(POP_DIGITAL_DATA, value, 2);

        p_len = 0; // end of packet
        break;

      case POP_RESET:
        printDebug("Received reset");
        delay(100); // delay to allow debug print
        reboot();
      
      // TODO: add other packet types here
      
      default:
        // invalid packet, send error and discard
        uint8_t err_code = ERR_INVALID_PIN;
        sendPacket(POP_ERROR, &err_code, 1);
        printDebug("Invalid op code " + String(p_data[0]));
        
        p_len = 0; // discard op code
    }
  }
}

// sends a packet to the raspi
void sendPacket(uint8_t op_code, uint8_t* msg, uint8_t msg_len) {
  Serial1.write(op_code);
  Serial1.write(msg, msg_len);
}

// prints a debug message if debugging is enabled
static inline void printDebug(String str) {
  if(DEBUG) {
    Serial.println(str);
  }
}

// software reboots the Arduino
void reboot() {
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while(1);
}
