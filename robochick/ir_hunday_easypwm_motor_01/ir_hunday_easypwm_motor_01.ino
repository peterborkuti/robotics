#include <IRremote.h>

int RECV_PIN = 12;

IRrecv irrecv(RECV_PIN);

decode_results results;

const byte LED = 13;

const int MAX_PWM_SETS = 7;
const int PWM_ZERO_INDEX = 3;
int pwm_sets[7] = {35, 75, 115, 0, 140, 180, 220};

//Motor LEFT
const int MLEFT = 0;
//Motor RIGHT
const int MRIGHT = 1;
const byte motor_pin_pwm[2] = {5, 6};
const byte motor_pin_direction[2] = {4, 7};

int motor_pwm_index[2] = {PWM_ZERO_INDEX, PWM_ZERO_INDEX};

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

void setupMotors() {
  pinMode(motor_pin_direction[MLEFT], OUTPUT);
  pinMode(motor_pin_direction[MRIGHT], OUTPUT);
  
  pinMode(motor_pin_pwm[MLEFT], OUTPUT);
  pinMode(motor_pin_pwm[MRIGHT], OUTPUT);

  setMotorAbsoluteSpeed(MLEFT, PWM_ZERO_INDEX);
  setMotorAbsoluteSpeed(MRIGHT, PWM_ZERO_INDEX); 
}

void setMotorVelocity(bool up) {
  if (up) {
    if ((motor_pwm_index[MLEFT] <= MAX_PWM_SETS) && (motor_pwm_index[MRIGHT] <= MAX_PWM_SETS)) {
      setMotorRelativeSpeed(MLEFT, 1);
      setMotorRelativeSpeed(MRIGHT, 1);
    }
  }
  else {
    if ((motor_pwm_index[MLEFT] > -1) && (motor_pwm_index[MRIGHT] > -1)) {
      setMotorRelativeSpeed(MLEFT, -1);
      setMotorRelativeSpeed(MRIGHT, -1);
    }
  }
}

void setMotorDirection(bool right) {
  if (right) {
    if ((motor_pwm_index[MRIGHT] > -1) && (motor_pwm_index[MLEFT] <= MAX_PWM_SETS)) {
      setMotorRelativeSpeed(MRIGHT, 1);
      setMotorRelativeSpeed(MLEFT, -1);
    }
  }
  else {
    if ((motor_pwm_index[MLEFT] > -1) && (motor_pwm_index[MRIGHT] <= MAX_PWM_SETS)) {
      setMotorRelativeSpeed(MLEFT,  1);
      setMotorRelativeSpeed(MRIGHT,-1);
    }
  }
}

void setMotorRelativeSpeed(byte motor, int relIndex) {
  short index = motor_pwm_index[motor] + relIndex;
  setMotorAbsoluteSpeed(motor, index);
}


void setMotorAbsoluteSpeed(byte motor, int pwmIndex) {
  Serial.print("pwmIndex:");
  Serial.println(pwmIndex);
  
  if ((pwmIndex < -1) || (pwmIndex > MAX_PWM_SETS)) {
    return;
  }
  Serial.print('motor:');
  Serial.print(motor);
 
  motor_pwm_index[motor] = pwmIndex;

  if (pwmIndex >= MAX_PWM_SETS) {
    //FFWD MAX SPEED
    Serial.println('FFWD');
    digitalWrite(motor_pin_pwm[motor], HIGH);
    digitalWrite(motor_pin_direction[motor], LOW);
  }
  else if (pwmIndex < 0) {
    //BACKWD MAX SPEED
    Serial.println('BWRD');
    digitalWrite(motor_pin_pwm[motor], LOW);
    digitalWrite(motor_pin_direction[motor], HIGH);
  }
  else if (pwmIndex == PWM_ZERO_INDEX) {
    //STOP
    Serial.println('STOP');
    digitalWrite(motor_pin_pwm[motor], LOW);
    digitalWrite(motor_pin_direction[motor], LOW);
  }
  else {
    Serial.print('PWM');
    analogWrite(motor_pin_pwm[motor], pwm_sets[pwmIndex]);
    digitalWrite(motor_pin_direction[motor], (pwmIndex > PWM_ZERO_INDEX) ? LOW:HIGH);
  }
}

void setup() {
  Serial.begin(9600);

  irrecv.enableIRIn(); // Start the receiver
   
  pinMode (LED, OUTPUT);

  //setupTimer1Interrupt(30);
  //setupPinChangeInterrupt();
  setupMotors();
  
}  // end of setup

void loop() {
  
  //digitalWrite(LED, (directionCW) ? HIGH : LOW);
  //pingResult = sonar.ping();
  if (irrecv.decode(&results)) {
    if (results.value == 2581 || results.value == 533) {
      //left
      Serial.println("L");
      setMotorDirection(false);
     }
    if (results.value == 2582 || results.value == 534) {
      //right
      Serial.println("R");
      setMotorDirection(true);
     }
    if (results.value == 2578 || results.value == 530) {
      //up
      Serial.println("UP");
      setMotorVelocity(true);
    }
    if (results.value == 2579 || results.value == 531) {
      //down
      Serial.println("DOWN");
      setMotorVelocity(false);
    }
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    
    Serial.println(results.value);
    irrecv.resume(); // Receive the next value
  }
 
  
  //Serial.print("Ping:");
  //Serial.println(pingResult);
}
