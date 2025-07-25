// Global defines
#define                        PIN_CHANNEL_A                2
#define                        PIN_CHANNEL_B                3
#define                        PIN_BUTTON                   10
#define                        PIN_ISR_DEBUG                4
#define                        CHANNEL_ACTIVE               1
#define                        CHANNEL_INACTIVE             0

// Global variables
volatile uint8_t uint8_portstate = 0x00;
volatile int int_TOTALRotation = 0x0000;

void setup() {

  Serial.begin(9600);                                                 //150200 baud

  pinMode(PIN_CHANNEL_A,  INPUT_PULLUP);
  pinMode(PIN_CHANNEL_B,  INPUT_PULLUP);
  pinMode(PIN_BUTTON,     INPUT_PULLUP);
  pinMode(PIN_ISR_DEBUG,  OUTPUT);


  
  // put your setup code here, to run once:
  // Setup Timer 0.125ms 
  // automatic re-start
  TCCR1A  = 0x00;                                                         // sets the Timer control Register to 0 for CTC mode
  TCCR1B  = (1 << WGM12) | (1 << CS10);                                   // WGM12 enables CTC mode  |  Prescaler 8 = bx001 -> (1 << CS10)
  OCR1A   = 2490;                                                         // sets Output Compare Register A of timer 1 to calculated 1250
  TIMSK1  = (1 << OCIE1A);                                                // enables Compare A Match Interrupt for timer 1

  sei();                                                                  // activates Interrupts

  
}


// Setup Interrupt interface
ISR(TIMER1_COMPA_vect) {                                                  // ISR vector declaration
  uint8_t currentCAState = 0;
  int currentCBState = 0;
  static uint8_t uint8_cARegister = 0x00;
  static uint8_t uint8_cBRegister = 0x00;
  static uint8_t int_Channel_A_Status = 0x00;
  static uint8_t int_Channel_B_Status = 0x00;
    
  digitalWrite(PIN_ISR_DEBUG,   HIGH);

  //sample channel
  currentCAState = digitalRead(PIN_CHANNEL_A);
  currentCBState = digitalRead(PIN_CHANNEL_B);

  //update debounce channel 1 Register
  uint8_cARegister <<= 1;
  uint8_cARegister += (HIGH == currentCAState) ? 1 : 0;

  //update debounce channel 2 Register
  uint8_cBRegister <<= 1;
  uint8_cBRegister += (HIGH == currentCBState) ? 1 : 0;
  

  //determine debounced channel A status
  if (0x00 == uint8_cARegister) {
    
    int_Channel_A_Status = CHANNEL_ACTIVE;                            // all bits are 0 --> channel 1 is activated
  } else if (0xFF == uint8_cARegister) {
    
    int_Channel_A_Status = CHANNEL_INACTIVE;                           // all bits are 1 --> channel 1 is de-activated
  }
  
  //determine debounced channel B status
  if (0x00 == uint8_cBRegister) {
    
    int_Channel_B_Status = CHANNEL_ACTIVE;                             // all bits are 0 --> channel 2 is activated
  } else if (0xFF == uint8_cBRegister) {
    
    int_Channel_B_Status = CHANNEL_INACTIVE;                           // all bits are 1 --> channel 2 is de-activated
  }





  //read CA & CB and mask pin D2 and D3 to bit 0/1
  uint8_portstate = (uint8_portstate << 1);
  uint8_portstate += int_Channel_B_Status;

  uint8_portstate = (uint8_portstate << 1);
  uint8_portstate += int_Channel_A_Status;

  uint8_portstate = uint8_portstate & 0b00001111;
  



  //direction logic
  if (uint8_portstate == 0b0010 or uint8_portstate == 0b1101) {
    int_TOTALRotation--;
  } else if (uint8_portstate == 0b0001 or uint8_portstate == 0b1110) {
    int_TOTALRotation++;
  }





  
  digitalWrite(PIN_ISR_DEBUG,   LOW);
}




void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(int_TOTALRotation);
}
