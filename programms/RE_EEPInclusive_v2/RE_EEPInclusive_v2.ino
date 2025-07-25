// version 1.0 
#define                     VERSION               0x01              // Version verification byte for validation and backwards compatibility 

// MAGIC COOKIE of @Wenkain
#define                     MAGIC_COOKIE          0xAFFE            // Validation COOKIE for correct Formating 

// default LENGTH for reset
#define                     DEFAULT_LENGTH        32                // default length for reset
           

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
  EEPROM.put(2, MAGIC_COOKIE);

  // 2. set VERSION
  EEPROM.put(4, VERSION);

  // 3. set LENGTH (default 32 BYTE)
  EEPROM.put(5, 0x00);                                                                            // reset LOWBYTE Length
  EEPROM.put(6, 0x00);                                                                            // reset HIGHBYTE Length
  EEPROM.put(5, DEFAULT_LENGTH);

  // 4. reset data registers 


  // 5. calculate CRC

  // fetch data array
  uint8_t DATA_ARRAY[DEFAULT_LENGTH];
  for (int i = 0; i < DEFAULT_LENGTH; i++) {
    uint8_t TRUE_INDEX = i + 7;
    EEPROM.get(TRUE_INDEX, DATA_ARRAY[i]);
  }

  
  uint8_t calculatedCRC = calculateCRC8(DATA_ARRAY, DEFAULT_LENGTH, CRC_POLYNOMIAL); 
  Serial.print("Calculated CRC: 0x");  
  Serial.println(calculatedCRC, HEX);





  
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

  Serial.begin(9600);                                                 //9600 baud

  
  // pin mode CA & CB INPUT
  DDRE &= 0b11001111; // DDRE &= ~((1<<DDRE4) | (1<<DDRE5));
  
  // pinMode Button input
  DDRB &= 0b11101111; // DDRB &= ~(1<<DDRB4);

  // pinMode pullup CA & CB
  PORTE |= 0b00110000; //(1<<PORTE4) | (1<<PORTE5)

  // pinMode pullup button
  PORTB |= 0b00010000; //(1<<PORTB4)
  
  // pin mode debug pin OUTPUT
  DDRG |= 0b00100000; //(1<<PORTG5)

  // pinMode OUTPUTLEDS
  DDRC |= 0b0011110;

  
  // put your setup code here, to run once:
  // Setup Timer 0.125ms 
  // automatic re-start
  TCCR1A  = (1 << COM1A0);                                                         // sets the Timer control Register to 0 for CTC mode
  TCCR1B  = (1 << WGM12) ;                                   // WGM12 enables CTC mode  |  Prescaler 8 = bx001 -> (1 << CS10)
  OCR1AH   = 0;
  OCR1AL   = 2000;                                                         // sets Output Compare Register A of timer 1 to calculated 1250
  TIMSK1 |= (1 << OCIE1A);                                                // enables Compare A Match Interrupt for timer 1
  TCCR1B |= (1 << CS10);


  // Reset EEPROM
  //EEPROM.put(2, 0x0000);
  //EEPROM.put(0, 0x0000);
  
  // adjusts int_TOTALRotation according to saved data in EEPROM
  //EEPROM.get(0, int16_TOTALRotation);
  //EEPROM.get(2, int16_InternalRotation);
  

  
  sei();                                                                  // activates Interrupts
  
  
}


// Setup Interrupt interface
ISR(TIMER1_COMPA_vect) {                                                  // ISR vector declaration

  //debug pin HIGH
  //digitalWrite(PIN_ISR_DEBUG, HIGH);
  PORTG |= B00100000;
  
  uint8_t currentCAState = 0;
  int currentCBState = 0;
  static uint8_t uint8_cARegister = 0x00;
  static uint8_t uint8_cBRegister = 0x00;
  static uint8_t int_Channel_A_Status = 0x00;
  static uint8_t int_Channel_B_Status = 0x00;
    

  uint8_t uint8_PORTE_DigitalInputs;

  //sample channel
  uint8_PORTE_DigitalInputs = PINE;

//  currentCAState = ((uint8_PORTE_DigitalInputs >> 4) & 0b00000001);
  currentCBState = ((uint8_PORTE_DigitalInputs >> 5) & 0b00000001);

  //update debounce channel 1 Register
  uint8_cARegister <<= 1;
  uint8_cARegister += ((uint8_PORTE_DigitalInputs >> 4) & 0b00000001);

  //update debounce channel 2 Register
  uint8_cBRegister <<= 1;
  uint8_cBRegister += ((uint8_PORTE_DigitalInputs >> 5) & 0b00000001);


//determine debounced channel A status

  uint8_portstate <<= 2;

  if (0x00 == uint8_cARegister) {
      
    uint8_portstate |= 1;
   
  } 
  
  //determine debounced channel B status
  if (0x00 == uint8_cBRegister) {
    
    uint8_portstate |= 2;

  }

  uint8_portstate &= 0b00001111;


  //direction logic
  if ((0b0010 == uint8_portstate) or (0b1101 == uint8_portstate)) {
    int16_TOTALRotation--;
    int16_InternalRotation--;
  } else if ((0b0001 == uint8_portstate) or (0b1110 == uint8_portstate)) {
    int16_TOTALRotation++;
    int16_InternalRotation++;
  }
  //determine internal rotation
  if ((int16_InternalRotation % 30  ) == 0) {
    int16_InternalRotation = 0;
  }


  


  // LED logic
  
  if ((int16_InternalRotation > 4 && int16_InternalRotation <12) or (int16_InternalRotation> -30 && int16_InternalRotation < -19)) {
    PORTC = 0b00010000;
  } else if((int16_InternalRotation > 12 && int16_InternalRotation < 19) or (int16_InternalRotation> -19 && int16_InternalRotation<-12)){
    PORTC = 0b00000100;
  } else if((int16_InternalRotation > 19 && int16_InternalRotation < 27) or (int16_InternalRotation >-12 && int16_InternalRotation<-4)) { 
    PORTC = 0b00000010;
  } else if((int16_InternalRotation > 27 && int16_InternalRotation <30) or (int16_InternalRotation > -4 && int16_InternalRotation<4)) {
   PORTC = 0b00001000; 
  }

  uint8_cycleClock++;
  if (8 == uint8_cycleClock) {
    uint8_cycleClock = 0;
    uint32_Clock++;
  }
  // debug pin LOW
  //digitalWrite(PIN_ISR_DEBUG, LOW);
  PORTG &= 0b11011111;
}
  




void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.println(int16_InternalRotation);
  Serial.print(OCR1A, BIN);
  //EEPROM.put(0, int16_TOTALRotation); // writes int_TOTALRotation to EEPROM[0,1]
  //EEPROM.put(2, int16_InternalRotation); // writes int_InternalRotation to EEPROM[2,3]








  
}
