/*

 DHT11 tempperature and humidity sensor
 
  FRONT view
      _______
      |     |
      |     |
      |     |
      |_____|
       1 2 3 4
       + |   GND
         |
   + --5k+---DATA OUT
 

 */






uint16_t DHT11_READ_BYTES()
{
    // BUFFER TO RECEIVE
    //DEBUG_LED = 1;
        int8_t loopCnt;
        const int8_t BIT_START_LENGTH = 4;//length of low before 1 bit data
                                          //value is by experience and it is
                                          //for 8MHz PIC16F684


	// EMPTY BUFFER
	for (uint8_t i=0; i< 5; i++) bits[i] = 0;

	// REQUEST SAMPLE
	TRISC5 = 0;     // Data port is output
        DHT11_DATA = 0;
	__delay_ms(20);
        DHT11_DATA = 1;
	__delay_us(5);
	TRISC5 = 1;     // Data port is input

        //Waiting for DHT11 to start communication
        loopCnt = 10000;
        while(DHT11_DATA && loopCnt--) ;
        if (loopCnt < 0) return 1;

	// ACKNOWLEDGE : low for 80 us and high for 80 us, or TIMEOUT
        loopCnt = 10000;
	while(!DHT11_DATA && loopCnt--) ;
        if (loopCnt < 0) return 2;

        /*
	loopCnt = 10000;
	while(DHT11_DATA && loopCnt--) ;
        if (loopCnt < 0) return 3;
         */

	// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
        // 1 bit:
        //          low : 50 us (= bitStart)
        //         high : 20-30 us -> 0 bit
        //                70-80 us -> 1 bit
	for (uint8_t idx=0; idx<5; idx++) {
            for (int8_t cnt=7; cnt>-1; cnt--) {
                while(DHT11_DATA == 1) ;
		loopCnt = BIT_START_LENGTH;
                //DEBUG_LED = 0;
		while(DHT11_DATA == 0) ;
                //DEBUG_LED = 1;
		while(DHT11_DATA == 1 && loopCnt--) ;
		if (loopCnt < 1) bits[idx] |= (1 << cnt);
            }
	}

	// WRITE TO RIGHT VARS
        // as bits[1] and bits[3] are allways zero they are omitted in formulas.
	humidity    = bits[0];
	temperature = bits[2];

	uint8_t sum = bits[0] + bits[2];

	if (bits[4] != sum) return 6;

	return 0;
}

void DHT11_READ(){

	uint16_t errcode;
        char buf[5];

        DEBUG_LED = 1;
        errcode = DHT11_READ_BYTES();
#ifdef SERIAL_DEBUG
        if (errcode == 1) {
            putStringLn("DHT11 timeout 1");
        }
        if (errcode == 2) {
            putStringLn("DHT11 timeout 2");
        }
        if (errcode == 3) {
            putStringLn("DHT11 timeout 3");
        }
        if (errcode == 6) {
            putStringLn("DHT11 checksum error");
        }

        if (errcode > 16) {
            errcode = errcode % 16;
            int16_t bitnum = errcode/16;
            if (errcode == 5) putString("DHT11 bitstart timeout at bit ");
            if (errcode == 6) putString("DHT11 bit timeout at bit ");
            sprintf(buf,"%d",bitnum);
            putStringLn(buf);
        }
        DEBUG_LED = 0;
        putString("Humidity:");
        sprintf(buf,"%d",humidity);
        putStringLn(buf);

        putString("Temperature:");
        sprintf(buf,"%d",temperature);
        putStringLn(buf);

#endif

}