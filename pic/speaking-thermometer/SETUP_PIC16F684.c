//**********************************************************************
//                                                                     *
//                      SETUP and INIT PIC16F684                       *
//                                                                     *
//**********************************************************************

void pic_init(){

//**********************************************************************
//                  SETUP PORTS                                        *
//**********************************************************************



// set up direction of PORTA I/O pins
		// 0 = output, 1 = input
		
        //banksel TRISA
        //movlw   b'00000000'
        TRISA=   0b00000000; 
        //         UU------
        //         --x-----  PIN2 : RA5
        //         ---x----  PIN3 : RA4
        //         ----x---  PIN4 : RA3
        //         -----x--  PIN11: RA2
        //         ------x-  PIN12: RA1
        //         -------x  PIN13: RA0
        //movwf   TRISA
        
// set up direction of PORTC I/O pins
		// 0 = output, 1 = input
		
        //banksel TRISC
        //movlw   b'00000000'
        TRISC=   0b00010000;
        //         UU------
        //         --x----- RC5
        //         ---x---- RC4
        //         ----x--- RC3 
        //         -----x-- RC2 
        //         ------x- RC1 
        //         -------x RC0 
        //movwf   TRISC


		// for setting a pin to digital/analog, see ANSEL


//**********************************************************************
//                  SETUP COMPRATOR  16F684, hiányos                   *
//**********************************************************************

// set up comparator

        // To  use  CIN+  and  CIN-  pins  as  analog
        // inputs,   the   appropriate   bits   must   be
        // programmed in the CMCON (19h) register.

        // Basic mode : GP0 = CIN+,GP1=CIN-
        // With Output: GP2 state = COUT
        // Internal Reference without multiplexing: GP0 as digital pin, GP1 = CIN+
        // Internal Reference with multiplexing: GP0 = CIN+, GP1 = CIN-

        //banksel CMCON0
        //movlw   b'00000111' //Comparators off. CxIN pins are configured as digital I/O
        CMCON0=  0b00000111;
		//         x-------  C2OUT
        //         -x------  C1OUT
        //         --x-----  C2INV
        //         ---x----  C1INV
        //         ----x---  CIS
        //         -----x--  CM2
        //         ------x-  CM1
        //         -------x  CM0
        //movwf   CMCON0
				//CM<2:0>: Comparator Mode bits (See Figure 8-5)
				//000 = Comparators off. CxIN pins are configured as analog
				//001 = Three inputs multiplexed to two comparators
				//010 = Four inputs multiplexed to two comparators
				//011 = Two common reference comparators
				//100 = Two independent comparators
				//101 = One independent comparator
				//110 = Two common reference comparators with outputs
				//111 = Comparators off. CxIN pins are configured as digital I/O

	    //banksel CMCON1
        //movlw   b'00000011'
        CMCON1=  0b00000000; 
		//         UUUUUU--  
        //         ------x-  T1GSS: Timer1 Gate Source Select bit    
		//         ------x-  1 = Timer1 gate source is T1G pin (pin should be configured as digital input)          
		//         ------x-  0 = Timer1 gate source is Comparator C2 output          
        //         -------x  C2SYNC: Comparator C2 Output Synchronization bit    
        //         -------x  1 = Output is synchronized with falling edge of Timer1 clock
        //         -------x  0 = Output is asynchronous
        //movwf   CMCON0


        
		//	 VRCON — VOLTAGE REFERENCE CONTROL REGISTER (ADDRESS: 99h) 
		//		set: internal voltage reference, half of Vdd
        //banksel VRCON
        //movlw   b'10101100'
        VRCON=   0b10101100; 
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
//                  SETUP ADC 16F684                                   *
//**********************************************************************

// set up A/D converter

	// ANS7:ANS0 = 0 : port is digital, 1 : port is analog 16F684
        //banksel ANSEL
        //movlw   b'00000000'
         ANSEL=  0b00000000; 
        //         x-------  ANS7
        //         -x------  ANS6
        //         --x-----  ANS5        
        //         ---x----  ANS4
        //         ----x---  ANS3    
        //         -----x--  ANS2		  
        //         ------x-  ANS1		 
        //         -------x  ANS0		 
        //movwf   ANSEL
        
        //banksel ADCON0
        //movlw   b'10000000'
        ADCON0=  0b10010100;
        //         x-------  ADFM	: 1=right justified:0=left justified result
        //         -x------  VCFG	: 0=Vdd is voltage reference
        //         --x-----  		  : not implemented
        //         ---xxx--  CHS2-0: 000=AN0 see below
        //         ------x-  GO/_DONE: 0=A/D conversion not started
        //         -------x  ADON	: 1=A/D converter module is operating
        //movwf   ADCON0

        //CHS2-0:
        //000 = AN0
        //001 = AN1
        //010 = AN2
        //011 = AN3
        //100 = AN4
        //101 = AN5
        //110 = AN6
        //111 = AN7
        
        //banksel ADCON1
        //movlw   b'01000000'
        ADCON1=  0b01110000;
        //         U------- 
        //         -xxx---- ADCS<2:0>: A/D Conversion Clock Select bits, see below
        //         ----UUUU
        //movwf   ADCON1     

       //ADCS<2:0>: A/D Conversion Clock Select bits
       //000 = FOSC/2
       //001 = FOSC/8
       //010 = FOSC/32
       //x11 = FRC (clock derived from a dedicated internal oscillator = 500 kHz max)
       //100 = FOSC/4
       //101 = FOSC/16
       //110 = FOSC/64
         
//**********************************************************************
//                  SETUP OSCILLATOR 16F684                            *
//**********************************************************************
         
//setup  OSCCON: OSCILLATOR CONTROL REGISTER

        //banksel OSCCON
        //movlw   b'01100110' // internal clock = system clock = 1 MHz
                              // 1 instruction = 0.25 uSec
        OSCCON=  0b01110111;
        //         U-------  
        //         -xxx---- IRCF<2:0>: Internal Oscillator Frequency Select bits
        //         ----x--- OSTS: Oscillator Start-up Time-out Status bit         
        //         ----1---    1 =  Device is running from the external clock defined by FOSC<2:0> of the CONFIG register
        //         ----0---    0 =  Device is running from the internal oscillator (HFINTOSC or LFINTOSC)  
        //         -----x-- HTS: HFINTOSC Status bit (High Frequency – 8 MHz to 125 kHz) 		  
        //         -----1-- 1 =  HFINTOSC is stable 		 
        //         -----0-- 0 =  HFINTOSC is not stable
        //         ------x- LTS: LFINTOSC Stable bit (Low Frequency – 31 kHz)
        //         ------1- 1 =  LFINTOSC is stable
        //         ------0- 0 =  LFINTOSC is not stable
        //         -------x SCS: System Clock Select bit
        //         -------1 1 =  Internal oscillator is used for system clock
        //         -------0 0 =  Clock source defined by FOSC<2:0> of the CONFIG register
        //movwf   OSCCON
        
        //IRCF<2:0>: Internal Oscillator Frequency Select bits
        //111 =8 MHz
        //110 =4 MHz (default)
        //101 =2 MHz
        //100 =1 MHz
        //011 =500 kHz
        //010 =250 kHz
        //001 =125 kHz
        //000 =31 kHz (LFINTOSC)
         
         
//**********************************************************************
//                  SETUP TIMER  16F684                                *
//**********************************************************************
         
//setup timer

    asm("clrwdt"); 					//Clear WDT and prescaler

    //BANKSEL	OPTION_REG 	//
    //movlw   b'00000000'
    OPTION_REG=0b00000000;
    //        x-------  RAPU  : PORTA Pull-up Enable bit
    //        1-------                     1 = PORTA pull-ups are disabled
    //        0-------			   0 = PORTA pull-ups are enabled by individual PORT latch values
    //        -x------  INTEDG	:	Interrupt Edge Select bit
    //        -1------				1 = Interrupt on rising edge of RA2/INT pin
    //        -0------				0 = Interrupt on falling edge of RA2/INT pin
    //        --x-----	T0CS	:	TMR0 Clock Source Select bit
    //        --1-----				1 = Transition on RA2/T0CKI pin
    //        --0-----				0 = Internal instruction cycle clock (FOSC/4)
    //        ---x----	T0SE: Timer0 Source Edge Select bit
    //        ---1----	      1 = Increment on high-to-low transition on RA2/T0CKI pin
    //        ---0----	      0 = Increment on low-to-high transition on RA2/T0CKI pin
    //        ----x---	PSA		: 	Prescaler Assignment bit
    //        ----1---				1 = Prescaler is assigned to the WDT
    //        ----0---				0 = Prescaler is assigned to the TIMER0 module
    //        -----xxx	PS2:PS0	:	Prescaler Rate Select bits
    //						         Bit      TMR0 Rate     WDT Rate
    //         					 000      1 : 2         1 : 1
    //      					 001      1 : 4         1 : 2
    //      					 010      1 : 8         1 : 4
    //      					 011      1 : 16        1 : 8
    //      					 100      1 : 32        1 : 16
    //      					 101      1 : 64        1 : 32
    //      					 110      1 : 128       1 : 64
    //      					 111      1 : 256       1 : 128
    //movwf   OPTION_REG

    //BANKSEL TMR0
    //clrf    TMR0            // reset timer (and prescaler!)
        TMR0=0;

//**********************************************************************
//                  SETUP INTERRUPTS  16F684                           *
//**********************************************************************

//setup interrupts
		//BANKSEL	INTCON 	//
        //movlw   b'00000000'
       INTCON=0b00000000;
    	//  	x-------   GIE: Global Interrupt Enable bit
	//	1-------	     1 = Enables all unmasked interrupts
	//	0-------			 0 = Disables all interrupts
    	//      -x------   PEIE: Peripheral Interrupt Enable bit
	//	-1------	     1 = Enables all unmasked peripheral interrupts
	//	-0------	     0 = Disables all peripheral interrupts
	//	--x-----	 T0IE: TMR0 Overflow Interrupt Enable bit
	//	--1-----	     1 = Enables the TMR0 interrupt
	//	--0-----	     0 = Disables the TMR0 interrupt
	//	---x----	 INTE: RA2/INT External Interrupt Enable bit
	//	---1----	     1 = Enables the RA2/INT external interrupt
	//	---0----	     0 = Disables the RA2/INT external interrupt
	//	----x---	 RAIE: Port Change Interrupt Enable bit	
	//	----x---	 	 	 IOCA register must also be enabled.		
	//	----1---	     1 = Enables the GPIO port change interrupt
	//	----0---	     0 = Disables the GPIO port change interrupt
	//	-----x--   T0IF: TMR0 Overflow Interrupt Flag bit
	//	-----x--   	 	T0IF bit is set when TIMER0 rolls over. TIMER0 is unchanged on RESET and
	//	-----x--   	 	should be initialized before clearing T0IF bit.
	//	-----1--       1 = TMR0 register has overflowed (must be cleared in software)
	//	-----0--       0 = TMR0 register did not overflow
	//	------x-   INTF: RA2/INT External Interrupt Flag bit
	//	------1-       1 = The RA2/INT external interrupt occurred (must be cleared in software)
	//	------0-       0 = The RA2/INT external interrupt did not occur
	//	-------x   RAIF: PORTA Change Interrupt Flag bit
	//	-------1       1 = When at least one of the PORTA <5:0> pins changed state (must be cleared in software)
	//	-------0       0 = None of the PORTA <5:0> pins have changed state
	//movwf   INTCON
	

//setup peripheral interrupts  16F684
	//BANKSEL	 PIE1 	//
        //movlw   b'00000000'
        PIE1= 0b00000000;
    	//	x-------  EEIE: EE Write Complete Interrupt Enable bit
	//	1-------	    1 = Enables the EE write complete interrupt
	//	0-------			  0 = Disables the EE write complete interrupt
    	//     	-x------  ADIE: A/D Converter (ADC) Interrupt Enable bit
	//	-1------	    1 = Enables the ADC interrupt
	//	-0------	    0 = Disables the ADC interrupt
	//	--x-----  CCP1IE: CCP1 Interrupt Enable bit 
	//	--1-----	 1 = Enables the CCP1 interrupt
	//	--0-----	 0 = Disables the CCP1 interrupt
	//	---x----	C2IE: Comparator 2 Interrupt Enable bit 
	//	---1----	1 = Enables the Comparator 2 interrupt 
	//	---0----	0 = Disables the Comparator 2 interrupt 
	//	----x---	C1IE: Comparator 1 Interrupt Enable bit 
	//	----1---	1 = Enables the Comparator 1 interrupt 
	//	----0---	0 = Disables the Comparator 1 interrupt  
	//	-----x--  OSFIE: Oscillator Fail Interrupt Enable bit
	//	-----1--  1 = Enables the oscillator fail interrupt
	//	-----0--  0 = Disables the oscillator fail interrupt 
	//	------x-  TMR2IE: Timer2 to PR2 Match Interrupt Enable bit
	//	------1-  1 = Enables the Timer2 to PR2 match interrupt
	//	------0-  0 = Disables the Timer2 to PR2 match interrupt
	//	-------x  TMR1IE: Timer1 Overflow Interrupt Enable bit
	//	-------1  1 = Enables the Timer1 overflow interrupt  
	//	-------0  0 = Disables the Timer1 overflow interrupt  
	//movwf   PIE1

/*
//        TMR0 interrupt
//        1000 Hz (1 interrupt per 1 msec)
//Timer0 Registers Prescaler= 4 - TMR0 Preset = 6 - Freq = 1000.00 Hz - Period = 0.001000 seconds
T0CS = 0;  // bit 5  TMR0 Clock Source Select bit...0 = Internal Clock (CLKO) 1 = Transition on T0CKI pin
T0SE = 0;  // bit 4 TMR0 Source Edge Select bit 0 = low/high 1 = high/low
PSA = 0;   // bit 3  Prescaler Assignment bit...0 = Prescaler is assigned to the Timer0
PS2 = 0;   // bits 2-0  PS2:PS0: Prescaler Rate Select bits
PS1 = 0;
PS0 = 1;
TMR0 = 6;             // preset for timer register
*/

/*
//100000 Hz
//Timer0 Registers Prescaler= 2 - TMR0 Preset = 251 - Freq = 100000.00 Hz - Period = 0.000010 seconds
T0CS = 0;  // bit 5  TMR0 Clock Source Select bit...0 = Internal Clock (CLKO) 1 = Transition on T0CKI pin
T0SE = 0;  // bit 4 TMR0 Source Edge Select bit 0 = low/high 1 = high/low
PSA = 0;   // bit 3  Prescaler Assignment bit...0 = Prescaler is assigned to the Timer0
PS2 = 0;   // bits 2-0  PS2:PS0: Prescaler Rate Select bits
PS1 = 0;
PS0 = 0;
TMR0 = 251;             // preset for timer register
*/

} // pic_init() 16F684




