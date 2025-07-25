int pinList[3];
void addSwitchPorts(int count);
void setModes();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  addSwitchPorts(1);
  setModes();
}

void loop() {
  delay(500);
  
  
    if (digitalRead(3) == LOW) {
      Serial.println("Button active");
    } else if (digitalRead(3) == HIGH) {
      Serial.println("Button inactive");
    
  }
}

void addSwitchPorts(int count) {
  memset(pinList, 0, sizeof(pinList));
  for(int i = 0; i < count; i++) {
    pinList[i] = 2+i;
  }
}

void setModes() {
  for (int i: pinList) {
    pinMode(i, INPUT_PULLUP);
  }
}
