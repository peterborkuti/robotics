
const int sensorPin = 8;     // the number of the QRD1114 output pin
const int ledPin =  13;      // the number of the LED pin

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(sensorPin, INPUT);     
}

int readQRD1114() {

  return digitalRead(sensorPin);
}

void testQRD1114() {
  int state = readQRD1114();

  digitalWrite(ledPin, state == HIGH);
}

void loop(){
  testQRD1114();
}
