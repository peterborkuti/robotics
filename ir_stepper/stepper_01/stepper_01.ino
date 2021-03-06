#include <IRremote.h>

int RECV_PIN = 12;

IRrecv irrecv(RECV_PIN);

decode_results results;

const byte LED = 13;

volatile uint8_t portbhistory = 0xFF;     // default is high because the pull-up
volatile uint8_t portdhistory = 0xFF;
const int STEPPER_DELAY = 4;
volatile bool directionCW = false;
volatile unsigned int stepCounter = 0;
volatile bool state = false;
volatile int stepIndex = 0;
volatile int goalStepCounter = 0;
volatile int stepperDelayCounter = STEPPER_DELAY;

const byte motor_pin_1 = 8;
const byte motor_pin_2 = 10;
const byte motor_pin_3 = 9;
const byte motor_pin_4 = 11;

//pin change interrupt service routine
//for pins in pingroup 0
ISR (PCINT0_vect)
 {
 // handle pin change interrupt for D8 to D13 here
 
 }  // end of PCINT0_vect

//pin change interrupt service routine
//for pins in pingroup 1
ISR (PCINT1_vect)
 {
 // handle pin change interrupt for A0 to A5 here
 }  // end of PCINT1_vect

//pin change interrupt service routine
//for pins in pingroup 2
ISR (PCINT2_vect)
 {
 // handle pin change interrupt for D0 to D7 here
 // http://electronics.stackexchange.com/questions/50380/determining-which-pin-triggered-a-pcintn-interrupt
 
 }  // end of PCINT2_vect

ISR(TIMER1_COMPA_vect)
{
  stepperDelayCounter--;
  if (stepperDelayCounter < 0) {
    stepperDelayCounter = STEPPER_DELAY;
  }
  if (stepperDelayCounter == STEPPER_DELAY - 3) {
     digitalWrite(motor_pin_1, LOW);
     digitalWrite(motor_pin_2, LOW);
     digitalWrite(motor_pin_3, LOW);
     digitalWrite(motor_pin_4, LOW);
  }
  if (stepperDelayCounter > 0) {
    return;
  }
  
  if (goalStepCounter == 0) {
    return;
  }

  directionCW = (goalStepCounter > 0);

  if (directionCW) {
    stepIndex++;
    goalStepCounter--;
    if (stepIndex > 3) { 
      stepIndex = 0;
    }
  }
  else {
    stepIndex--;
    goalStepCounter++;
    if (stepIndex < 0) {
      stepIndex = 3;
    }
  }

  stepCounter++;
  oneStep();
 
  state = !state;  // toggle
  digitalWrite (LED, state ? HIGH : LOW);
}

/*
https://sites.google.com/site/qeewiki/books/avr-guide/external-interrupts-on-the-atmega328

PCICR  Pin Change Interrupt Control Register
PCIFR  Pin Change Interrupt Flag Register
PCMSKn Pin Change Mask Register 0-2

The PCIEx bits in the PCICR registers enable
External Interrupts and tells the MCU to check
PCMSKx on a pin change state.

When a pin changes states (HIGH to LOW, or LOW to HIGH)
and the corresponding PCINTx bit in the PCMSKx register
is HIGH the corresponding PCIFx bit in the PCIFR register
is set to HIGH and the MCU jumps to the corresponding
Interrupt vector.
PORTx is for sending a value out to the port when it is set as an output.
PINx is for use in getting the current value from the port when it is set as an input.
PORTx also sets the state of the internal pull-up resistors when the port is set to input.
And on more recent models, writing to PINx will toggle the value of the pins (the PORTx value) 



  // pin change interrupt (example for D9)
  PCMSK0 |= bit (PCINT1);  // want pin 9
  PCIFR  |= bit (PCIF0);   // clear any outstanding interrupts
  PCICR  |= bit (PCIE0);   // enable pin change interrupts for D8 to D13

PIN       CHANGE NAME(MASK /
D0	  PCINT16 (PCMSK2 / PCIF2 / PCIE2) PORTD, PIND, PD0
D1	  PCINT17 (PCMSK2 / PCIF2 / PCIE2) PORTD, PIND, PD1
D2	  PCINT18 (PCMSK2 / PCIF2 / PCIE2) PORTD, PIND, PD2
D3	  PCINT19 (PCMSK2 / PCIF2 / PCIE2) PORTD, PIND, PD3
D4	  PCINT20 (PCMSK2 / PCIF2 / PCIE2) PORTD, PIND, PD4
D5	  PCINT21 (PCMSK2 / PCIF2 / PCIE2) PORTD, PIND, PD5
D6	  PCINT22 (PCMSK2 / PCIF2 / PCIE2) PORTD, PIND, PD6
D7	  PCINT23 (PCMSK2 / PCIF2 / PCIE2) PORTD, PIND, PD7
D8	  PCINT0  (PCMSK0 / PCIF0 / PCIE0) PORTB, PINB, PB0
D9	  PCINT1  (PCMSK0 / PCIF0 / PCIE0) PORTB, PINB, PB1
D10	  PCINT2  (PCMSK0 / PCIF0 / PCIE0) PORTB, PINB, PB2
D11	  PCINT3  (PCMSK0 / PCIF0 / PCIE0) PORTB, PINB, PB3
D12	  PCINT4  (PCMSK0 / PCIF0 / PCIE0) PORTB, PINB, PB4
D13	  PCINT5  (PCMSK0 / PCIF0 / PCIE0) PORTB, PINB, PB5
A0	  PCINT8  (PCMSK1 / PCIF1 / PCIE1) PORTC, PINC, PC0
A1	  PCINT9  (PCMSK1 / PCIF1 / PCIE1) PORTC, PINC, PC1
A2	  PCINT10 (PCMSK1 / PCIF1 / PCIE1) PORTC, PINC, PC2
A3	  PCINT11 (PCMSK1 / PCIF1 / PCIE1) PORTC, PINC, PC3
A4	  PCINT12 (PCMSK1 / PCIF1 / PCIE1) PORTC, PINC, PC4
A5	  PCINT13 (PCMSK1 / PCIF1 / PCIE1) PORTC, PINC, PC5
*/

