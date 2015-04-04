#include <IRremote.h>

int RECV_PIN = 12;

IRrecv irrecv(RECV_PIN);

#include <NewPing.h>

#define TRIGGER_PIN  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     3  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 20 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

#define PIRL  11        // Left PIR sensor
#define PIRR  10        // Right PIR sensor

const byte PIR_STATE_NONE = 0;
const byte PIR_STATE_BOTH = 1;
const byte PIR_STATE_LT = 2;
const byte PIR_STATE_RT = 3;

volatile byte PIR_STATE = PIR_STATE_NONE;

decode_results results;

const byte LED = 13;

const int MAX_PWM_SETS = 7;
const int PWM_ZERO_INDEX = 3;
int pwm_sets[7] = {35, 75, 115, 0, 140, 180, 220};

//Motor LEFT
const int MLEFT = 0;
//Motor RIGHT
const int MRIGHT = 1;

const byte MOTOR_STATE_OFF = 0;
const byte MOTOR_STATE_FWD = 1;
const byte MOTOR_STATE_LT = 2;
const byte MOTOR_STATE_RT = 3;
const byte MOTOR_STATE_BWD = 4;

const byte motor_pin_pwm[2] = {5, 6};
const byte motor_pin_direction[2] = {4, 7};

int motor_pwm_index[2] = {PWM_ZERO_INDEX, PWM_ZERO_INDEX};

//pin change interrupt service routine
//for pins in pingroup 0
ISR (PCINT0_vect)
 {
 // handle pin change interrupt for D8 to D13 here
  int pirL = digitalRead(PIRL);
  int pirR = digitalRead(PIRR);

  if ((pirL == HIGH) || (pirR == HIGH)) {
      digitalWrite(LED, HIGH); 
  }
  
  if ((pirL == HIGH) && (pirR == HIGH)) {
     PIR_STATE = PIR_STATE_BOTH;
  }
  else if ((pirL == LOW) && (pirR == LOW)) {
     PIR_STATE = PIR_STATE_NONE;
     digitalWrite(LED, LOW); 
  }
  else if (pirL == HIGH) {
     PIR_STATE = PIR_STATE_LT;
  }
  else {
     PIR_STATE = PIR_STATE_RT;
  }
  
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
//
//
//Pingroup       Pin             Pin
//   0            D10              D11
  PCMSK0 |= bit (PCINT2) | bit (PCINT3);
  PCIFR  |= bit (PCIF0);   // clear any outstanding interrupts
  PCICR  |= bit (PCIE0);   // enable pin change interrupts
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
      Serial.println("motor: accel");
      setMotorRelativeSpeed(MLEFT, 1);
      setMotorRelativeSpeed(MRIGHT, 1);
    }
  }
  else {
    if ((motor_pwm_index[MLEFT] > -1) && (motor_pwm_index[MRIGHT] > -1)) {
      Serial.println("motor: deccel");
      setMotorRelativeSpeed(MLEFT, -1);
      setMotorRelativeSpeed(MRIGHT, -1);
    }
  }
}

void setMotorDirection(bool right) {
  if (right) {
    if ((motor_pwm_index[MRIGHT] > -1) && (motor_pwm_index[MLEFT] <= MAX_PWM_SETS)) {
      Serial.println("motor: right");
      setMotorRelativeSpeed(MRIGHT, 1);
      setMotorRelativeSpeed(MLEFT, -1);
    }
  }
  else {
    if ((motor_pwm_index[MLEFT] > -1) && (motor_pwm_index[MRIGHT] <= MAX_PWM_SETS)) {
      Serial.println("motor: left");
      setMotorRelativeSpeed(MLEFT,  1);
      setMotorRelativeSpeed(MRIGHT,-1);
    }
  }
}

void setMotorRelativeSpeed(byte motor, int relIndex) {
  short index = motor_pwm_index[motor] + relIndex;
  setMotorAbsoluteSpeed(motor, index);
}

byte getMotorState() {
  short indexL = motor_pwm_index[MLEFT];
  short indexR = motor_pwm_index[MRIGHT];
  if ((indexL == PWM_ZERO_INDEX) && (indexR == PWM_ZERO_INDEX)) {
     return MOTOR_STATE_OFF;
  } 
  
  if (indexL == indexR) {
     return (indexL > PWM_ZERO_INDEX) ? MOTOR_STATE_FWD: MOTOR_STATE_BWD;
  }
  
  if (indexL > indexR) {
     return MOTOR_STATE_RT;
  }
  
  return MOTOR_STATE_LT;
  
}


void setMotorAbsoluteSpeed(byte motor, int pwmIndex) {
  if ((pwmIndex < -1) || (pwmIndex > MAX_PWM_SETS)) {
    return;
  }
 
  motor_pwm_index[motor] = pwmIndex;

  if (pwmIndex >= MAX_PWM_SETS) {
    //FFWD MAX SPEED
    digitalWrite(motor_pin_pwm[motor], HIGH);
    digitalWrite(motor_pin_direction[motor], LOW);
  }
  else if (pwmIndex < 0) {
    //BACKWD MAX SPEED
    digitalWrite(motor_pin_pwm[motor], LOW);
    digitalWrite(motor_pin_direction[motor], HIGH);
  }
  else if (pwmIndex == PWM_ZERO_INDEX) {
    //STOP
    digitalWrite(motor_pin_pwm[motor], LOW);
    digitalWrite(motor_pin_direction[motor], LOW);
  }
  else {
    analogWrite(motor_pin_pwm[motor], pwm_sets[pwmIndex]);
    digitalWrite(motor_pin_direction[motor], (pwmIndex > PWM_ZERO_INDEX) ? LOW:HIGH);
  }
}

