/*
 * CRC.cpp - Library for creating CRC8 Values for defined Arrays 
 * 
 * 
 * Syntax: calculateCRC8(DataArray, size, Polynomial)
 * Syntax: checkDataIntegrity(DataArray, size, Polynomial)
 * 
 *  DISCLAIMER: DataArray must be data + CRC-Byte (0x00) for correct calculation 
 *  DISCLAIMER: size includes the CRC Value at the last Byte of the array 
 * 
 * Created by Yannik F. Soyka, July 22, 2025.
 * Released into the public domain.
 */





//----------------------------------------includes------------------------------------------------------------//

#include "CRC.h"



//----------------------------------------methodes------------------------------------------------------------//

// calculates the CRC8 Value of given array
uint8_t calculateCRC8(const uint8_t* CRC_DATA_BUFFER, const uint16_t length, const uint8_t CRC_POLYNOMIAL) {
  
  uint8_t   CRC = 0x00;                           // tempory variable for CRC Calculation
  
  for (size_t i = 0; i < length-1; i++) {         // for every byte in the array excluding CRC-BYTE 
    CRC ^= CRC_DATA_BUFFER[i];                    // XOR Check-Byte with current CRC

    for (uint8_t j = 0; j < 8; j++) {             // for every Bit
      
      if (CRC & 0x80) {                           // if current testbit = 1
        CRC = (CRC << 1) ^ CRC_POLYNOMIAL;        // --> XOR current CRC with polynomial
        
      } else {                                    // if current testbit = 0
        CRC <<= 1;                                // skip current bit
        
      }
    }
  }
  return CRC;                                     // return final CRC Value 
}


// checks Data integrity by calculating and comparing the CRC8 value 
bool checkDataIntegrity(const uint8_t* CRC_DATA_BUFFER, const uint16_t length, const uint8_t CRC_POLYNOMIAL) {
  
  // extractes CRC-Byte from DataArray
  uint8_t expected_CRC = CRC_DATA_BUFFER[length-1];     

  // calculates CRC value of given DataArray 
  uint8_t calculated_CRC = calculateCRC8(CRC_DATA_BUFFER, length, CRC_POLYNOMIAL);

  if (calculated_CRC == expected_CRC) {                 // if calculated CRC == extracted CRC
    return true;                                        //  --> intact data
  } else {                                              // else
    return false;                                       //  --> corrupt data
  }
}
