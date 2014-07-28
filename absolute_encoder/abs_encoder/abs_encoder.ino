#include <Encoder.h>

const int sensorPin = 8;     // the number of the QRD1114 output pin
const int ledPin =  13;      // the number of the LED pin

long incrementalPos  = -999;
long oldPosition = -999;    // position for debouncing
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

//localization
const byte LEN = 24;
const char bits[] = {1,0,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,1,0,1,1,1,0,0}; //map
float sensor_right = 0.9;
float p_move = 0.7;

float p[] = {1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
             1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
             1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
             1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
             1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
             1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0};

             
Encoder enc(3,4);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);

  Serial.begin(9600);
  Serial.println("Encoder Test:");  
}

int readQRD1114() {

  return digitalRead(sensorPin);
}

void testQRD1114() {
  int state = readQRD1114();

  digitalWrite(ledPin, state == HIGH);
}

bool readIncEncoder() {
  long readPosition = enc.read();

  if (oldPosition != readPosition) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    if (incrementalPos != readPosition) {
      incrementalPos += (incrementalPos < readPosition) ? 1 : -1;
      enc.write(incrementalPos);
      oldPosition = incrementalPos;

      return true;
    }
  }
  
  oldPosition = readPosition;

  return false;
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

void move(int U) {
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
   Serial.print(p[i]);
   Serial.print(',');
 }
 Serial.println();
}

void testMove() {
 showP();
 for (byte i = 0; i < LEN; i++) {
   move(1);
   showP();
 } 

}

int a = 0;
void loop(){
  //testQRD1114();
  //testIncEncoder();
  if (a == 0) testMove();
  a = 1;
}
