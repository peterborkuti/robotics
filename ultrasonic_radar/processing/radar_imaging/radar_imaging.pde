import processing.serial.*;

int BACKGROUND = 100;
float MAXINDEX = 2048.0;
float PIEARC = 2.0 * PI / MAXINDEX;
float MAX_US = 30000.0;

int[] COLORS = {255, 0};


float[] data;
float index = 0;

float arc_len;

String CMD = "";
boolean direction = false;

Serial port;

String getSerialPort() {
  String[] ports = Serial.list();
  int i;
  for (i = 0; i < ports.length; i++) {
   if (ports[i].indexOf("USB") > 0) break; 
  }
  
  println("Serial port:" + ports[i]);
  return ports[i];
  
}

void setupScreen() {
  size(400, 400);

  background(BACKGROUND);
  stroke(10);
  fill(50);
}

void setup() {

  port = new Serial(this, getSerialPort(), 9600);
  port.bufferUntil(10); // ASCII linefeed
  port.clear();
  
  setupScreen();

}

int i = 0;

void draw() {
  translate(width/2, height/2);

  //processArduinoOutput("DATA,CW,"+i+","+random(MAX_US));
 if ("DATA".equals(CMD)) {
   processData();
   drawLastData();
 } 

 CMD = "";

}

void processData() {  
  
}

void drawLastData() {  
  index = data[0] * (direction ? 1.0 : -1.0);
  stroke(BACKGROUND);
  fill(BACKGROUND);
  arc(0, 0, min(width, height), min(width, height), index * PIEARC, (index + 10.0) * PIEARC);

  if (data[1] > 0) {
    float len = (min(width, height) * data[1] / MAX_US * 2.0);
    stroke(10);
    fill(50);
    arc(0, 0, len, len, index * PIEARC, (index + 10.0) * PIEARC);
  }
  
}

void processArduinoOutput(String s) {
  if (s.startsWith("DATA")) {
    String[] a = s.split(",");
    if (a.length != 4) {
      println("Length != 4");
      return;
    }
    CMD = a[0].trim();
    if ("DATA".equals(CMD)) {
      direction = ("CCW".equals(a[1])) ? false : true;
      data = new float[a.length - 2];
      for (byte i = 0; i < data.length; i++) {
         data[i] = float(a[i + 2]);
      }
    }
    else {
      CMD = "";
    }
    // For debugging
  println( "serial input:" + s + " CMD:" + CMD + "," + data[0] + "," + data[1]);

  }
 }

void serialEvent(Serial port) {
  // Data from the Serial port is read in serialEvent() using the read() function and assigned to the global variable: val
  String s = port.readString();
  if (s != null) {
    processArduinoOutput(s.trim());
  }

}
