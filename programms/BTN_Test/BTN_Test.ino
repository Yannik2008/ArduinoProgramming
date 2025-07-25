void setup() {
  // put your setup code here, to run once:
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (LOW == digitalRead(7)) {
    Serial.println("ON");
  } else if(LOW == digitalRead(8)){
    Serial.println("OFF");
  }
}
