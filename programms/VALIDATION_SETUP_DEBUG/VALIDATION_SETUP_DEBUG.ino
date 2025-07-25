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
bool validationDataSetup(uint8_t block) {                                                         // 0 or 1

  // 1. set MAGIC COOKIE
  EEPROM.put(2, reset_MAGIC_COOKIE);

  // 2. set VERSION
  EEPROM.put(4, VERSION);

  // 3. set LENGTH (default 32 BYTE)
  EEPROM.put(5, 0x00);                                                                            // reset LOWBYTE Length
  EEPROM.put(6, 0x00);                                                                            // reset HIGHBYTE Length
  EEPROM.put(5, DEFAULT_LENGTH);

  // 4. reset data registers 
  
  for (int i = 0; i < DEFAULT_LENGTH; i++) {                                                      // for each register important for CRC
    int TRUE_INDEX = i + 128;                                                                       // true register count
    EEPROM.put(TRUE_INDEX, 0x00);                                                                 // reset current data register
  }

  // 5. calculate CRC

  // fetch CRC array
  uint8_t CRC_ARRAY[DEFAULT_LENGTH + Validation_BYTES_LENGTH + 1] = {0};
  for (int i = 0; i < Validation_BYTES_LENGTH; i++) {
    uint8_t TRUE_INDEX = i + 2;
    EEPROM.get(TRUE_INDEX, CRC_ARRAY[i]);
    Serial.print("Fetched at: ");
    Serial.println(TRUE_INDEX);
    Serial.print("VALUE: ");
    Serial.println(CRC_ARRAY[i], BIN);
  }
  
  
  for (int i = 0; i < DEFAULT_LENGTH; i++) {
    uint8_t TRUE_INDEX = i + 128;
    EEPROM.get(TRUE_INDEX, CRC_ARRAY[(i + 6)]);
    Serial.print("Fetched at: ");
    Serial.println(TRUE_INDEX);
    Serial.print("VALUE: ");
    Serial.println(CRC_ARRAY[i + 6]);
  }

  

  uint8_t calculatedCRC = calculateCRC8(CRC_ARRAY, (DEFAULT_LENGTH+Validation_BYTES_LENGTH), CRC_POLYNOMIAL); 
  

  // append CRC to array 
  CRC_ARRAY[DEFAULT_LENGTH+Validation_BYTES_LENGTH] = calculatedCRC;
  Serial.print("CRC INDEX ");
  Serial.print(DEFAULT_LENGTH+Validation_BYTES_LENGTH);
  Serial.print(": ");
  Serial.println(CRC_ARRAY[DEFAULT_LENGTH+Validation_BYTES_LENGTH], HEX);


  // print data array
  for (int i = 0; i < DEFAULT_LENGTH + 6; i++) {
    Serial.print("INDEX ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(CRC_ARRAY[i], DEC);
  }
  // check calculated CRC 
  bool Integrity = checkDataIntegrity(CRC_ARRAY, (DEFAULT_LENGTH+Validation_BYTES_LENGTH), CRC_POLYNOMIAL);
  // return result via Serial monitor
  Serial.print("Dataintegrity: ");
  Serial.println(Integrity ? "true": "false"); 

  
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  validationDataSetup(0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
