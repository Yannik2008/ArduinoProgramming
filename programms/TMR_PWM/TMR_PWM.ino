#define                   TIME_TOTAL                  50
uint16_t                  TIME_ON             =       10;
uint16_t                  TIME_OFF            =       500;
uint16_t                    CURRENT_TIME        =       0;

void setup() {
  
  DDRK |= (1 << 1) | (1 << 2);
  //enables timer module to interrupt every 1ms
  TCCR1A = 0;                                             //sets the Timer control Register to 0 for CTC mode
  TCCR1B |= (1 << WGM12) | (1 << CS10);                   //WGM12 enables CTC mode  |  Prescaler 8 = bx001 -> (1 << CS10)
  TCCR1B &= ~((1<<CS11) | (1 << CS12));
  OCR1A = 1999;                                           //sets Output Compare Register A of timer 1 to calculated 1999
  TIMSK1 |= (1 << OCIE1A);                                //enables Compare A Match Interrupt for timer 1
  sei();                                                  //activates Interrupts
}

//interrupt module
ISR(TIMER1_COMPA_vect) {                                  // ISR vector declaration
   
  PORTK |= (1 << 2);                                      //toggles outputpin DEBUG (A10) ON
  
  if (TIME_TOTAL == CURRENT_TIME) {
    CURRENT_TIME = 1;
  }
  



  if (CURRENT_TIME <= TIME_ON) {
  PORTK |= (1 << 1);                                      //toggles outputpin A9 ON

  } else {
  PORTK &= ~(1 << 1);                                     //toggles outputpin A9 OFF
  }

  
  CURRENT_TIME++;
  PORTK &= ~(1 << 2);                                     //toggles outputpin DEBUG (A10) OFF
                
}

void loop() {
  // put your main code here, to run repeatedly:
}


void calculateTimeOFF(uint16_t TIMEON) {
  TIME_OFF = TIME_TOTAL - TIMEON;
}
