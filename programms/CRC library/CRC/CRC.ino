// variables and constants for CRC Calculation 
const uint16_t                    CRC_DATA_LENGTH = 1;                                            // define datalength for CRC Calculation and Verification
const uint8_t                     CRC_DATA_BUFFER[CRC_DATA_LENGTH] = {0b11000010};

uint8_t                           CRC_OUT = 0x00; 

void setup() {
  Serial.begin(9600);

  //calculate data length
  
  CRC_OUT = calculateCRC8(CRC_DATA_BUFFER, 0x11D);
  Serial.println(CRC_OUT, HEX);
  
}

void loop() {
  // put your main code here, to run repeatedly:
}

uint8_t calculateCRC8(const uint8_t CRC_DATA_BUFFER_PTR[], const uint8_t CRC_POLYNOMIAL) {
  
  uint8_t   CRC = 0x00; 
  
  size_t length = sizeof(CRC_DATA_BUFFER_PTR) / sizeof(CRC_DATA_BUFFER_PTR[0]);
  for (size_t i = 0; i < length; i++) { 
    CRC ^= CRC_DATA_BUFFER_PTR[i];

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


bool checkDataIntegrity(const uint8_t CRC_DATA_BUFFER_PTR[]) {
}
