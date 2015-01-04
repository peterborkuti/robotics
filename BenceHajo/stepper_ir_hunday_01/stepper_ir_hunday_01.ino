#include <IRremote.h>

int RECV_PIN = 12;

IRrecv irrecv(RECV_PIN);

decode_results results;

const byte LED = 13;

volatile uint8_t portbhistory = 0xFF;     // default is high because the pull-up
volatile uint8_t portdhistory = 0xFF;
const int STEPPER_DELAY = 5;
volatile bool directionCW = false;
volatile unsigned int stepCounter = 0;
volatile bool state = false;
volatile int stepIndex = 0;
volatile int goalStepCounter = 0;
volatile int stepperDelayCounter = STEPPER_DELAY;
volatile int propellerVelocity = 0;

const int MAX_PROPELLER_VELOCITY = 10;
const byte propeller_pin_pwm = 3;
const byte propeller_pin_direction = 4;

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

void setupPropeller() {
  pinMode(propeller_pin_direction, OUTPUT);
  
  //setup Fast PWN Mode
  pinMode(propeller_pin_pwm, OUTPUT);
  //using digital pin 3 for PWM (TIMER 2)
  //(PCINT19/OC2B/INT1) PD3
  
  //_BV
  //The _BV() is a macro that shifts 1 to left by the specified numnber.
  //Source Code
  //The _BV() is defined in hardware/tools/avr/avr/include/avr/sfr_defs.h, as shown below.
  //#define _BV(bit) (1 << (bit))
  
  //TCCR2A bits:
  //COM2A1 COM2A0 COM2B1 COM2B0 – – WGM21 WGM20
  
  //TCCR2B bits:
  //FOC2A FOC2B – – WGM22 CS22 CS21 CS20 
  
  //WGM2
  //Fast PWM Mode: set WGM22:0 bits in TCCR2A to 011

  //COM2B
  //Non inverted PWM for output B : set COM2B1:0 bits in TCCR2A to 10 
  //(Clear OC2B on Compare Match, set OC2B at BOTTOM)
  //11: Set OC2B on Compare Match, clear OC2B at BOTTOM,(inverting mode)
  //00: Normal port operation, OC2B disconnected
  
  //CS2
  //Clock source and prescaler
  //set CS22:0 bits in TCCR2B to 100 (clk/64 from prescaler)
  
  //Output Compare Unit:
  //Whenever TCNT2 equals OCR2A or OCR2B, the comparator signals a match.
  
  TCCR2B = _BV(CS22);
  OCR2B = 50;
  propellerOff();
  
}

void propellerOff() {
  TCCR2A = 0;
  digitalWrite(propeller_pin_pwm, LOW);
  digitalWrite(propeller_pin_direction, LOW);
}

void propellerForward(byte propellerSpeed) {
  //PWM output non-inverting mode
  TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  digitalWrite(propeller_pin_direction, LOW);
  OCR2B = propellerSpeed;
}

void propellerBackward(byte propellerSpeed) {
  //PWM output inverting mode
  TCCR2A = _BV(COM2B1) | _BV(COM2B0) | _BV(WGM21) | _BV(WGM20);
  digitalWrite(propeller_pin_direction, HIGH);
  OCR2B = propellerSpeed;
}

void setPropellerSpeed(int propVelocity) {
  byte velo = abs(propVelocity) % 256;
  if (propVelocity == 0) {
    propellerOff();
  }
  else if (propVelocity > 0) {
    propellerForward(velo);
  }
  else {
    propellerBackward(velo);
  }
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
      //left
      Serial.println("L");
      TIMSK1 = 0;
      goalStepCounter-=10;
      TIMSK1 = bit (OCIE1A);
    }
    if (results.value == 2582 || results.value == 534) {
      //right
      Serial.println("R");
      TIMSK1 = 0;
      goalStepCounter+=10;
      TIMSK1 = bit (OCIE1A);
    }
    if (results.value == 2578 || results.value == 530) {
      //up
      Serial.println("UP");
      propellerVelocity += 10;
      setPropellerSpeed(propellerVelocity);
    }
    if (results.value == 2579 || results.value == 531) {
      //down
      Serial.println("DOWN");
      propellerVelocity -= 10;
      setPropellerSpeed(propellerVelocity);
    }
    
    Serial.println(results.value);
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
