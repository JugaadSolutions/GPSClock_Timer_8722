
/*
*------------------------------------------------------------------------------
* device.c
*
* Board specipic drivers module(BSP)
*
* (C)2009 Sam's Logic.
*
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* File				: device.c
* Created by		: Sam
* Last changed by	: Sam
* Last changed		: 11/12/2010 
*------------------------------------------------------------------------------
* Revision 1.3 11/12/2010  Sam
* Demo Release
* Revision 1.2 15/11/2010  Sam
* Updated according to the current project requirement
* Revision 1.1 07/07/2010 Sam
* First Release
* Revision 1.0 14/07/2009 Sam
* Initial revision
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/


#include "board.h"
#include "config.h"
#include "typedefs.h"




/*
*------------------------------------------------------------------------------
* Private Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Macros
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables
*------------------------------------------------------------------------------
*/
UINT16 tickPeriod = 0;
/*
*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/

static BOOL ledState;

/*
*------------------------------------------------------------------------------
* Public Constants
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Constants (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Function Prototypes (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* void InitializeBoard(void)

* Summary	: This function configures all i/o pin directions
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void BRD_init(unsigned long systemFreauency )
{



	unsigned long temp;


	switch(systemFreauency)
	{
		case MHz_32:
		default:
				OSCCON = 0X70; 		//internal oscillator 8MHz
				OSCTUNEbits.PLLEN = 1;	//PLL Enable
		break;

		case MHz_16:
				OSCCON = 0X60; 		//internal oscillator 4MHz
				OSCTUNEbits.PLLEN = 1;	//PLL Enable
		break;
		
		case MHz_8:
				OSCCON = 0X50; 		//internal oscillator 2MHz
				OSCTUNEbits.PLLEN = 1;	//PLL Enable
		break;
	}
	DelayMs(5);

	//calculating tick period for timer-0
	temp = systemFreauency >> 2;	
	temp *= (TIMER0_TIMEOUT_DURATION/1000);
	temp /= 1000;		

	tickPeriod = (FULLSCALE_16BIT - (UINT16)temp);


	// set all anolog channels as Digital I/O
	ADCON0 = 0x00;
	ADCON1 = 0x0F;
	ADCON2 = 0xB5;

	MEMCON = 0x80;

	// Configure heart beat LED output
	HEART_BEAT_DIR 	= PORT_OUT;
	HEART_BEAT 		= SWITCH_OFF;

	// Rs485 Direction Controle
	TX1_EN_DIR 		= PORT_OUT;
	TX1_EN 			= SWITCH_OFF;

	// Configure Serial port
	SER1_TX_DIR 		=  PORT_OUT;
	SER1_RX_DIR 		=  PORT_IN;

	DIGIT_1_DIRECTION = PORT_OUT;
	DIGIT_2_DIRECTION = PORT_OUT;
	DIGIT_3_DIRECTION = PORT_OUT;
	DIGIT_4_DIRECTION = PORT_OUT;
	DIGIT_5_DIRECTION = PORT_OUT;
	DIGIT_6_DIRECTION = PORT_OUT;


	KEYPAD_PORT_0_DIRECTION = PORT_IN;
	KEYPAD_PORT_1_DIRECTION = PORT_IN;
	KEYPAD_PORT_2_DIRECTION = PORT_IN;
	KEYPAD_PORT_3_DIRECTION = PORT_IN;


	DelayMs(100);

}

	



/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*  End of device.c
*/
