#include <avr/wdt.h>
#include <SPI.h>
#include "packets.h"
#include "pins.h"

#define DEBUG     (true) // TODO
#define SEND_EN   (49)
#define BUF_SIZE  (100)

// transmission states
#define NO_DATA   (0)
#define DATA      (1)
#define REQUEST   (2)
//#define SENDING   (3) // TODO: remove unused

struct Pin pins[NUM_PINS];

// TODO: refactor into struct in separate file?
volatile uint8_t inputBuf[BUF_SIZE];
volatile uint16_t inputBufStart;          // position of first byte in buffer
volatile uint16_t inputBufLen;            // number of bytes in buffer
volatile uint8_t outputBuf[BUF_SIZE];
volatile uint16_t outputBufStart;         // position of first byte in buffer
volatile uint16_t outputBufLen;           // number of bytes in buffer
volatile uint8_t outputState;            // transmission state

// set up board
void setup() {
  // debug serial
  Serial.begin(115200);
  printDebug("Booting up");

  // initialize spi
  pinMode(50, OUTPUT);
  pinMode(SEND_EN, OUTPUT);
  digitalWrite(SEND_EN, LOW);
  inputBufStart = 0;
  inputBufLen = 0;
  outputBufStart = 0;
  outputBufLen = 0;
  outputState = NO_DATA;
  SPCR |= _BV(SPE); // spi slave mode
  SPCR |= _BV(SPIE); // turn on interrupt

  // initialize builtin LED
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize pin structure
  setupPins();

  // send startup packet
  sendPacket(POP_STARTUP, NULL, 0);
}

// TODO
// spi interrupt routine
ISR(SPI_STC_vect) {
  uint8_t c = SPDR;

  // TODO: check state for data request
  if(outputState == REQUEST) {
    if(outputBufLen == 0) {
      outputState = NO_DATA;
      return;
    }
    
    SPDR = outputBuf[outputBufStart % BUF_SIZE];
    outputBufStart++;
    if(outputBufStart == BUF_SIZE) {
      outputBufStart = 0;
    }
    outputBufLen--;
    return;
  }
  
  // TODO: remove test print
//  Serial.write("Received byte: ");
//  Serial.write(c);
//  Serial.write("\n");

  // check for buffer overflow
  if (inputBufLen >= BUF_SIZE) {
    Serial.write("Error: SPI buffer overflow\n");
    return;
  }

  // TODO
  // add to end of buffer
  inputBuf[(inputBufStart + inputBufLen) % BUF_SIZE] = c;
  inputBufLen++;
}

// loop through main functions
void loop() {
  processSerial();

  // TODO: add other functions here
}

// initialize pin structure
void setupPins() {
  for (int i = 0; i < NUM_PINS; i++) {
    // TODO: fill in default used pins
    pins[i].state = PIN_S_UNUSED;
  }
}

// process serial packets until no more are available
void processSerial() {
  static uint8_t p_data[PACKET_MAX_SIZE]; //Todo: rename to packetData
  static uint8_t p_len = 0;

  // process data until none available
  while (inputBufLen > 0) {
    // read next byte
    p_data[p_len] = inputBuf[inputBufStart % BUF_SIZE];
    p_len++;
    inputBufStart++;
    if(inputBufStart == BUF_SIZE) {
      inputBufStart = 0;
    }
    inputBufLen--;

    // report new packet
    if (p_len == 1) {
      printDebug("New op code: " + String(p_data[0]));
    }

    // TODO: refactor each medium/long case into its own function
    switch (p_data[0]) {
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
        if (p_len < 3) {
          break;
        }

        // check pin is unused
        if (pins[p_data[1]].state != PIN_S_UNUSED) {
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
        if (p_len < 2) {
          break;
        }

        // TODO: save pin # to variable for readability

        // check for valid pin
        if (pins[p_data[1]].state != INPUT) {
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

      case POP_REQUEST:
//        printDebug("Received request"); // TODO: remove print
//        printDebug("outputBufLen: " + String(outputBufLen)); // TODO: remove print

        // wait until not sending data to raspi
        while(outputState == REQUEST);
        
        SPCR &= ~(_BV(SPIE)); // turn off spi interrupt
        SPDR = outputBufLen;
        outputState = REQUEST;
//        while((SPSR & (1<<SPIF))); // TODO: wait for data to load into register?
//        printDebug(String(SPDR)); // TODO: remove
//        delay(20); // TODO: remove
        digitalWrite(SEND_EN, LOW);
        SPCR |= _BV(SPIE); // turn on spi interrupt

        // wait until not sending data to raspi
        while(outputState == REQUEST);
        
        p_len = 0; // end of packet
        break;

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

// TODO: helper function to add a byte to output buffer
void addToOutput(uint8_t msgByte) {
//  SPCR &= ~(_BV(SPIE)); // turn off spi interrupt
  
  if(outputBufLen >= BUF_SIZE) {
    printDebug("Error: Buffer overflow");
    return;
  }

  outputBuf[(outputBufStart + outputBufLen) % BUF_SIZE] = msgByte;
  outputBufLen++;
//  SPCR |= _BV(SPIE); // turn on spi interrupt
}

// queues a packet to be sent to the raspi
// TODO: convert to SPI
// sends a packet to the raspi
void sendPacket(uint8_t op_code, uint8_t* msg, uint8_t msg_len) {
  uint8_t i;
//  printDebug("Queueing"); // TODO: remove

  // wait until not sending data to raspi
  while(outputState == REQUEST);

  SPCR &= ~(_BV(SPIE)); // turn off spi interrupt

  // TODO: write in op code
  addToOutput(op_code);

  // TODO: write in message
  for(i = 0; i < msg_len; i++) {
    addToOutput(msg[i]);
  }

//  printDebug("here0"); // TODO: remove
  digitalWrite(SEND_EN, HIGH);
  outputState = DATA;

  SPCR |= _BV(SPIE); // turn on spi interrupt

  // TODO: remove old comments
    // TODO: prepare data
    // TODO: set transmission pin to high
    // TODO: wait for transmission to complete
    // TODO: repeat for all data
    // TODO: clear flags if necessary
  
  // TODO: remove old code
//  Serial1.write(op_code);
//  Serial1.write(msg, msg_len);
}

// prints a debug message if debugging is enabled
static inline void printDebug(String str) {
  if (DEBUG) {
    Serial.println(str);
  }
}

// software reboots the Arduino
void reboot() {
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1);
}
