#include <CPutil.h>

// Encoder State Machine testing


// Put your pins here 
const int encoderPin1 = 2;


void setup() {
  // put your setup code here, to run once:

  // Setup serial monitor
  Serial.begin(9600);
  setupMessage(__FILE__, "Encoder Count Test");
  
  // configure the encoder pin 
  pinMode(encoderPin1, INPUT);
}

void loop() 
{
	processEncoder(); 
}

void processEncoder()
{
	static MSTimer aTimer(1500);
	unsigned long count = 0;
	
	count = encoderCount();
		
	if (aTimer.done())
	{
		aTimer.set(1500);
		print2("Current Count: ", count);
	}
}

unsigned long encoderCount(int resetFlag)
{ 
  int pinValue = 0;
 
  pinValue = digitalRead(encoderPin1);
  
  // Put your encoder state machine here

  // put your return here
  
}

