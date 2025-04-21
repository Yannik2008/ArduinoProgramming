#include <Arduino.h>

//========= Variables =========//
const int wires = 8;  // Number of wires to test
const int TotalPins = wires * 2;  // Total number of pins (each wire has 2 pins)
String wireConnectionArray[16];  // Array to store the wire connection statuses (✅ or ❌)
String shortsList[16];  // Array to store any shorts detected between wires


//========= Methode declarations =========//
void applyVoltage(int currentPin);  // Method to apply voltage (set pin to LOW)
void setPinModes(int currentPin);  // Method to set pin modes (OUTPUT for current pin, INPUT_PULLUP for others)
bool checkWireIntegrity(int currentOutputPin);  // Method to check the integrity of the wire (check if pin is connected correctly)
String checkShorts(int currentOutputPin);  // Method to check for shorts between wires
void checkCable();  // Method to check all wires and their connections

//========= Main setup methode declaration =========//
void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
}

//========= Main loop methode declaration =========//
void loop() {
  checkCable();  // Start checking the cable connections and shorts
  Serial.println("");  // Empty line for better separation between checks

  // Print the wire connection statuses (✅ or ❌) in one line
  for (int i = 0; i < wires; i++) {
    Serial.print(wireConnectionArray[i]);  // Print each wire connection result (✅ or ❌)
  }
  Serial.println("");  // Move to the next line after printing all results

  // Print the shorts that were detected underneath the wire connection results
  for (int i = 0; i < wires; i++) {
    if (shortsList[i] != "") {  // Only print shorts if there's any
      Serial.println(shortsList[i]);  // Print the shorts message
    }
  }

  Serial.println("");  // Empty line after printing shorts
  delay(800);  // Delay before performing the next check
}

void checkCable() {
  int currentIteration = 0;  // Initialize iteration counter for wires

  // Iterate through the wires to check for integrity and shorts
  for (int i = 0; i < wires; i++) {
    int currentTestPin = A0 + i;  // Assign the current test pin (A0 to A7 for the wires)
    setPinModes(currentTestPin);  // Set the pin modes (current pin as OUTPUT, others as INPUT_PULLUP)
    applyVoltage(currentTestPin);  // Apply voltage (set the current pin to LOW)

    // Check wire integrity and store result as ✅ or ❌ in wireConnectionArray
    if (checkWireIntegrity(currentTestPin)) {
      wireConnectionArray[currentIteration] = "✅";  // Connection is intact
    } else {
      wireConnectionArray[currentIteration] = "❌";  // Connection failed
    }

    // Check for shorts and add the result to shortsList
    shortsList[currentIteration] = checkShorts(currentTestPin);  // Store any detected shorts

    currentIteration++;  // Move to the next wire for the next iteration
  }
}

// Reset all pins to INPUT mode (no voltage applied)
void resetVoltage() {
  for (int i = 0; i < TotalPins; i++) {
    digitalWrite(A0 + i, HIGH);  // Set all pins to HIGH (no voltage applied)
  }
}

// Set the current output pin to OUTPUT, others to INPUT_PULLUP
void setPinModes(int currentPin) {
  for (int i = 0; i < 16; i++) {
    if ((A0 + i) == currentPin) {
      pinMode(A0 + i, OUTPUT);  // Set the current pin as OUTPUT
    } else {
      pinMode(A0 + i, INPUT_PULLUP);  // Set other pins as INPUT_PULLUP
    }
  }
}

// Set the output pin to LOW to test connectivity (apply voltage)
void applyVoltage(int currentPin) {
  digitalWrite(currentPin, LOW);  // Set the current pin to LOW (apply voltage)
}

// Check if the corresponding input pin reads LOW (indicating the wire is connected correctly)
bool checkWireIntegrity(int currentOutputPin) { 
  int pairedInputPin = currentOutputPin + 8;  // Find the paired input pin (8 pins apart)
  return digitalRead(pairedInputPin) == LOW;  // If paired pin reads LOW, wire is intact
}

// Check for unintended connections to other pins (shorts)
String checkShorts(int currentOutputPin) {
  String result = "";  // String to store any shorts detected

  // Loop through each pin to check if it is shorted with the current output pin
  for (int i = 0; i < 16; i++) {
    int pinToCheck = A0 + i;  // Assign the current pin to check

    // Skip checking the paired pins (current pin and its pair)
    if (pinToCheck == currentOutputPin || pinToCheck == currentOutputPin + 8) {
      continue;  // Skip the current output pin and its paired input pin
    }

    // Check the value on each pin (if it reads LOW, there's a short)
    int value = digitalRead(pinToCheck);
    if (value == LOW) {
      result += "⚠️ A" + String(currentOutputPin - A0) + " --> A" + String(i) + "\n";  // Add short message to result
    }
  }

  return result;  // Return the result of all shorts found
}
