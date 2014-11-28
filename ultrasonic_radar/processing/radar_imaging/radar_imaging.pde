import processing.serial.*;

//colorMode(HSB, 360, 100, 100)
color BACKGROUND = color(0, 0, 0); //black
color ACTUAL  = color(107, 99, 99); //orange


int LIMIT_OF_POINTS = 50;
boolean BEAM = false;
boolean TEST = false;

int MAX_MAXINDEX = 2048;
int MAXINDEX = MAX_MAXINDEX;
float MINARC = radians(5);
float MAXARC = radians(355);
float MAX_US = 30000.0;

int testDirection = 0;
int testIndex = 0;

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
    distance = int(d);
    maxindex = mIndex;
  }
}

ArrayList<Measurement> measurements = new ArrayList<Measurement>();

float[] data;

String CMD = "";
boolean direction = false; // true: CW, false: CCW
int index = 0;

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
  colorMode(HSB, 360, 100, LIMIT_OF_POINTS);

}

void setup() {
  if (!TEST) {
    port = new Serial(this, getSerialPort(), 9600);
    port.bufferUntil(10); // ASCII linefeed
    port.clear();
  }
  setupScreen();
}

void doTest() {
  testIndex = int(testIndex + random(30));
  int dist = int(random(MAX_US));
  String s = "";

  if (abs(testIndex - MAX_MAXINDEX) < 30) {
    s = "M," + testIndex;
    testDirection = 1 - testDirection;
    testIndex = 30;
  }
  else {  
    s = "D," + testDirection + "," + testIndex + "," + dist;
  }

  processArduinoOutput(s);
}

void draw() {
  background(BACKGROUND);

  if (TEST) {
     doTest();     
  }

  translate(width/2, height/2);

 if ("D".equals(CMD)) {
   processData();
 }
 else if ("M".equals(CMD)) {
   MAXINDEX = int(data[0]);   
 }

 drawData();
 println("size:" + measurements.size());
 
 CMD = "";
}

/**
*
* decrease the age of measurement
* remove if it is zero
*/
void removeMeasurements() {
  if (measurements.size() > LIMIT_OF_POINTS) {
   // measurements.subList(0, measurements.size() - LIMIT_OF_POINTS).clear();
  }
}

void processData() {
      boolean prevDirection = direction;
      direction = (data[0] > 0); //CW:1, CCW:0
      int prevIndex = (prevDirection != direction) ? 0 : index;
      index = int(data[1]);

      measurements.add(new Measurement(prevIndex, index, direction, data[2], MAXINDEX));
      if (measurements.size() > LIMIT_OF_POINTS) {
         measurements.remove(0); 
      }
}

void drawOneData(int i, boolean last, int pIndex, int index, boolean dir, int distance, int maxindex) {
  float piearc = (MAXARC-MINARC) / maxindex;
  float arcLen = min(width, height) * distance * 2.0 / MAX_US;
  
  float arcRad = (index - pIndex) * piearc;
  float startArc, endArc;

  if (dir) {
    startArc = MINARC + pIndex * piearc;
    endArc = startArc + arcRad;
  }
  else {
    //arc() needs that startArc < endArc
    endArc = MAXARC - pIndex * piearc;
    startArc = endArc - arcRad;
  }
  
  /*
  stroke(BACKGROUND);
  fill(BACKGROUND);
  arc(0, 0, min(width, height), min(width, height), startArc, endArc);
  */
  if (last) {
    stroke(107, 99, 99);
    fill(107, 99, 99);
    arc(0, 0, min(width, height), min(width, height), (startArc+endArc) / 2 - PI/90, (startArc+endArc) / 2 + PI/90);
  }

  if (arcLen > 0) {
    if (BEAM) {
      noStroke();
      fill(107, 99, i);
    }
    else {  
      stroke(107, 99, i);
      noFill();
    }
    arc(0, 0, arcLen, arcLen, startArc, endArc);
  }
}

void drawData() {
  for (int i = 0; i < measurements.size(); i++) {
      Measurement m = measurements.get(i);
      drawOneData(i, i == (measurements.size() - 1), m.prevIndex, m.index,
        m.direction, m.distance, m.maxindex);
  }
}

void processArduinoOutput(String s) {
  println("PAO:" + s);
  CMD = "";
  if (s.indexOf(",") > 0) {
    String[] a = s.split(",");
    CMD = a[0].trim();
    data = new float[a.length - 1];
    for (byte i = 0; i < data.length; i++) {
       data[i] = float(a[i + 1]);
    }
    // For debugging
    /*
    println( "serial input:" + s + " CMD:" + CMD +
    "," + join(nf(data, 5,0), ", ") );
    */
  }
 }

void serialEvent(Serial port) {
  // Data from the Serial port is read in serialEvent() using the read() function and assigned to the global variable: val
  String s = port.readString();
  if (s != null && !TEST) {
    processArduinoOutput(s.trim());
  }
}
