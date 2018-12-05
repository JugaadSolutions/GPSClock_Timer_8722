#ifndef __BOARD__
#define __BOARD__

/*
*------------------------------------------------------------------------------
* device.h
*
* Include file for port pin assignments
*

*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* File				: device.h
*------------------------------------------------------------------------------
*

*------------------------------------------------------------------------------
*/

/* 
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include <p18f8722.h>
#include <delays.h>
#include <timers.h>
#include "typedefs.h"
#include "config.h"


/*
*------------------------------------------------------------------------------
* Hardware Port Allocation
*------------------------------------------------------------------------------
*/



// Heart Beat




// Rs485 / RS232 Serial commnunicaton port
#define		TX1_EN					PORTGbits.RG3			// TX control for RS485 communication
#define		TX1_EN_DIR				TRISGbits.TRISG3

#define 	SER1_TX					PORTCbits.RC6 		// serial transmit
#define		SER1_TX_DIR				TRISCbits.TRISC6
#define 	SER1_RX					PORTCbits.RC7			// serial receive
#define		SER1_RX_DIR				TRISCbits.TRISC7


#define 	DIGIT_1					PORTA
#define 	DIGIT_1_DIRECTION		TRISA

#define 	DIGIT_2					PORTB
#define 	DIGIT_2_DIRECTION		TRISB

#define 	DIGIT_3					PORTD
#define 	DIGIT_3_DIRECTION		TRISD

#define 	DIGIT_4					PORTE
#define 	DIGIT_4_DIRECTION		TRISE

#define 	DIGIT_5					PORTH
#define 	DIGIT_5_DIRECTION		TRISH

#define 	DIGIT_6					PORTJ
#define 	DIGIT_6_DIRECTION		TRISJ


#define 	KEYPAD_PORT_0				PORTFbits.RF0
#define 	KEYPAD_PORT_0_DIRECTION 	TRISFbits.RF0

#define 	KEYPAD_PORT_1				PORTFbits.RF1
#define 	KEYPAD_PORT_1_DIRECTION 	TRISFbits.RF1

#define 	KEYPAD_PORT_2				PORTFbits.RF2
#define 	KEYPAD_PORT_2_DIRECTION 	TRISFbits.RF2

#define 	KEYPAD_PORT_3				PORTFbits.RF3
#define 	KEYPAD_PORT_3_DIRECTION 	TRISFbits.RF3

					

// HeartBeat
#define		HEART_BEAT				PORTGbits.RG0			// high - OFF, Low - ON
#define		HEART_BEAT_DIR			TRISGbits.TRISG0


			
/*
*------------------------------------------------------------------------------
* Public Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Macros
*------------------------------------------------------------------------------
*/



#define PERIPHERAL_CLOCK			(SYSTEM_CLOCK / 4)UL

#define PERIPHERAL_CLOCK_PERIOD 	(1 / PERIPHERAL_CLOCK)UL



// Direction controle bit is processor specific ,
#define PORT_OUT				(BOOL)(0)
#define PORT_IN					(BOOL)(0xFF)

#define OFF_FOREVER				(BOOL)(0)
#define LOOP_FOREVER			(BOOL)(1)

#define SWITCH_OFF				(BOOL)(0)
#define SWITCH_ON				(BOOL)(1)

#define DISPLAY_DISABLE			(BOOL)(1)
#define DISPLAY_ENABLE			(BOOL)(0)


#define GetSystemClock()		(SYSTEM_CLOCK)      // Hz
#define GetInstructionClock()	(GetSystemClock()/4)
#define GetPeripheralClock()	GetInstructionClock()

#define ENTER_CRITICAL_SECTION()	INTCONbits.GIE = 0;// Disable global interrupt bit.


#define EXIT_CRITICAL_SECTION()		INTCONbits.GIE = 1;// Enable global interrupt bit.

#define ENABLE_GLOBAL_INT()			EXIT_CRITICAL_SECTION()


#define DISABLE_INT0_INTERRUPT()	INTCONbits.INT0IE = 0
#define ENABLE_INT0_INTERRUPT()		INTCONbits.INT0IE = 1
#define CLEAR_INTO_INTERRUPT()		INTCONbits.INT0IF = 0


#define DISABLE_TMR0_INTERRUPT()	INTCONbits.TMR0IE = 0
#define ENABLE_TMR0_INTERRUPT()		INTCONbits.TMR0IE = 1

#define DISABLE_TMR1_INTERRUPT()	PIE1bits.TMR1IE = 0
#define ENABLE_TMR1_INTERRUPT()		PIE1bits.TMR1IE = 1



#define DISABLE_UART2_TX_INTERRUPT()	PIE3bits.TX2IE = 0
#define ENABLE_UART2_TX_INTERRUPT()		PIE3bits.TX2IE = 1

#define DISABLE_UART2_RX_INTERRUPT()	PIE3bits.RC2IE = 0
#define ENABLE_UART2_RX_INTERRUPT()		PIE3bits.RC2IE = 1

#define DISABLE_UART1_TX_INTERRUPT()	PIE1bits.TX1IE = 0
#define ENABLE_UART1_TX_INTERRUPT()		PIE1bits.TX1IE = 1

#define DISABLE_UART1_RX_INTERRUPT()		PIE1bits.RC1IE = 0
#define ENABLE_UART1_RX_INTERRUPT()		PIE1bits.RC1IE = 1

#define ENB_485_TX()	TX_EN = 1;
#define ENB_485_RX()	TX_EN = 0

#define Delay10us(us)		Delay10TCYx(((GetInstructionClock()/1000000)*(us)))
#define DelayMs(ms)												\
	do																\
	{																\
		unsigned int _iTemp = (ms); 								\
		while(_iTemp--)												\
			Delay1KTCYx((GetInstructionClock()+999999)/1000000);	\
	} while(0)

/*
*------------------------------------------------------------------------------
* Public Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables (extern)
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* Public Constants (extern)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Function Prototypes (extern)
*------------------------------------------------------------------------------
*/

extern void BRD_init(unsigned long systemFreauency );

#endif
/*
*  End of device.h
*/