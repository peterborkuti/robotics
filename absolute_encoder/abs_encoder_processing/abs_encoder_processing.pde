import processing.serial.*;

int BACKGROUND = 100;

int[] COLORS = {255, 0};

int[] bits;
float[] p;

float arc_len;
int map_x;
int map_y;
int map_d;

String CMD = "";

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
  size(640, 360);

  map_x = width / 2;
  map_y = height / 2;
  map_d = min(width, height);

  background(BACKGROUND);
  stroke(BACKGROUND);
}

void setup() {

  port = new Serial(this, getSerialPort(), 9600);
  port.bufferUntil(10); // ASCII linefeed
  
  setupScreen();

}

void draw() {
 if ("P".equals(CMD)) {
   drawProbs();
 } 

 if ("MAP".equals(CMD)) {
   drawMap();
 }
 
 CMD = "";

}

void drawMap() {
  
  arc_len = 2.0 * PI / (float) bits.length;
  
  arc(map_x,map_y, map_d,map_d,arc_len * 2, arc_len * 3, PIE);
  
  for (byte i = 0; i < bits.length; i++) {
    fill(COLORS[bits[i]]);
    //println(map_x+","+ map_y+","+ map_d+","+ map_d+","+ (arc_len * i)+","+ (arc_len * (i + 1) - 1));
    arc(map_x, map_y, map_d, map_d, arc_len * i, arc_len * (i + 1), PIE);  
  }
}

void drawProbs() {
  if (p.length == 0) return;
  arc_len = 2.0 * PI / (float) p.length;
  for (byte i = 0; i < p.length; i++) {
    fill(255 - p[i] * 255);
    //fill(255 - (255.0/p.length) * i);
    //println(map_x+","+ map_y+","+ map_d+","+ map_d+","+ (arc_len * i)+","+ (arc_len * (i + 1) - 1));
    arc(map_x, map_y, map_d * 2 / 3, map_d * 2 / 3, arc_len * i, arc_len * (i + 1), PIE);  
  }
  
  fill(BACKGROUND);
  arc(map_x, map_y, map_d / 3.0, map_d / 3.0, 0, TWO_PI, PIE);  
}

void serialEvent(Serial port) {
  // Data from the Serial port is read in serialEvent() using the read() function and assigned to the global variable: val
  String s = port.readString();
  if (s.startsWith("@")) {
    String[] a = s.substring(1).split(",");
    CMD = a[0].trim();
    println("CMD:" + CMD + ":");
    if ("P".equals(CMD)) {
      println("got p");
      p = new float[a.length - 2];
      for (byte i = 0; i < p.length; i++) {
         p[i] = float(a[i + 1]);
      }
    }
    else if ("MAP".equals(CMD)) {
      println("got map");
      bits = new int[a.length - 2];
      for (byte i = 0; i < bits.length; i++) {
         bits[i] = int(float(a[i + 1]));
      }
    }
    else {
      CMD = "";
    }
  }
  // For debugging
  println( "serial input:" + s);
}