void setup() {
  Serial.begin(9600);

  irrecv.enableIRIn(); // Start the receiver
   
  pinMode (LED, OUTPUT);
  
  pinMode (PIRL, INPUT);
  pinMode (PIRR, INPUT);
  digitalWrite(PIRL, HIGH);
  digitalWrite(PIRR, HIGH);

  //setupTimer1Interrupt(30);
  setupPinChangeInterrupt();
  setupMotors();
  
}  // end of setup

void remoteControl() {
  if (irrecv.decode(&results)) {
    if (results.value == 2581 || results.value == 533) {//left
      Serial.println("L");
      setMotorDirection(true);
     }
    if (results.value == 2582 || results.value == 534) {//right
      Serial.println("R");
      setMotorDirection(false);
     }
    if (results.value == 2578 || results.value == 530) {//up
      Serial.println("UP");
      setMotorVelocity(true);
    }
    if (results.value == 2579 || results.value == 531) {//down
      Serial.println("DOWN");
      setMotorVelocity(false);
    }
    if (results.value == 2580 || results.value == 532) {//OK, Panic button
      Serial.println("OK");
      stopMotors();
    }
    
    Serial.print("ir received:");
    Serial.println(results.value);
    irrecv.resume(); // Receive the next value
  }
}

void stopMotors() {
   Serial.println("stop motors");
   setMotorAbsoluteSpeed(MLEFT, PWM_ZERO_INDEX);
   setMotorAbsoluteSpeed(MRIGHT, PWM_ZERO_INDEX); 
}

void obstacleAvoidance() {
  
  bool obstacleDetected = (sonar.ping_median(5) > 0);
  
  if (obstacleDetected) {
    Serial.println("Obstacle detected first");
    int i = 0;
    stopMotors();
    setMotorDirection(true);
 
    while ((i < 10) && obstacleDetected) {
      Serial.println("Obstacle detected loop");
      obstacleDetected = (sonar.ping_median(5) > 0);
      i++;
    }
    
    stopMotors();
    if (!obstacleDetected) {
      Serial.println("No obstacle, go forward");
      setMotorVelocity(true);
    }
   }

}


bool livingOnLeft = false;
bool livingOnRight = false;

bool lastStatePIRL = true;
bool lastStatePIRR = true;

void goForward() {
   byte state = getMotorState();
  if (state == MOTOR_STATE_FWD) {
     return;
  } 
  if (state != MOTOR_STATE_OFF) {
     stopMotors();
  }
  setMotorVelocity(true);

}

void goLeft() {
  byte state = getMotorState();
  if (state == MOTOR_STATE_LT) {
     return;
  } 
  if (state != MOTOR_STATE_OFF) {
     stopMotors();
  }
  setMotorDirection(true);
    delay(100);
  stopMotors();
}

void goRight() {
   byte state = getMotorState();
  if (state == MOTOR_STATE_RT) {
     return;
  } 
  if (state != MOTOR_STATE_OFF) {
     stopMotors();
  }
  setMotorDirection(false);
  delay(100);
  stopMotors();
}

void goHalt() {
   byte state = getMotorState();
  if (state == MOTOR_STATE_OFF) {
     return;
  } 
  if (state != MOTOR_STATE_OFF) {
     stopMotors();
  }
}

void loop() {
  
  bool obstacleDetected = (sonar.ping_median(5) > 0);
  byte mState = getMotorState();
  if ((mState == MOTOR_STATE_FWD) && obstacleDetected) {
    stopMotors();
  }
  
  PCICR  &= ~(bit (PCIE0));   // disable pin change interrupts
  byte pState = PIR_STATE;
  PCICR  |= bit (PCIE0);   // enable pin change interrupts
  
  if ((pState == PIR_STATE_BOTH) && !obstacleDetected) {
   goForward(); 
  }
  else if (pState == PIR_STATE_RT) {
   goRight(); 
  }
  else if (pState == PIR_STATE_LT) {
   goLeft(); 
  }
  else if (pState == PIR_STATE_NONE) {
   goHalt(); 
  }
  
  

  /*
  Serial.print("Motor state:");
  Serial.println(getMotorState());
  
  bool obstacleDetected = (sonar.ping_median(5) > 0);
  
  if (obstacleDetected) {
    Serial.println("Obstacle detected");
    stopMotors();
    
    return;
  }

  bool pirL = digitalRead(PIRL);
  bool pirR = digitalRead(PIRR);
  livingOnLeft =  pirL && !lastStatePIRL;
  livingOnRight = pirR && !lastStatePIRR;
  
  lastStatePIRL = pirL;
  lastStatePIRR = pirR;
  
  if (!livingOnLeft && !livingOnRight) {
     Serial.println("no living");
     stopMotors(); 
  }
  else if (livingOnLeft && livingOnRight) {
    Serial.println("livingAhead");
    goForward();
  }
  else if (livingOnRight) {
    Serial.println("livingOnRight");
    goRight();
  }
  else if (livingOnLeft) {
    Serial.println("livingOnLeft");
    goLeft();
  }
  
  if (livingOnLeft || livingOnRight) {
     delay(200            ); 
  }
 
  remoteControl(); 

  */
 }
