/*
 * CRC.h - Library for creating CRC8 Values for defined Arrays 
 * 
 * 
 * Syntax: calculateCRC8(DataArray, size, Polynomial)
 * Syntax: checkDataIntegrity(DataArray, size, Polynomial)
 * 
 *  DISCLAIMER: DataArray must be data + CRC-Byte (0x00) for correct calculation 
 *  DISCLAIMER: size includes the CRC Value at the last Byte of the array 
 * 
 * Created by Yannik F. Soyka, July 22, 2025.
 * NOT released into the public domain.
 */


//----------------------------------------headerlines---------------------------------------------------------//
#ifndef CRC_H
#define CRC_H

#include <Arduino.h>

//----------------------------------------methodes------------------------------------------------------------//

// calculates the CRC8 Value of given array
uint8_t calculateCRC8(const uint8_t* CRC_DATA_BUFFER, const uint16_t length, const uint8_t CRC_POLYNOMIAL);

//checks Data integrity by calculating and comparing the CRC8 value 
bool    checkDataIntegrity(const uint8_t* CRC_DATA_BUFFER, const uint16_t length, const uint8_t CRC_POLYNOMIAL);


#endif
