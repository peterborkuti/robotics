/*
 WTV020-SD-MINI
 TWO LINE SERIAL MODE

1  RESET 1      o     16  VDD+
2        2            15  BUSY
3        3            14
4  SPK+  4            13
5  SPK-  5            12
6        6            11
7  CLOCK 7            10  DATAINPUT
8  GND   8             9
 *


*/

#define LOW     0
#define HIGH    1
#define WTV020_VOLUME_MAX   0xFFF7

void WTV020_sendCommand(uint16_t command) {
  //Start bit Low level pulse for 2 ms
    WTV020_CLK = LOW;
    __delay_ms(2); //START bit wait time
    for (uint8_t i=0;i<16;i++) { // MSB first, 16 bits
      __delay_us(100);
      WTV020_CLK = LOW;
      __delay_us(50);
      WTV020_DATA = ((command & 0x8000) == 0x8000);
      __delay_us(50);
      WTV020_CLK = HIGH;
      command <<= 1;
    }
    __delay_ms(50); //STOP bit wait time (2 ms) and wait for BUSY
  while (WTV020_BUSY) {__delay_us(20);};
    __delay_ms(50); //WAIT for ready for next command
  WTV020_DATA = HIGH;
  WTV020_CLK = HIGH;
}


void WTV020_reset(){
  WTV020_CLK =  LOW;
  WTV020_RESET = HIGH;
  //Reset pulse.
  WTV020_RESET = LOW;
  __delay_ms(5);
  WTV020_RESET = HIGH;
  //Reset idle to start bit.
  WTV020_CLK = HIGH;
  WTV020_DATA = HIGH;
  __delay_ms(300);

}
