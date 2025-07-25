#include "Arduino.h"
#include "DataHandler.h"

DataHandler::DataHandler()
{
  
}

void DataHandler::begin()
{
  Serial.begin(9600);
}


void DataHandler::setSize(size_t newSize) 
{
  if (data != nullptr) {
    delete[] data;
  }
  size = newSize;
  data = new uint8_t[size];  
}


void DataHandler::setValue(size_t index, uint8_t value)
{
  if (index < size) {
    data[index] = value;
  }
}


uint8_t Datahandler::getValue(size_t index) 
{
  if (index < size) {
    return data[index];
  }
  return 0;
}


void DataHandler::resetValues() 
{
  for (int i = 0; i < size; i++) {
    data[i] = 0;
  }
}


void DataHandler::printValues() 
{
  for (int i = 0; i < size; i++) {
    Serial.print("Value: ");
    Serial.println(data[i]);
  }
}
