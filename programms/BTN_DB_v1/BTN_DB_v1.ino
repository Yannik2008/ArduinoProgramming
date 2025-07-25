// Global Defines
#define         PIN_INDEX_SWITCH        3                             // Pin index for sampling the switch
#define         PIN_INDEX_ISR_DEBUG     4                             // Pin index for debug pulse ISR

#define         SWITCH_ACTIVE           0x01
#define         SWITCH_INACTIVE         0x00


// Global Variables
uint16_t            int_buttonActivationCount = 0x0000;               // define variable for current switch state
uint16_t            int_latestButtonActivationCount = 0x0000;         // define variable for storing latest switch state

volatile uint8_t    uint8_buttonShiftRegister = 0x00;                 // dynamic button input register 
volatile uint8_t    uint8_debouncedSwitchStatus = 0x00;               // button status after debouncing    
      
uint8_t             uint8_latestSwitchStateRegister = 0x00;           // temp register changes storage



void setup() {
  // put your setup code here, to run once:

  // Initialize Serial Interface
  Serial.begin(9600);                                                 // 9600 Baud
  
  // Setup DIOs
  pinMode(PIN_INDEX_ISR_DEBUG, OUTPUT);                               // set output pin for ISR debug signal 
  pinMode(PIN_INDEX_SWITCH, INPUT_PULLUP);                            // set input w/ weak pullup for sampling switch

  // Setup TMR1
  //    - 1ms period
  //    - automatic re-start
  TCCR1A  = 0x00;                                                     // sets the Timer control Register to 0 for CTC mode
  TCCR1B  = (1 << WGM12) | (1 << CS11) | (1 << CS10);                 // WGM12 enables CTC mode  |  Prescaler 64 = bx011 -> (1 << CS11) | (1 << CS10)
  OCR1A   = 249;                                                      // sets Output Compare Register A of timer 1 to calculated 249
  TIMSK1  = (1 << OCIE1A);                                            // enables Compare A Match Interrupt for timer 1

  // inital reading
  Serial.println((int_buttonActivationCount));  


  // Enable Global Interrupts
  sei();                                                              // activates Interrupts
}




// Setup Interrupt interface
ISR(TIMER1_COMPA_vect) {                                              // ISR vector declaration

  uint8_t uint8_currentSwitchState;                                   // reading of switch

  // signal ISR start on external pin
  digitalWrite(PIN_INDEX_ISR_DEBUG, HIGH);                            // toggle Outputpin to HIGH

  // sample switch
  uint8_currentSwitchState = digitalRead(PIN_INDEX_SWITCH);               // read input status

  // adapt debounce shift register
  uint8_buttonShiftRegister <<= 1;                                     // shift left by one position

  uint8_buttonShiftRegister += (HIGH == uint8_currentSwitchState) ? 1 : 0;
                                                                      // set lowest bit if sample of switch is high

  // determine debounced switch status
  if (0x00 == uint8_buttonShiftRegister) {

    uint8_debouncedSwitchStatus = SWITCH_ACTIVE;                      // all bits are 0 --> switch is activated
  
  } else if (0xFF == uint8_buttonShiftRegister) {
    
    uint8_debouncedSwitchStatus = SWITCH_INACTIVE;                    // all bits are 1 --> switch is de-activated
  } 
  
  // signal ISR end on external pin
  digitalWrite(PIN_INDEX_ISR_DEBUG, LOW);                             // toggle Outputpin to LOW

}


void loop() {



    //Serial.println(uint8_debouncedSwitchStatus);                     



  //determine button press 
  
  if ((SWITCH_ACTIVE == uint8_debouncedSwitchStatus) and (uint8_debouncedSwitchStatus != uint8_latestSwitchStateRegister) ) { 
                                                                      // if new button press --> activation count ++
    int_buttonActivationCount ++;
    uint8_latestSwitchStateRegister = uint8_debouncedSwitchStatus;       // update dynamic register changing storage
    
  } else if ((SWITCH_INACTIVE == uint8_debouncedSwitchStatus) and (uint8_debouncedSwitchStatus != uint8_latestSwitchStateRegister) ) {
    uint8_latestSwitchStateRegister = uint8_debouncedSwitchStatus;
  }
 
   if (int_buttonActivationCount != int_latestButtonActivationCount) { // check for updated Activation Count
    Serial.println((int_buttonActivationCount));                     // Output actual Activation Count
    int_latestButtonActivationCount = int_buttonActivationCount;
  }


}
