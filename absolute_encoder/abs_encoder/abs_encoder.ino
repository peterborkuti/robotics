#include <Encoder.h>

const int sensorPin = 8;     // the number of the QRD1114 output pin
const int ledPin =  13;      // the number of the LED pin

long incrementalPos  = 0;
long oldPosition = 0;    // position for debouncing
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 500;    // the debounce time; increase if the output flickers
long sumDiffIncPos = 0;
long stepCounter = 0;

//localization
const byte LEN = 24;
//1: black - sensor reads 1
//0: white - sensor reads 0
const byte bits[] = {1,0,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,1,0,1,1,1,0,0}; //map
float sensor_right = 0.9;
float p_move = 0.6;
float p_not_move = 0.1;
float p_move_opposit_direction = 1 - p_move - p_not_move;

float p[] = {1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
             1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
             1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
             1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
             1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
             1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0};

const byte sensed[] = {0,0,1,1,1,0,1,1,1,1,0,0,0,0,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,1,0};
             
Encoder enc(4, 3);

void setup() {
  stepCounter = 0;
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);

  Serial.begin(9600);
  Serial.println("Encoder Test:");
  sense(readQRD1114());
  showP();
  enc.write(0);
  //testMove();
  //test();
}

int readQRD1114() {
  int state =  digitalRead(sensorPin);

  digitalWrite(ledPin, state == HIGH);

  return state;
}

void testQRD1114() {
  int state = readQRD1114();

  digitalWrite(ledPin, state == HIGH);
}

int readIncEncoder() {
  long readPosition = enc.read();

  if (oldPosition != readPosition) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    if (incrementalPos != readPosition) {
      sumDiffIncPos += (readPosition - oldPosition);
      int diff = (sumDiffIncPos > 0) ? 1 : -1;
 
      sumDiffIncPos = 0;

      enc.write(0);
      oldPosition = 0;
      incrementalPos = 0;

      return -diff;
    }
  }
  
  sumDiffIncPos += (readPosition - oldPosition);
  oldPosition = readPosition;

  return 0;
}

void testIncEncoder() {
  if (readIncEncoder()) {
    Serial.println(incrementalPos);
  }
  
}

void sense(int Z) {

  float s = 0.0;
  for (byte i = 0; i < LEN; i++) {
    bool hit = (Z == bits[i]);
    float prob = (hit) ? sensor_right : 1 - sensor_right;
    p[i] = p[i] * prob;
    s += p[i]; 
  }

  //normalization
  for (byte i = 0; i < LEN; i++) {
    p[i] = p[i] / s;
  }

}

int getPlace(int i) {
  if (i < 0) return LEN + i;
  if (i >= LEN) return i - LEN;
  return i;
}

void moveEncoder(int U) {
  float q[LEN];
  for (int i = 0; i < LEN; i++) {
    float v_notMove = p[i] * p_not_move;
    float v_move = p[getPlace(i - U)] * p_move;
    float v_move_opposit = p[getPlace(i + U)] * p_move_opposit_direction; 
    float s = v_notMove + v_move + v_move_opposit;
    q[i] = s;
  }
  
  for (byte i = 0; i < LEN; i++) {
    p[i] = q[i];
  }
  
}

void showP() {

 Serial.print("@MAP,");
 for (byte i = 0; i < LEN; i++) {
   Serial.print(bits[i]);
   Serial.print("   ,");
 }
 Serial.println();
 
 Serial.print("@P  ,");
 float mx = 0;
 byte idx = 100;
  for (byte i = 0; i < LEN; i++) {
   if (p[i] > mx) {
      mx = p[i];
      idx = i; 
   }
   Serial.print(p[i]);
   Serial.print(',');
 }
 Serial.println();

 Serial.print("@LOC,");
 for (byte i = 0; i < LEN; i++) {
   if ( i == idx) {
     Serial.print("  * |");
   }
   else {
     Serial.print("    |");
   }
 }
  
 Serial.println();
}

void testMove() {
 for (byte i = 0; i < LEN; i++) {
   p[i] = (float)i;
 }  
 showP();
 for (byte i = 0; i < LEN; i++) {
   moveEncoder(1);
   showP();
 } 

}

void test() {
  for (byte i = 0; i < sizeof(sensed); i++) {
     sense(sensed[i]);
     Serial.print("Sensed:");
     Serial.println(sensed[i]);
     showP();
     moveEncoder(-1);
     Serial.println("Moved:-1");
     showP();   
  }
}


void loop(){
  
  int moves = readIncEncoder();
  if (moves != 0) {
    stepCounter++;
    Serial.print("@STEP,");Serial.println(stepCounter);
    Serial.print("@MOVES,");Serial.println(moves);
    moveEncoder(moves);
    int sensed =  readQRD1114();
    Serial.print("@SENSE,");Serial.println(sensed);
    sense(sensed);
    showP();
  } 
   
}

