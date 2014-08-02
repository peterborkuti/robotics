size(640, 360);

int BACKGROUND = 100;
background(BACKGROUND);
stroke(BACKGROUND);

int[] COLORS = {255, 0};

byte[] bits = {0,1,1,0,1,0,1,1,1,0,0,1,1,0,1,0,1,0,1};
float[] p = new float[bits.length];

float arc_len = 2.0 * PI / (float) bits.length;
int map_x = width / 2;
int map_y = height / 2;
int map_d = min(width, height);

arc(map_x,map_y, map_d,map_d,arc_len * 2, arc_len * 3, PIE);

for (byte i = 0; i < bits.length; i++) {
  fill(COLORS[bits[i]]);
  println(map_x+","+ map_y+","+ map_d+","+ map_d+","+ (arc_len * i)+","+ (arc_len * (i + 1) - 1));
  arc(map_x, map_y, map_d, map_d, arc_len * i, arc_len * (i + 1), PIE);  
}

fill(BACKGROUND);
arc(map_x, map_y, map_d / 3.0 * 2.0, map_d / 3.0 * 2.0, 0, 2 * PI, PIE);  

for (byte i = 0; i < bits.length; i++) {
  fill(random(255));
  println(map_x+","+ map_y+","+ map_d+","+ map_d+","+ (arc_len * i)+","+ (arc_len * (i + 1) - 1));
  arc(map_x, map_y, map_d / 2, map_d / 2, arc_len * i, arc_len * (i + 1), PIE);  
}

fill(BACKGROUND);
arc(map_x, map_y, map_d / 3.0, map_d / 3.0, 0, 2 * PI, PIE);  


