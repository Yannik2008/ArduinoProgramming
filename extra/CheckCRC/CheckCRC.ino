/*
 * CRC - Library for creating CRC8 Values for defined Arrays 
 * 
 * 
 * Syntax: checkDataIntegrity(DataArray, size, Polynomial)
 * 
 *  DISCLAIMER: DataArray must be data + CRC-Byte (0x00) for correct calculation 
 *  DISCLAIMER: size includes the CRC Value at the last Byte of the array 
 * 
 * Created by Yannik F. Soyka, July 22, 2025.
 * NOT released into the public domain.
 */

#include <CRC.h>
#include <Arduino.h>

//---------------------------------------------Constant & Variable defintions-----------------------------------------------------//

// define datalength for CRC Calculation and Verification
const uint16_t                    CRC_DATA_LENGTH = 10;         

// define Polynomial for CRC Calculation
const uint8_t                     CRC_POLYNOMIAL = 0x7;

// define DataArray (Data + CRC)                                     {--------------------------DATA----------------------,-CRC-}
uint8_t                           CRC_DATA_BUFFER[CRC_DATA_LENGTH] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0xF4};



//---------------------------------------------------Setup------------------------------------------------------------------------//

void setup() {
  // 9600 baud
  Serial.begin(9600);                                       

  // check integrity with CRC methode
  bool Integrity = checkDataIntegrity(CRC_DATA_BUFFER, CRC_DATA_LENGTH, CRC_POLYNOMIAL);

  // return result via Serial monitor
  Serial.print("Dataintegrity: ");
  Serial.println(Integrity ? "true": "false");  
}



//--------------------------------------------------loop--------------------------------------------------------------------------//

void loop() {
  
}
