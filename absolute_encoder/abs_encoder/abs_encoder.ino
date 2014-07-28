#include <Encoder.h>

const int sensorPin = 8;     // the number of the QRD1114 output pin
const int ledPin =  13;      // the number of the LED pin

long incrementalPos  = -999;
long oldPosition = -999;    // position for debouncing
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers


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
void loop(){
  //testQRD1114();
  testIncEncoder();
}
