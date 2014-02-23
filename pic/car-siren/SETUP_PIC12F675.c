void pic_setup(){
	
//**********************************************************************
//                  SETUP PORTS 12F675                                 *
//**********************************************************************



// set up direction of I/O pins
		// PIN8: Vss, PIN1: Vdd
		// 0 = output, 1 = input
        //banksel TRISIO
        //movlw   b'00000000'
	TRISIO=  0b00000000;
        //         UU------
        //         --x-----  PIN2:GP5/T1CKI/OSC1/CLKIN
        //         ---x----  PIN3:GP4/AN3/T1G/OSC2/CLKOUT
        //         ----x---  PIN4:GP3/MCLR/VPP
        //         -----x--  PIN5:GP2/AN2/T0CKI/INT/COUT
        //         ------x-  PIN6:GP1/AN1/CIN-/VREF/ICSPCLK
        //         -------x  PIN7:GP0/AN0/CIN+/ICSPDAT
        //movwf   TRISIO

		// for setting a pin to digital/analog, see ANSEL


//**********************************************************************
//                  SETUP COMPRATOR                                    *
//**********************************************************************

// set up comparator

        // To  use  CIN+  and  CIN-  pins  as  analog
        // inputs,   the   appropriate   bits   must   be
        // programmed in the CMCON (19h) register.

        // Basic mode : GP0 = CIN+,GP1=CIN-
        // With Output: GP2 state = COUT
        // Internal Reference without multiplexing: GP0 as digital pin, GP1 = CIN+
        // Internal Reference with multiplexing: GP0 = CIN+, GP1 = CIN-

        //banksel CMCON
        CMCON=   0b00010111;
	//         U-------          
        //         x-------  COUT: Comparator Output bit 
        //         x-------    When CINV = 0:
        //         x-------      1 = V IN + > V IN -
        //         x-------      0 = V IN + < V IN -
        //         x-------    When CINV = 1:
        //         x-------      1 = V IN + < V IN -
        //         x-------      0 = V IN + > V IN -
        //         -U------  
        //         ---x----  CINV: Comparator Output Inversion bit
        //         ---1----    1 = Output inverted
        //         ---0----    0 = Output not inverted
        //         ----x---  CIS: Comparator Input Switch bit
        //         ----x---    When CM2:CM0 = 110 or 101:  (Multiplexing)
        //         ----1---      1 = V IN - connects to CIN+
        //         ----0---      0 = V IN - connects to CIN-
        //         -----xxx  CM2:CM0: Comparator Mode bits see docs
        //         -----000    Comparator Reset (POR Default Value - low power)
        //         -----001    Comparator with Output
        //         -----010    Comparator without Output
        //         -----011    Comparator with Output and Internal Reference        
        //         -----100    Comparator w/o Output and with Internal Reference, GP1/CIN- analog
        //         -----101    Multiplexed Input with Internal Reference and Output
        //         -----110    Multiplexed Input with Internal Reference      
        //         -----111    Comparator Off (Lowest power)
        //movwf   CMCON

	//	 VRCON — VOLTAGE REFERENCE CONTROL REGISTER (ADDRESS: 99h) 
	//		set: internal voltage reference, half of Vdd
        //banksel VRCON
        VRCON=   0b10001111;
        //         x-------  VREN: CV REF  Enable bit
        //         1-------      1 = CV REF  circuit powered on
        //         0-------      0 = CV REF  circuit powered down, no I DD  drain
        //         -U------
        //         --x-----  VRR: CV REF  Range Selection bit 
        //         --1-----      1 = Low range
        //         --0-----      0 = High range
        //         ---U----
        //         ----xxxx   VR3:VR0: CV REF  value selection 0 ? VR [3:0] ? 15
        //         ----xxxx      When VRR = 1: CV REF  = (VR3:VR0 / 24) * V DD
        //         ----xxxx      When VRR = 0: CV REF  = V DD /4 + (VR3:VR0 / 32) * V DD

	//movwf   VRCON

//**********************************************************************
//                  SETUP ADC                                          *
//**********************************************************************

// set up A/D converter

	// ANS3:ANS0 = 0 : port is digital, 1 : port is analog
        //banksel ANSEL
        ANSEL=   0b00010000;
        //         U-------  not implemented
        //         -xxx----  ADCS2-0	: 001=Focs/8 Conversion Clock
        //         ----x---  ANS3     	: GP4
        //         -----x--  ANS2	: GP2
        //         ------x-  ANS1	: GP1
        //         -------x  ANS0	: GP0
        //movwf   ANSEL
        
        //banksel ADCON0
        ADCON0=  0b10000100;
        //         x-------  ADFM	 : 1=right justified:0=left justified result
        //         -x------  VCFG	 : 0=Vdd is voltage reference
        //         --xx----  		 : not implemented
        //         ----xx--  CHS1-0	 : 00=select channel 0 (GP0)
        //         ------x-  GO/_DONE: 0=A/D conversion not started
        //         -------x  ADON	 : 1=A/D converter module is operating
        //movwf   ADCON0
		//                   CHS1-0:  channel: which pin is connected to the a/d
		//                                     00 - GP0  
		//                                     01 - GP1
		//                                     10 - GP2
		//                                     11 - GP3
		//	STARTING A CONVERSION
		//	The   A/D   conversion   is   initiated   by   setting   the
		//	GO/DONE bit (ADCON0<1>). When the conversion is
		//	complete, the A/D module:
		//	*  Clears the GO/DONE bit
		//	*  Sets the ADIF flag (PIR1<6>)
		//	*  Generates an interrupt (if enabled). 
		//  the result is loaded into   the   ADRESH:ADRESL   registers  






//**********************************************************************
//                  SETUP TIMER                                        *
//**********************************************************************

//setup timer

		asm("clrwdt"); 					//Clear WDT and prescaler

		//BANKSEL	OPTION_REG 	//
        OPTION_REG=0b00000011;
        //           x-------  _GPPU	:	GPIO Pull-up Enable bit
	//	     1-------				1 : GPIO pull-ups are disabled
	//	     0-------			 	0 : GPIO pull-ups are enabled by individual port latch values 
        //           -x------  INTEDG	:	Interrupt Edge Select bit
	//	     -1------				1 = Interrupt on rising edge of GP2/INT pin
	//	     -0------				0 = Interrupt on falling edge of GP2/INT pin
	//	     --x-----	T0CS	:	TMR0 Clock Source Select bit		
	//	     --1-----				1 = Transition on GP2/T0CKI pin
	//	     --0-----				0 = Internal instruction cycle clock (CLKOUT)
	//	     ---x----	T0SE	:	TMR0 Source Edge Select bit	
	//	     ---1----				1 = Increment on high-to-low transition on GP2/T0CKI pin
	//	     ---0----				0 = Increment on low-to-high transition on GP2/T0CKI pin
	//	     ----x---	PSA		: 	Prescaler Assignment bit
	//	     ----1---				1 = Prescaler is assigned to the WDT
	//	     ----0---				0 = Prescaler is assigned to the TIMER0 module
	//	     -----xxx	PS2:PS0	:	Prescaler Rate Select bits
	//					         Bit      TMR0 Rate     WDT Rate
	//       						 000      1 : 2         1 : 1  
	//        						 001      1 : 4         1 : 2  
	//        						 010      1 : 8         1 : 4  
	//        						 011      1 : 16        1 : 8  
	//        						 100      1 : 32        1 : 16 
	//        						 101      1 : 64        1 : 32 
	//        						 110      1 : 128       1 : 64 
	//        						 111      1 : 256       1 : 128
        //movwf   OPTION_REG

	//BANKSEL TMR0
	TMR0=0;            // reset timer (and prescaler!)

//**********************************************************************
//                  SETUP INTERRUPTS                                   *
//**********************************************************************

//setup interrupts
	//BANKSEL	INTCON 	//
        INTCON= 0b00000000;
    	//   	  x-------   GIE: Global Interrupt Enable bit
	//	  1-------	     1 = Enables all unmasked interrupts
	//	  0-------			 0 = Disables all interrupts
    	//        -x------   PEIE: Peripheral Interrupt Enable bit
	//	  -1------	     1 = Enables all unmasked peripheral interrupts
	//	  -0------	     0 = Disables all peripheral interrupts
	//	  --x-----	 T0IE: TMR0 Overflow Interrupt Enable bit
	//	  --1-----	     1 = Enables the TMR0 interrupt
	//	  --0-----	     0 = Disables the TMR0 interrupt
	//	  ---x----	 INTE: GP2/INT External Interrupt Enable bit
	//	  ---1----	     1 = Enables the GP2/INT external interrupt
	//	  ---0----	     0 = Disables the GP2/INT external interrupt
	//	  ----x---	 GPIE: Port Change Interrupt Enable bit	
	//	  ----x---	 	 	IOC register must also be enabled to enable an interrupt-on-change.		
	//	  ----1---	     1 = Enables the GPIO port change interrupt
	//	  ----0---	     0 = Disables the GPIO port change interrupt
	//	  -----x--   T0IF: TMR0 Overflow Interrupt Flag bit
	//	  -----x--   	 	T0IF bit is set when TIMER0 rolls over. TIMER0 is unchanged on RESET and
	//	  -----x--   	 	should be initialized before clearing T0IF bit.
	//	  -----1--       1 = TMR0 register has overflowed (must be cleared in software)
	//	  -----0--       0 = TMR0 register did not overflow
	//	  ------x-   INTF: GP2/INT External Interrupt Flag bit
	//	  ------1-       1 = The GP2/INT external interrupt occurred (must be cleared in software)
	//	  ------0-       0 = The GP2/INT external interrupt did not occur
	//	  -------x   GPIF: Port Change Interrupt Flag bit
	//	  -------1       1 = When at least one of the GP5:GP0 pins changed state (must be cleared in software)
	//	  -------0       0 = None of the GP5:GP0 pins have changed state
	//movwf   INTCON
	
}