void setupPinChangeInterrupt(){
// PCICR  Pin Change Interrupt Control Register
// PCIFR  Pin Change Interrupt Flag Register
// PCMSKn Pin Change Mask Register 0-2

  //set the appropriate pin in the pingroup
  //pingroups: 0, 1, 2
  //D4 - D5 : 
  // pin change interrupt (example for D9)
  PCMSK2 |= bit (PCINT20) | bit (PCINT21);
  PCIFR  |= bit (PCIF2);   // clear any outstanding interrupts
  PCICR  |= bit (PCIE2);   // enable pin change interrupts
}

void setupTimer1Interrupt(unsigned int compare) {
  //http://www.gammon.com.au/forum/?id=11488
  // set up Timer 1
  TCCR1A = 0;          // normal operation
  //TCCR1B = bit(WGM12) | bit(CS10);   // CTC, no pre-scaling
  TCCR1B = bit(WGM12) | bit(CS10) | bit (CS12);   // CTC, scale to clock / 1024
  OCR1A =  compare;       // compare A register value ((compare + 1) * clock speed / 1024)
  TIMSK1 = bit (OCIE1A);             // interrupt on Compare A Match

}  

void setup() {
  Serial.begin(9600);

  irrecv.enableIRIn(); // Start the receiver
   
  pinMode (LED, OUTPUT);

  pinMode (motor_pin_1, OUTPUT);
  pinMode (motor_pin_2, OUTPUT);
  pinMode (motor_pin_3, OUTPUT);
  pinMode (motor_pin_4, OUTPUT);
  
  setupTimer1Interrupt(30);
  //setupPinChangeInterrupt();

}  // end of setup

void oneStep() {
   switch (stepIndex) {
      case 0:    // 1010
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_2, LOW);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, LOW);
      break;
      case 1:    // 0110
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, LOW);
      break;
      case 2:    //0101
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_3, LOW);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 3:    //1001
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_2, LOW);
      digitalWrite(motor_pin_3, LOW);
      digitalWrite(motor_pin_4, HIGH);
      break;
    }
}

int prevStepCounter = 0;

void loop() {
  //digitalWrite(LED, (directionCW) ? HIGH : LOW);
  //pingResult = sonar.ping();
  if (irrecv.decode(&results)) {
    if (results.value == 2581 || results.value == 533) {
      
      Serial.println("L");
      TIMSK1 = 0;
      goalStepCounter-=10;
      TIMSK1 = bit (OCIE1A);
    }
    if (results.value == 2582 || results.value == 534) {
      Serial.println("R");
      TIMSK1 = 0;
      goalStepCounter+=10;
      TIMSK1 = bit (OCIE1A);
    }
    irrecv.resume(); // Receive the next value
  }
  

  if ((goalStepCounter != 0) || (prevStepCounter != stepCounter)) {
    Serial.print("DIR:");
    Serial.print(directionCW ? 1 : 0);
    Serial.print(",SCOUNTER:");
    Serial.print(stepCounter);
    Serial.print(",GOAL:");
    Serial.print(goalStepCounter);
    Serial.print("\n"); // Processing software needs LF
  }
  
  prevStepCounter = stepCounter;
  //Serial.print("Ping:");
  //Serial.println(pingResult);
}
