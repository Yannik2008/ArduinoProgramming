// global defines  
#define                   int_C1PIN                    2                  // PIN D10 
#define                   int_C2PIN                    3                  // PIN A8 
#define                   int_ButtonPIN                10                 // PIN D10
#define                   PIN_INDEX_ISR_DEBUG          4                  // Pin index for debug pulse ISR

#define                   CHANNEL_ACTIVE               0x01
#define                   CHANNEL_INACTIVE             0x00
#define                   SWITCH_ACTIVE                0x01
#define                   SWITCH_INACTIVE              0x00

volatile uint8_t          uint8_c1Register          =  0x00;
volatile uint8_t          uint8_c2Register          =  0x00;
volatile uint8_t          uint8_buttonRegister      =  0x00;              // dynamic button input register 


volatile uint8_t          int_Channel_1_Status      =  0x00;
volatile uint8_t          int_Channel_2_Status      =  0x00;

volatile uint8_t          int_Channel_1_StatusLatest = 0x00;
volatile uint8_t          int_Channel_2_StatusLatest = 0x00;



volatile uint8_t          int_Channel_1_debouncedStatus = 0x00;           // channel 1 status after debouncing 
volatile uint8_t          int_Channel_2_debouncedStatus = 0x00;           // channel 2 status after debouncing 
volatile uint8_t          int_debouncedSwitchStatus     = 0x00;           // button status after debouncing 

 
volatile int16_t          int_TOTALRotation          = 0x0000;
int16_t                   int_latestTOTALRotation    = 0x0000;

void setup() {
  // put your setup code here, to run once:


  //pinModes
  pinMode(int_C1PIN, INPUT_PULLUP);
  pinMode(int_C2PIN, INPUT_PULLUP);
  pinMode(int_ButtonPIN, INPUT_PULLUP);
  pinMode(PIN_INDEX_ISR_DEBUG, OUTPUT);                                   // set output pin for ISR debug signal 


  //SerialMonitor
  Serial.begin(115200);                                                     // 9600 BAUD


  // Setup TMR1 
  //    - 1ms period
  //    - automatic re-start
  TCCR1A  = 0x00;                                                         // sets the Timer control Register to 0 for CTC mode
  TCCR1B  = (1 << WGM12) | (1 << CS11);                                   // WGM12 enables CTC mode  |  Prescaler 64 = bx011 -> (1 << CS11) | (1 << CS10)
  OCR1A   = 249;                                                          // sets Output Compare Register A of timer 1 to calculated 249
  TIMSK1  = (1 << OCIE1A);                                                // enables Compare A Match Interrupt for timer 1

  sei();                                                                  // activates Interrupts
}

// Setup Interrupt interface
ISR(TIMER1_COMPA_vect) {                                                  // ISR vector declaration
  
  
  uint8_t currentC1State = 0;
  int currentC2State = 0;
  int currentSwitchStatus = 0;                                            // reading of switch
  
  digitalWrite(PIN_INDEX_ISR_DEBUG, HIGH);                                // signal ISR start on external pin

  //sample channel
  currentC1State = digitalRead(int_C1PIN);
  currentC2State = digitalRead(int_C2PIN);
  currentSwitchStatus = digitalRead(int_ButtonPIN);                       // read input status

  //update debounce channel 1 Register
  uint8_c1Register <<= 1;
  uint8_c1Register += (HIGH == currentC1State) ? 1 : 0;

  //update debounce channel 2 Register
  uint8_c2Register <<= 1;
  uint8_c2Register += (HIGH == currentC2State) ? 1 : 0;

  //update debounce button Register
  uint8_buttonRegister <<= 1;                                         // shift left by one position
  uint8_buttonRegister += (HIGH == currentSwitchStatus) ? 1 : 0;

  

  //determine debounced channel 1 status
  if (0x00 == uint8_c1Register) {
    
    int_Channel_1_Status = CHANNEL_ACTIVE;                            // all bits are 0 --> channel 1 is activated
  } else if (0xFF == uint8_c1Register) {
    
    int_Channel_1_Status = CHANNEL_INACTIVE;                           // all bits are 1 --> channel 1 is de-activated
  }
  
  //determine debounced channel 2 status
  if (0x00 == uint8_c2Register) {
    
    int_Channel_2_Status = CHANNEL_ACTIVE;                             // all bits are 0 --> channel 2 is activated
  } else if (0xFF == uint8_c2Register) {
    
    int_Channel_2_Status = CHANNEL_INACTIVE;                           // all bits are 1 --> channel 2 is de-activated
  }

  // determine debounced switch status
  if (0x00 == uint8_buttonRegister) {

    int_debouncedSwitchStatus = SWITCH_ACTIVE;                      // all bits are 0 --> switch is activated
  
  } else if (0xFF == uint8_buttonRegister) {
    
    int_debouncedSwitchStatus = SWITCH_INACTIVE;                    // all bits are 1 --> switch is de-activated
  } 

  //rotation direction determination logic
  if (int_Channel_1_Status == int_Channel_2_Status) {
    int_Channel_1_StatusLatest = int_Channel_1_Status;
    int_Channel_2_StatusLatest = int_Channel_2_Status;

  }
  if (int_Channel_1_Status != int_Channel_1_StatusLatest) {
    if (int_Channel_2_Status == int_Channel_2_StatusLatest) {
      int_Channel_1_StatusLatest = int_Channel_1_Status;
      int_TOTALRotation++;

    } 
  }
  else if (int_Channel_2_Status != int_Channel_2_StatusLatest) {
    if (int_Channel_1_Status == int_Channel_1_StatusLatest) {
      int_Channel_2_StatusLatest = int_Channel_2_Status;
      int_TOTALRotation--;


    }
  } 

   //determine button press 
  
  if (SWITCH_ACTIVE == int_debouncedSwitchStatus) { 
    //reset Rotation parameters
    int_TOTALRotation = 0;

  }
 
  
 

  
  digitalWrite(PIN_INDEX_ISR_DEBUG, LOW);                                 // signal ISR end on external pin
}



void loop() {
  // put your main code here, to run repeatedly:
  if (int_TOTALRotation != int_latestTOTALRotation) { // check for updated Activation Count
    
  
  Serial.print("Ges: ");
  Serial.print(int_TOTALRotation);
  Serial.println();
  int_latestTOTALRotation = int_TOTALRotation;
  }
  
}
