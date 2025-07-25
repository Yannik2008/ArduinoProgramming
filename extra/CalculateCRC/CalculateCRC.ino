/*
 * CRC - Library for creating CRC8 Values for defined Arrays + checking Data Integrity of DataArrays
 * 
 * 
 * Syntax: calculateCRC8(DataArray, size, Polynomial)
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

// define datalength for CRC Calculation
const uint16_t                    CRC_DATA_LENGTH = 10;         

// define Polynomial for CRC Calculation
const uint8_t                     CRC_POLYNOMIAL = 0x7;

// define DataArray (Data + CRC)                                     {--------------------------DATA----------------------,-CRC-}
uint8_t                           CRC_DATA_BUFFER[CRC_DATA_LENGTH] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x00};



//---------------------------------------------------Setup------------------------------------------------------------------------//

void setup() {
  // 9600 baud
  Serial.begin(9600);                                       

  // calculate CRC Value and save in CRC_OUT
  uint8_t CRC_OUT = calculateCRC8(CRC_DATA_BUFFER, CRC_DATA_LENGTH, CRC_POLYNOMIAL);

  // update CRC-Byte in Data array
  CRC_DATA_BUFFER[CRC_DATA_LENGTH-1] = CRC_OUT;

  // return result via Serial monitor
  Serial.print("CRC-8: 0x");
  Serial.println(CRC_DATA_BUFFER[CRC_DATA_LENGTH-1], HEX);
}



//--------------------------------------------------loop--------------------------------------------------------------------------//

void loop() {
  
}
