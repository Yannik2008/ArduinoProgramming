// version 1.0 
#define                     VERSION               0x01              // Version verification byte for validation and backwards compatibility 

// MAGIC COOKIE of @Wenkain
#define                     MAGIC_COOKIE          0xAFFE            // Validation COOKIE for correct Formating 
#define                     reset_MAGIC_COOKIE    0xFEAF

// default LENGTH for reset
#define                     DEFAULT_LENGTH        32                // default length for reset

// Validation Byte Size fo each block

#define                     Validation_BYTES_LENGTH 5
           

// includes
#include <EEPROM.h>
#include <CRC.h>


// Global defines
#define                        CHANNEL_ACTIVE               1
#define                        CHANNEL_INACTIVE             0
#define                        VALID                        0xA5

// Global variables
volatile uint8_t uint8_portstate = 0x00;
volatile int16_t int16_TOTALRotation;
volatile int16_t int16_InternalRotation;
volatile uint32_t uint32_Clock;
volatile uint8_t uint8_cycleClock;


// variables and constants for CRC Calculation                                             
const uint8_t                           CRC_POLYNOMIAL = 0b100011101;


// reset both data blocks to default
bool validationDataSetup(uint8_t block, void (*setData)()) {                                                         // 0 or 1
  
  // set Validation Byte to INVALID
  EEPROM.put(0+505*block, 0x00);
  
  // 1. set MAGIC COOKIE
  EEPROM.put(2+505*block, reset_MAGIC_COOKIE);

  // 2. set VERSION
  EEPROM.put(4+505*block, VERSION);

  // 3. set LENGTH (default 32 BYTE)
  EEPROM.put(5+505*block, 0x00);                                                                            // reset LOWBYTE Length
  EEPROM.put(6+505*block, 0x00);                                                                            // reset HIGHBYTE Length
  EEPROM.put(5+505*block, DEFAULT_LENGTH);





  // set true data 
    (*setData)();
  
  // 4. calculate CRC

  // fetch CRC array
  uint8_t CRC_ARRAY[DEFAULT_LENGTH + Validation_BYTES_LENGTH + 1] = {0};
  for (int i = 0; i < Validation_BYTES_LENGTH; i++) {
    uint8_t TRUE_INDEX = i + 2 + 505*block;
    EEPROM.get(TRUE_INDEX, CRC_ARRAY[i]);
  }
  
  
  for (int i = 0; i < DEFAULT_LENGTH; i++) {
    uint8_t TRUE_INDEX = i + 128 + 384*block;
    EEPROM.get(TRUE_INDEX, CRC_ARRAY[(i + 6)]);
  }

  

  uint8_t calculatedCRC = calculateCRC8(CRC_ARRAY, (DEFAULT_LENGTH+Validation_BYTES_LENGTH), CRC_POLYNOMIAL); 
  

  // append CRC to array 
  CRC_ARRAY[DEFAULT_LENGTH+Validation_BYTES_LENGTH] = calculatedCRC;


  // check calculated CRC 
  bool Integrity = checkDataIntegrity(CRC_ARRAY, (DEFAULT_LENGTH+Validation_BYTES_LENGTH), CRC_POLYNOMIAL);
  // return result via Serial monitor
  Serial.print("Dataintegrity: ");
  Serial.println(Integrity ? "true": "false"); 
  if (Integrity) {
    EEPROM.put(0 + 505*block, VALID);
  }

  
}

bool checkDataIntegrity(uint8_t block) {
  // check validation 
  uint8_t uint8_MAGICCOOKIEBYTELOW;
  uint8_t uint8_MAGICCOOKIEBYTEHIGH;
  uint8_t uint8_VALIDATION_BYTE;
  uint8_t uint16_MAGICCOOKIEBYTE;
  uint8_t uint8_VERSION_BYTE;
  uint8_t uint8_LENGTHBYTELOW;
  uint8_t uint8_LENGTHBYTEHIGH;
  
  EEPROM.get(block ? (0) : (512), uint8_VALIDATION_BYTE);                                         // get validation byte at position 0 in EEPROM
  
  if (VALID == uint8_VALIDATION_BYTE) {
    EEPROM.get(1, uint8_MAGICCOOKIEBYTELOW);                                 // read LOW Byte of Magic Cookie
    EEPROM.get(2, uint8_MAGICCOOKIEBYTEHIGH);                                 // read HIGH byte of Magic Cookie

    // calculate true magic cookie
    uint16_MAGICCOOKIEBYTE = ((uint16_t)uint8_MAGICCOOKIEBYTEHIGH << 8) + uint8_MAGICCOOKIEBYTELOW;       

    if (MAGIC_COOKIE == uint16_MAGICCOOKIEBYTE) {
      EEPROM.get(3, uint8_VERSION_BYTE);
      if (VERSION == uint8_VERSION_BYTE) {
        EEPROM.get(4, uint8_VERSION_BYTE);
      }
    }
  }
}


void printArray() {
  Serial.println("hello world");
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  validationDataSetup(0, printArray);
}

void loop() {
  // put your main code here, to run repeatedly:

}
