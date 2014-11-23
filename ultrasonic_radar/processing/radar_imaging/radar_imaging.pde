import processing.serial.*;

int BACKGROUND = 100;
float MAXINDEX,PIEARC;
float MINARC = radians(5);
float MAXARC = radians(355);
float MAX_US = 30000.0;

int[] COLORS = {255, 0};

float[] data;
float prevIndex = 0;
float index = 0;

float arcLen;

String CMD = "";
boolean prevDirection = false; // true: CW, false: CCW
boolean direction = false;

Serial port;

void setPIEARC(float maxindex) {
  MAXINDEX = maxindex;
  PIEARC = (MAXARC-MINARC) / MAXINDEX;
}

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
  setPIEARC(2048);
  setupScreen();

}

void draw() {
  translate(width/2, height/2);

  //processArduinoOutput("DATA,CW,"+i+","+random(MAX_US));
 if ("D".equals(CMD)) {
   processData();
   drawLastData();
 }
 else if ("M".equals(CMD)) {
   setPIEARC(data[0]);   
 }

 CMD = "";
}

void processData() {
  
      prevDirection = direction;
      direction = (data[0] > 0); //CW:1, CCW:0
      if (direction != prevDirection) {
        if (index > MAXINDEX) {
          MAXINDEX = index;
          PIEARC = (MAXARC-MINARC) / MAXINDEX;
        }
      }

      prevIndex = (prevDirection != direction) ? 0 : index;
      index = data[1];
      arcLen = (min(width, height) * data[2] / MAX_US * 2.0);
      
      println(direction?"CW":"CCW",prevIndex, index, arcLen);
  
}

void drawLastData() {
  float arcRad = (index - prevIndex) * PIEARC;
  float startArc, endArc;
  if (direction) {
    startArc = MINARC + prevIndex * PIEARC;
    endArc = startArc + arcRad;
  }
  else {
    //arc() needs that startArc < endArc
    endArc = MAXARC - prevIndex * PIEARC;
    startArc = endArc - arcRad;
  }
  
  stroke(BACKGROUND);
  fill(BACKGROUND);
  arc(0, 0, min(width, height), min(width, height), startArc, endArc);

  if (arcLen > 0) {
    noStroke();
    fill(50);
    println("len, start, end:", arcLen, startArc, endArc);
    arc(0, 0, arcLen, arcLen, startArc, endArc);
  }
  
}

void processArduinoOutput(String s) {
  CMD = "";
  if (s.indexOf(",") > 0) {
    String[] a = s.split(",");
    CMD = a[0].trim();
    data = new float[a.length - 1];
    for (byte i = 0; i < data.length; i++) {
       data[i] = float(a[i + 1]);
    }
    // For debugging
    println( "serial input:" + s + " CMD:" + CMD +
    "," + join(nf(data, 5,0), ", ") );
  }
 }

void serialEvent(Serial port) {
  // Data from the Serial port is read in serialEvent() using the read() function and assigned to the global variable: val
  String s = port.readString();
  if (s != null) {
    processArduinoOutput(s.trim());
  }

}
