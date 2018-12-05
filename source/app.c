
#include "app.h"
#include "eep.h"
#include "linearkeypad.h"
#include "timer.h"
#include <string.h>
#define EEPROM_HH 0
#define EEPROM_MM 1
#define EEPROM_SS 2

typedef enum
{
	CLOCK = 1,
	TIMER = 2
} APP_MODE;

typedef enum
{
	RESET = 1,
	PAUSED = 2,
	COUNT = 3,
	SET

} TIMER_STATE;

typedef struct 
{
	APP_MODE Mode;

	UINT8 HH;
	UINT8 MM;
	UINT8 SS;

	UINT8 TMR_HH;
	UINT8 TMR_MM;
	UINT8 TMR_SS;

	

	TIMER_STATE State;
	
	UINT8 DigitBuf[10];
	UINT8 PrevDigitBuf[10];
	
	UINT8 CurSetDigit;
	

} APP;

APP App;


	


const UINT8 LED_MAP[11] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00};
char GPSDATA[12] =  {0};


BOOL GPS_UPDATED = FALSE;





void ConvertUTC2IST(void);
void TIMERStateHandler(void);
void UpdateTimerCount(void);
void UpdateDigits(void);
void UpdateTimerValue(void);

void APP_Init()
{

	int i,j;

	App.Mode = CLOCK;
	App.State = RESET;
	

	App.TMR_HH = Read_b_eep(EEPROM_HH );
	Busy_eep();

	App.TMR_MM = Read_b_eep(EEPROM_MM );
	Busy_eep();

	App.TMR_SS = Read_b_eep(EEPROM_SS );
	Busy_eep();

	for(i = 0; i < 11 ; i++)
	{
		DIGIT_1 = DIGIT_2 = DIGIT_3 = DIGIT_4 = DIGIT_5 = DIGIT_6 = LED_MAP[i];

		DelayMs(500);
	}
	for(i = 0; i < 6 ; i++)
	{
		App.PrevDigitBuf[i] = App.DigitBuf[i] = 0;
	}

}


void APP_Task()
{
	if(LinearKeyPad_getKeyState( KEY0 ) == KEY_PRESSED) //TIMER MODE
	{
		if(App.Mode == CLOCK ) 
		{
			DISABLE_UART1_RX_INTERRUPT();
			App.HH = App.TMR_HH;
			App.MM = App.TMR_MM;
			App.SS = App.TMR_SS;

			App.State = RESET;
			App.Mode = TIMER;
		}

		TIMERStateHandler();
	}

	else		//GPS CLOCK MODE
	{
		if(App.Mode == TIMER)
		{
			App.Mode = CLOCK;
			ENABLE_UART1_RX_INTERRUPT();
		}

		if(GPS_UPDATED == FALSE ) return;
	
		ConvertUTC2IST();
		
		GPS_UPDATED = FALSE;
	
	}
	UpdateDigits();

}

void ConvertUTC2IST()
{
		

		App.HH = (GPSDATA[0] - '0')*10+(GPSDATA[1] - '0');

		App.MM = (GPSDATA[2]- '0')*10+(GPSDATA[3]- '0');
		App.MM +=30;
		
		App.SS = (GPSDATA[4]- '0')*10+(GPSDATA[5]- '0');
		
			
		if(App.MM > 60 )
		{
			App.HH++;
			App.MM-=60;
		}

		App.HH+=5;

		if(App.HH >12)
		{
			App.HH-=12;
		}

	

}

UINT8 APP_Callback(char* rxdata, UINT8* txCode,UINT8** txPacket)
{
	char *gpsdata;
	char delimiter[] = ",";
	if( strlen(rxdata) < 27 )
		return 0;
	
	gpsdata = strtok(rxdata , delimiter );
	
	gpsdata = strtok(NULL , delimiter);
	strcpy ( GPSDATA, gpsdata);

	GPS_UPDATED = TRUE;
	return 0;
}

