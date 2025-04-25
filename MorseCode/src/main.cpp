#include <Arduino.h>
int buttonPort1 = A0;
int buttonPort2 = A1;
int diodePort = A2;
int buttonStatus1 = 0;
int buttonStatus2 = 0;
int interruptLetter = 0;
void setup(){
  Serial.begin(9600);
  pinMode(buttonPort1, INPUT);
  pinMode(buttonPort2, INPUT);
  pinMode(diodePort, OUTPUT);
}

void loop(){
  interruptLetter = 0;
  buttonStatus1 = digitalRead(buttonPort1);
  if (buttonStatus1 == HIGH) {
    Serial.println("440");
    digitalWrite(diodePort, HIGH);
    delay(50);
    digitalWrite(diodePort,LOW);
    delay(100);
    buttonStatus2 = digitalRead(buttonPort2);
  } 
  else if (buttonStatus2 == HIGH) {
    Serial.println("Button 2 Pressed!");
    digitalWrite(diodePort, HIGH);
    delay(150);
    digitalWrite(diodePort,LOW);

  }
  else {
    interruptLetter = 1;
    Serial.println("Interrupted!");
    delay(50);
  }
}