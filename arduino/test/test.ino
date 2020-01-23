void setup() {
  // setup serial connection back to computer
  Serial.begin(115200);
}

int msg_cnt = 0;
void loop() {
  // increment message count
  msg_cnt++;
  
  // send message to computer
  Serial.print("ping ");
  Serial.println(msg_cnt);

  // wait to avoid spam
  delay(1000);
}
