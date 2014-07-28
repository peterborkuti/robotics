#include <Encoder.h>

const int sensorPin = 8;     // the number of the QRD1114 output pin
const int ledPin =  13;      // the number of the LED pin

long incrementalPos  = -999;
long oldPosition = -999;    // position for debouncing
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 150;    // the debounce time; increase if the output flickers

//localization
const byte LEN = 24;
//1: black - sensor reads 1
//0: white - sensor reads 0
const byte bits[] = {1,0,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,1,0,1,1,1,0,0}; //map
float sensor_right = 0.9;
float p_move = 0.7;

float p[] = {1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
             1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
             1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
             1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
             1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
             1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0};

             
Encoder enc(4, 3);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);

  Serial.begin(9600);
  Serial.println("Encoder Test:");
  sense(readQRD1114());
  showP();
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
      int diff = (incrementalPos < readPosition) ? 1 : -1;
      enc.write(incrementalPos);
      oldPosition = incrementalPos;

      return -diff;
    }
  }
  
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

void moveEncoder(int U) {
  float q[LEN];
  for (byte i = 0; i < LEN; i++) {
    float v_notMove = p[i] * (1.0 - p_move);
    float v_move = p[(i - U) % LEN] * p_move;
    float s = v_notMove + v_move;
    q[i] = s;
  }
  
  for (byte i = 0; i < LEN; i++) {
    p[i] = q[i];
  }
  
}

void showP() {
 for (byte i = 0; i < LEN; i++) {
   Serial.print(bits[i]);
   Serial.print("   ,");
 }

 Serial.println();
  for (byte i = 0; i < LEN; i++) {
   Serial.print(p[i]);
   Serial.print(',');
 }
 Serial.println();
}

void testMove() {
 showP();
 for (byte i = 0; i < LEN; i++) {
   moveEncoder(1);
   showP();
 } 

}

void loop(){
  int moves = readIncEncoder();
  if (moves != 0) {
    moveEncoder(moves);
    int sensed =  readQRD1114();
    sense(sensed);
    Serial.print("Moved:");
    Serial.print(moves);
    Serial.print(", Sensed:");
    Serial.println(sensed);
    showP();
  }  
}
