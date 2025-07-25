// data buffer for pointer access
uint8_t                           CRC_POLYNOMIAL = 0x11D;
uint8_t                           CRC = 0x00; 



// class for data buffer creation
class DataHandler {
  public:
  uint8_t* data = nullptr;
  size_t size = 0;

  void setSize(size_t newSize) {
    if (data != nullptr) {
      delete[] data;
    }
    size = newSize;
    data = new uint8_t[size];  
  }

  void setValue(size_t index, uint8_t value) {
    if (index < size) {
      data[index] = value;
    }
  }

  uint8_t getValue(size_t index) {
    if (index < size) {
      return data[index];
    }
    return 0;
  }
  void resetValues() {
    for (int i = 0; i < size; i++) {
      data[i] = 0;
    }
  }

  void printValues() {
    for (int i = 0; i < size; i++) {
      Serial.print("Value: ");
      Serial.println(data[i]);
    }
  }
};

DataHandler handler;



void setup() {
  Serial.begin(9600);
  handler.setSize(2);
  handler.resetValues();
  handler.setValue(0, 0b11000010);
  handler.setValue(1, 0b11000010);
  
  handler.printValues();
  //calculate data length
  size_t length = handler.size / sizeof(handler.getValue(0));
  CRC = calculateCRC8(handler.data, length);
  Serial.println(sizeof(handler.data));
  Serial.println(CRC, HEX);
  
}

void loop() {
  // put your main code here, to run repeatedly:
}






void setPolynomial(uint8_t input_POLYNOMIAL) {
  CRC_POLYNOMIAL = input_POLYNOMIAL;
}


void setData(uint8_t input_DATA, uint16_t input_LOCATION) {
  handler.data[input_LOCATION] = input_DATA;
}

uint8_t calculateCRC8(const uint8_t* CRC_DATA_BUFFER, size_t length) {

  for (size_t i = 0; i < length; i++) { 
    CRC ^= CRC_DATA_BUFFER[i];

    for (uint8_t j = 0; j < 8; j++) { 
      
      if (CRC & 0x80) { 
        CRC = (CRC << 1) ^ CRC_POLYNOMIAL;
        
      } else {
        CRC <<= 1;
        
      }
    }
  }
  
  return CRC;
}
