volatile int pin = A0;
volatile boolean bool_currentMode = false;
volatile int int_ocrValue;
volatile int int_prescaler = 64;

void setup() {
  pinMode(pin, OUTPUT);                                   //sets the outputpin 

  //enables timer module to interrupt every 1ms
  TCCR1A = 0;                                             //sets the Timer control Register to 0 for CTC mode
  TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);     //WGM12 enables CTC mode  |  Prescaler 64 = bx011 -> (1 << CS11) | (1 << CS10)
  OCR1A = 2.49;                                           //sets Output Compare Register A of timer 1 to calculated 249
  TIMSK1 |= (1 << OCIE1A);                                //enables Compare A Match Interrupt for timer 1
  sei();                                                  //activates Interrupts
}

//interrupt module
ISR(TIMER1_COMPA_vect) {                                  // ISR vector declaration 
  digitalWrite(pin, HIGH);                   //toggles outputpin according to global variable
  asm
  (
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
  );
  digitalWrite(pin, LOW);                   
}

void loop() {
  // put your main code here, to run repeatedly:
}
