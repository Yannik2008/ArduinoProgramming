#include <CRC.h>
#include <Arduino.h>

const uint16_t                    CRC_DATA_LENGTH = 10;                                            // define datalength for CRC Calculation and Verification
uint8_t                           CRC_DATA_BUFFER[CRC_DATA_LENGTH] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0xF4};
const uint8_t                     CRC_POLYNOMIAL = 0x7;

void setup() {
  Serial.begin(9600);
  //uint8_t CRC_OUT = calculateCRC8(CRC_DATA_BUFFER, CRC_DATA_LENGTH, CRC_POLYNOMIAL);
  //CRC_DATA_BUFFER[CRC_DATA_LENGTH-1] = CRC_OUT;
  //Serial.println(CRC_DATA_BUFFER[CRC_DATA_LENGTH-1], HEX);
  bool Integrity = checkDataIntegrity(CRC_DATA_BUFFER, CRC_DATA_LENGTH, CRC_POLYNOMIAL);
  Serial.println(Integrity);
}

  
void loop() {
  
}
