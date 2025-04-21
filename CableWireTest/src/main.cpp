#include <Arduino.h>

const int wires = 8;  // Number of wires to test
const int TotalPins = wires * 2;
int NumberFailedConnections = 0;
int NumberShorts = 0;
String shortsList[16];
  // Choose one output pin to test

void printPins();
void resetVoltage();
void applyVoltage(int currentPin);
void setPinModes(int currentPin);
bool checkWireIntegrity(int currentOutputPin);
String checkShorts(int currentOutputPin);
void checkCable();
void resetValues();

void setup() {
  Serial.begin(9600);
}

void loop() {
  checkCable();
  Serial.println("");
  /*
  Serial.println("Check finished!");
  Serial.println("Number of Failed connections: " + String(NumberFailedConnections));
  Serial.println("Number of Shorts: " + String(NumberShorts));
  */
  resetValues();
  delay(800);
}


void checkCable() {
  /*
  Serial.println("");
  Serial.println("Check for internal integrety and shorts started!");
  Serial.println("============================");
  Serial.println("");
  */
  int currentIteration = 0;
  for (int i = 0; i<wires; i++) {
    
    int currentTestPin = A0 + i;
    setPinModes(currentTestPin);
    applyVoltage(currentTestPin);
  
    if (checkWireIntegrity(currentTestPin)) {
      Serial.print("✅");
      //Serial.println("✅  Wire A" + String(currentTestPin - A0) + " --> A" + String(currentTestPin - A0 + 8) + " intact!");
    } else {
      Serial.print("❌");
      //Serial.println("❌  Wire A" + String(currentTestPin - A0) + " --> A" + String(currentTestPin - A0 + 8) + " NOT intact!");
      NumberFailedConnections ++;
    }

    shortsList[currentIteration] = (checkShorts(currentTestPin));
    /*
    Serial.println("");
    Serial.println("============================");
    Serial.println("");
    */
   currentIteration ++;
  }
  Serial.println("");
  for (int i = 0; i<16;i++) {
    if (shortsList[i] != ""){
    Serial.println(shortsList[i]);
  }
}
  
  
}
void resetValues() {
  NumberFailedConnections = 0;
  NumberShorts = 0;
}
// Reset all pins to INPUT mode (no voltage)
void resetVoltage() {
  for (int i = 0; i < TotalPins; i++) {
    digitalWrite(A0 + i, HIGH);
  }
}

// Set the current output pin to OUTPUT, others to INPUT_PULLUP
void setPinModes(int currentPin) {
  for (int i = 0; i < 16; i++) {
    if ((A0 + i) == currentPin) {
      pinMode(A0 + i, OUTPUT);
    } else {
      pinMode(A0 + i, INPUT_PULLUP);
    }
  }
}

// Set the output pin to LOW to test connectivity
void applyVoltage(int currentPin) {
  digitalWrite(currentPin, LOW);
  //Serial.println("Current reading on output pin A" + String(currentPin - A0) + ": " + String(digitalRead(currentPin)));
}

// Check if the corresponding input pin reads LOW (indicating connection)
bool checkWireIntegrity(int currentOutputPin) {
  int pairedInputPin = currentOutputPin + 8;
  return digitalRead(pairedInputPin) == LOW;
}

// Check for unintended connections to other pins (shorts)
String checkShorts(int currentOutputPin) {
  String result = "";
  for (int i = 0; i < 16; i++) {
    int pinToCheck = A0 + i;

    if (pinToCheck == currentOutputPin || pinToCheck == currentOutputPin + 8) {
      continue; // skip correct pair
    }

    int value = digitalRead(pinToCheck);
    if (value == LOW) {
      result += "⚠️ A" + String(currentOutputPin - A0) + " --> A" + String(i);
      NumberShorts ++;
    }
  }
  return result;
}