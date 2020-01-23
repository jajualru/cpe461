void setup() {
  // set up serial connection back to computer
  Serial.begin(115200);

  // set up LED to blink
  pinMode(LED_BUILTIN, OUTPUT);
}

int msg_cnt = 0;
bool led_state = false;
void loop() {
  // increment message count
  msg_cnt++;
  
  // send message to computer
  Serial.print("ping ");
  Serial.println(msg_cnt);

  // toggle LED
  led_state = !led_state;
  if(led_state) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  // wait to avoid spam
  delay(1000);
}