void TIMERStateHandler()
{
	UINT8 i;
	switch(App.State)
	{
		case RESET:
		if(LinearKeyPad_getPBState( KEY1 ) == PB_PRESSED) //COUNT
		{
			TMR_ResetAppUpdateCount();
			App.State = COUNT;
		}
		else if(LinearKeyPad_getPBState( KEY3 ) == PB_PRESSED) //SET
		{
			TMR_ResetAppUpdateCount();
			App.CurSetDigit = 0;

			for(i = 0; i < 6 ; i++)
				App.DigitBuf[i] = 0;

			App.State = SET;
		}
		break;

		case COUNT:
		if(LinearKeyPad_getPBState( KEY2 ) == PB_PRESSED) //RESET
		{
			App.HH = App.TMR_HH;
			App.MM = App.TMR_MM;
			App.SS = App.TMR_SS;

			App.State = RESET;
			return;

		}
		else if(LinearKeyPad_getPBState( KEY1 ) == PB_PRESSED) //PAUSED
		{
			
			App.State = PAUSED;
			return;
		}
		
		if( (App.HH == 0) && (App.MM==0)&& (App.SS == 0 )) 
		{
			return;
		}
		if(TMR_GetAppUpdateCount() >= 1000)
		{
			TMR_ResetAppUpdateCount();
			UpdateTimerCount();
		}

		break;

		case PAUSED:
		if(LinearKeyPad_getPBState( KEY2 ) == PB_PRESSED) //RESET
		{
			App.HH = App.TMR_HH;
			App.MM = App.TMR_MM;
			App.SS = App.TMR_SS;

			App.State = RESET;
			return;
		}
		else if(LinearKeyPad_getPBState( KEY1 ) == PB_PRESSED) //COUNT
		{
			TMR_ResetAppUpdateCount();
			App.State = COUNT;
		}

		break;

		case SET:
		if(TMR_GetAppUpdateCount() >= 500)
		{
			switch(App.CurSetDigit)
			{
				case 0: 
				DIGIT_1 = LED_MAP[10];
				break;
				case 1: 
				DIGIT_2 = LED_MAP[10];
				break;
				case 2: 
				DIGIT_3 = LED_MAP[10];
				break;
				case 3: 
				DIGIT_4 = LED_MAP[10];
				break;
				case 4: 
				DIGIT_5 = LED_MAP[10];
				break;
				case 5: 
				DIGIT_6 = LED_MAP[10];
				break;
			}
			
		}
	
		if(TMR_GetAppUpdateCount() >= 1000)
		{
			UpdateDigits();
			TMR_ResetAppUpdateCount();
		}
			
		if(LinearKeyPad_getPBState( KEY2 ) == PB_PRESSED) //RESET
		{
			App.CurSetDigit++;
			if(App.CurSetDigit >= 6 )
				App.CurSetDigit = 0;
			return;
			
		}
		if(LinearKeyPad_getPBState( KEY1 ) == PB_PRESSED) //RESET
		{
			App.DigitBuf[App.CurSetDigit]++ ;
			switch(App.CurSetDigit)
			{
				case 0: 
				if(App.DigitBuf[App.CurSetDigit] >= 1 )
					App.DigitBuf[App.CurSetDigit] = 0;
				break;
				case 1: 
				if(App.DigitBuf[App.CurSetDigit] >= 2 )
					App.DigitBuf[App.CurSetDigit] = 0;
				break;
				case 2: 
				if(App.DigitBuf[App.CurSetDigit] >= 5 )
					App.DigitBuf[App.CurSetDigit] = 0;
				break;
				case 3: 
				if(App.DigitBuf[App.CurSetDigit] >= 9 )
					App.DigitBuf[App.CurSetDigit] = 0;
				break;
				case 4: 
				if(App.DigitBuf[App.CurSetDigit] >= 5 )
					App.DigitBuf[App.CurSetDigit] = 0;
				break;
				case 5: 
				if(App.DigitBuf[App.CurSetDigit] >= 9 )
					App.DigitBuf[App.CurSetDigit] = 0;
				break;
			}
			return;
			
		}
		
		if(LinearKeyPad_getPBState( KEY3 ) == PB_PRESSED) //RESET
		{
			UpdateTimerValue();
			App.HH = App.TMR_HH;
			App.MM = App.TMR_MM;
			App.SS = App.TMR_SS;
			App.State = RESET;
			UpdateDigits();
			return;
			
		}

		break;

		default:
		break;
	}
}

