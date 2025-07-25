// variables and constants for CRC Calculation 
const uint16_t                    CRC_DATA_LENGTH = 9;                                            // define datalength for CRC Calculation and Verification
const uint8_t                     CRC_DATA_BUFFER_IN[] = {0x31,0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};


void setup() {
  Serial.begin(9600);

  //calculate data length
  
  uint8_t CRC_OUT = calculateCRC8(CRC_DATA_BUFFER_IN, CRC_DATA_LENGTH, 0x11D);
  Serial.println(CRC_OUT, HEX);
  
}

void loop() {
  // put your main code here, to run repeatedly:
}

uint8_t calculateCRC8(const uint8_t* CRC_DATA_BUFFER, const uint16_t length, const uint8_t CRC_POLYNOMIAL) {
  
  uint8_t   CRC = 0x00; 

  Serial.println(sizeof(CRC_DATA_BUFFER[0]));
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
