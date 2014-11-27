import processing.serial.*;

//colorMode(HSB, 360, 100, 100)
color BACKGROUND = color(0, 0, 0); //black
color ACTUAL  = color(3, 92, 92); //orange
color OLD_POINTS = color(117, 92, 100); //green

float MAXINDEX,PIEARC;
float MINARC = radians(5);
float MAXARC = radians(355);
float MAX_US = 30000.0;

class Measurement {

  int prevIndex;
  int index;
  int distance;
  boolean direction;
  int maxindex;  

  Measurement(int pIndex, int aIndex, boolean dir, float d, int mIndex) {
    prevIndex = pIndex;
    index = aIndex;
    direction = dir;
    distance = d;
    maxindex = mIndex;
  }
}

ArrayList<Measurement> measurements = new ArrayList<Measurement>;

float[] data;

String CMD = "";
boolean direction = false; // true: CW, false: CCW
int index;

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
  colorMode(RGB);

  background();
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
 }
 else if ("M".equals(CMD)) {
   setPIEARC(data[0]);   
 }

 drawData();
 
 CMD = "";
}

/**
*
* decrease the age of measurement
* remove if it is zero
*/
void removeMeasurements() {
  if (measurements.size() > LIMIT_OF_POINTS) {
    measurements.removeRange(0, measurements.size() - LIMIT_OF_POINTS);
  }
}

void processData() {
      boolean prevDirection = direction;
      direction = (data[0] > 0); //CW:1, CCW:0
      int prevIndex = (prevDirection != direction) ? 0 : index;
      index = int(data[1]);

      //float arcLen = (min(width, height) * data[2] / MAX_US * 2.0);
      
      float arcLen = data[2] * 2.0 / MAX_US;
      
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
          
      println(direction?"CW":"CCW",prevIndex, index, arcLen);

      measurements.add(new Measurement(prevIndex, index, ,direction, data[2], MAXINDEX));
}

void drawOneData(int i, boolean last, int pIndex, int index, boolean dir, int distance, int maxindex) {
  float piearc = (MAXARC-MINARC) / maxindex;
  float arcLen = min(width, height) * distance * 2.0 / MAX_US;
  
  float arcRad = (index - pIndex) * piearc;
  float startArc, endArc;

  if (dir) {
    startArc = MINARC + prevIndex * piearc;
    endArc = startArc + arcRad;
  }
  else {
    //arc() needs that startArc < endArc
    endArc = MAXARC - prevIndex * piearc;
    startArc = endArc - arcRad;
  }
  
  /*
  stroke(BACKGROUND);
  fill(BACKGROUND);
  arc(0, 0, min(width, height), min(width, height), startArc, endArc);
  */
  if (arcLen > 0) {
    noStroke();
    fill(117, 92, i);
    println("len, start, end:", arcLen, startArc, endArc);
    arc(0, 0, arcLen, arcLen, startArc, endArc);
  }
  
  if (last) {
    stroke(ACTUAL);
    fill(ACTUAL);
    arc(0, 0, min(width, height), min(width, height), (startArc+endArc)
  }
}

void drawData() {
  for (int i = 0; i < measurements.size(); i++) {
      Measurement m = measurements[i];
      drawOneData(i, i == (measurements.size() - 1), m.prevIndex, m.index,
        m.direction, m.distance, m.maxindex);
  }
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