void UpdateTimerCount()
{
	
	if(App.SS > 0 )
	{
		App.SS--;
	}
	else if(App.SS == 0 )
	{
		App.SS = 59;
		if(App.MM >0 )	
		{
			App.MM--;
		}
		else if(App.MM == 0)
		{
			App.MM = 59;
			if(App.HH > 0 )
			{
				App.HH--;
			}
			
		}
	}

}


void UpdateDigits()
{

	if(App.State == SET )
	{
		DIGIT_1 = LED_MAP[App.DigitBuf[0] ];
		DIGIT_2 = LED_MAP[App.DigitBuf[0] ];
		DIGIT_3 = LED_MAP[App.DigitBuf[0] ];
		DIGIT_4 = LED_MAP[App.DigitBuf[0] ];
		DIGIT_5 = LED_MAP[App.DigitBuf[0] ];
		DIGIT_6 = LED_MAP[App.DigitBuf[0] ];
	}
	
	App.DigitBuf[0] = App.HH/10 ;
	App.DigitBuf[1] = App.HH%10;
	App.DigitBuf[2] = App.MM/10 ;
	App.DigitBuf[3] = App.MM%10;
	App.DigitBuf[4] = App.SS/10 ;
	App.DigitBuf[5] = App.SS%10;

		
	if(App.PrevDigitBuf[0] != App.DigitBuf[0] )
	{
		DIGIT_1 = LED_MAP[App.DigitBuf[0] ];
		App.PrevDigitBuf[0] = App.DigitBuf[0];
	}

	if(App.PrevDigitBuf[1] != App.DigitBuf[1] )
	{
		DIGIT_2 = LED_MAP[App.DigitBuf[1] ];
		App.PrevDigitBuf[1] = App.DigitBuf[1];
	}

	if(App.PrevDigitBuf[2] != App.DigitBuf[2] )
	{
		DIGIT_3 = LED_MAP[App.DigitBuf[2] ];
		App.PrevDigitBuf[2] = App.DigitBuf[2];
	}

	if(App.PrevDigitBuf[3] != App.DigitBuf[3] )
	{
		DIGIT_4 = LED_MAP[App.DigitBuf[3] ];
		App.PrevDigitBuf[3] = App.DigitBuf[3];
	}

	if(App.PrevDigitBuf[4] != App.DigitBuf[4] )
	{
		DIGIT_5 = LED_MAP[App.DigitBuf[4] ];
		App.PrevDigitBuf[4] = App.DigitBuf[4];
	}

	if(App.PrevDigitBuf[5] != App.DigitBuf[5] )
	{
		DIGIT_6 = LED_MAP[App.DigitBuf[5] ];
		App.PrevDigitBuf[5] = App.DigitBuf[5];
	}
	
}	
	

void UpdateTimerValue()
{
	
	App.TMR_HH = (App.DigitBuf[0] - '0')*10+(App.DigitBuf[1] - '0');
	Write_b_eep(EEPROM_HH,App.TMR_HH);
	Busy_eep();
	ClrWdt();

	App.TMR_MM = (App.DigitBuf[2]- '0')*10+(App.DigitBuf[3]- '0');
	Write_b_eep(EEPROM_HH,App.TMR_HH);
	Busy_eep();
	ClrWdt();

	App.TMR_SS = (App.DigitBuf[4]- '0')*10+(App.DigitBuf[5]- '0');
	Write_b_eep(EEPROM_HH,App.TMR_HH);
	Busy_eep();
	ClrWdt();

}
