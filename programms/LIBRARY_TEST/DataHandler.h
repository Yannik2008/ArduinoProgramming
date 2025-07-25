/*
 DataHandler - Library for creating and editing uint8_t Arrays with specified length 
 Created by Yannik F. Soyka, July 21, 2025'
 Not released into the public domain.
 */

#ifndef DataHandler
#define DataHandler

#include <Arduino.h>

class DataHandler
{
  public:
    DataHandler();
    void begin();
    void setSize(size_t newSize);
    void setValue(size_t index, uint8_t value);
    uint8_t getValue(size_t index);
    void resetValues();
    void printValues();

}

#endif
