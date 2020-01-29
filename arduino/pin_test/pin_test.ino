#define INPUT_PIN   (2)

void setup() {
  // set up serial connection back to computer
  Serial.begin(115200);

  // set up input pin
  pinMode(INPUT_PIN, INPUT);
}

void loop() {
  // print pin state
  if(digitalRead(INPUT_PIN) == HIGH) {
    Serial.println("HIGH");
  }
  else {
    Serial.println("LOW");
  }

  // delay to avoid spam
  delay(100);
}
