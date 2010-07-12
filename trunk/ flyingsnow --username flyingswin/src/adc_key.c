

#include "main.h"


/*
**********************************************************
*					LOCAL MACRO
**********************************************************
*/
#define 	ADC_KEY_SCAN_TIME		4
#define 	ADC_KEY_JTTER_TIME		8
#define 	ADC_KEY_CP_TIME			200
#define 	ADC_KEY_HOLD_TIME		20

//efine 	ADC_KEY_RANGE			30
#define		ADC_KEY_COUNT			8
//efine		ADC_KEY_FULL_VAL		1024



/*
**********************************************************
*					LOCAL VARIABLE
**********************************************************
*/
//UCHAR   DATA		PreKeyIndex = 0;
UCHAR	DATA		AdcKeyWaitTimer;
UCHAR	DATA		AdcKeyScanTimer;
ADC_KEY_STATE DATA  AdcKeyState;



/*
**********************************************************
*					LOCAL TABLE
**********************************************************
*/
BYTE CODE AdcKeyEvent[][4] = 
{
	{IN_KEY_NONE,			IN_KEY_NONE,		IN_KEY_NONE,	IN_KEY_NONE 		},	//NO KEY
	
	{IN_KEY_AST_SP, 		IN_KEY_AST_CP,		IN_KEY_NONE,	IN_KEY_NONE 		},	
	{IN_KEY_VOL_SUB,		IN_KEY_NONE,		IN_KEY_NONE,	IN_KEY_NONE 		},	
	{IN_KEY_CLK_SP, 		IN_KEY_CLK_CP,		IN_KEY_NONE,	IN_KEY_NONE 		},	
	{IN_KEY_NEXT_SP,		IN_KEY_NEXT_CP, 	IN_KEY_NEXT_CPR,IN_KEY_NEXT_CPC 	},	
	{IN_KEY_PRE_SP, 		IN_KEY_PRE_CP,		IN_KEY_PRE_CPR, IN_KEY_PRE_CPC		},		
	{IN_KEY_BAND_SP,		IN_KEY_NONE,		IN_KEY_NONE,	IN_KEY_NONE 		},	
	{IN_KEY_MOD_SP, 		IN_KEY_NONE,		IN_KEY_NONE,	IN_KEY_NONE 		},	
	{IN_KEY_PWR_SP, 		IN_KEY_PWR_CP,		IN_KEY_NONE,	IN_KEY_NONE 		},	
	{IN_KEY_VOL_ADD,		IN_KEY_NONE,		IN_KEY_NONE,	IN_KEY_NONE 		},	
	{IN_KEY_SEL_SP, 		IN_KEY_SEL_CP,		IN_KEY_NONE,	IN_KEY_NONE 		},			
	{IN_KEY_P6_SP,			IN_KEY_P6_CP,		IN_KEY_NONE,	IN_KEY_NONE 		},	
	{IN_KEY_P5_SP,			IN_KEY_P5_CP,		IN_KEY_NONE,	IN_KEY_NONE 		},	
	{IN_KEY_P4_SP,			IN_KEY_P4_CP,		IN_KEY_NONE,	IN_KEY_NONE 		},	
	{IN_KEY_P3_SP,			IN_KEY_P3_CP,		IN_KEY_NONE,	IN_KEY_NONE 		},	
	{IN_KEY_P2_SP,			IN_KEY_P2_CP,		IN_KEY_NONE,	IN_KEY_NONE 		},	
	{IN_KEY_P1_SP,			IN_KEY_P1_CP,		IN_KEY_NONE,	IN_KEY_NONE 		}
};	

//WORD  CODE	KeyValueTable[9] = {780,530,465,375,280,200,140,50,0};
WORD  CODE	KeyValueTable[9] = {800,525,460,375,285,205,125,40,0};

/*
**********************************************************
*					GLOBAL FUNCTION
**********************************************************
*/
VOID InitAdc(VOID)
{
	ADCH = 0x06;		//enable channle 1,2
	ADCON |= 0x80;		//enable adc
	ADT = 0x27; 		//Tad = 4* Tsys 
}

//
// Initialize hardware key scan (GPIO) operation.
//
VOID
AdcKeyScanInit(VOID)
{
	AdcKeyState = ADC_KEY_STATE_IDLE;
	AdcKeyScanTimer = 0;
}



/*
**********************************************************
*					LOCAL FUNCTIONS
**********************************************************
*/
//
// Key process, image key value to key event.
//
static BYTE		
AdcChannelKeyGet(
	BYTE Channel
	)							
{
	UINT    Val;
	BYTE	KeyIndex,i;

#if 1
//		UINT  temph;
		UCHAR templ = 15;
		UCHAR wait_conuter;
		
//		ADCON |= 0x80;		//enable adc
	
		ADCON &= 0xF1;					//clear channel 	
		ADCON |= Channel << 1;			//set channel 
		ADCON |= 0x01;					//start AD convert. 

		EA = 0;
		
		while(templ--)
			;
		/* Waiting for AD convert finished	  */
		while((ADCON & 0x01)&& wait_conuter < 0x80) //loop_counter limit should guarantee at least 10us
			wait_conuter++; 
	
		
		Val = ADDH;		//Read MSB
		templ = ADDL;	//Read LSB
		EA = 1;

		Val = Val << 2;
	
		Val = (UINT)Val|templ;
#else 
	Val = AdcReadChannel(Channel);	
#endif
	if (Val >= 700)	{
		KeyIndex = IN_KEY_NONE;
		return KeyIndex;
	}

	for(i = 0; i <= 8; i++) {
		if(Val >= KeyValueTable[i])
			break;
	}
	if(i > 8)
			KeyIndex = IN_KEY_NONE;
		else
			KeyIndex = i;

//	if(Val > 30)
//	{
//		DBG(("Channel:%2d, ", (WORD)Channel));
//		DBG(("Val:%4d, ", (WORD)Val));
//		DBG(("KeyIndex:%4d\n", (WORD)KeyIndex));
//		DBG(("*************************\n"));
//	}
	DispBuff[4] = Num[KeyIndex];
	return KeyIndex;
}

//
// Key process, image key value to key event.
//
KEY_EVENT
AdcKeyEventGet(VOID)				
{
	static 	 UINT DATA PreKeyIndex = 0;
	KEY_EVENT		event = IN_KEY_NONE;
	UCHAR	DATA		CurKeyIndex;

//	DBG(("AdcKeyEventGet*******\n"));
	if (AdcKeyScanTimer > 0)
	{
		return IN_KEY_NONE;
	}
	AdcKeyScanTimer = ADC_KEY_SCAN_TIME;
	
//	AdcOpen(12);
	CurKeyIndex = AdcChannelKeyGet(1);
#if 1
	if(CurKeyIndex == 0)
	{
		CurKeyIndex = AdcChannelKeyGet(2);
		if(CurKeyIndex > 0)
		{
			CurKeyIndex += ADC_KEY_COUNT;
		}
	}
#endif
//	AdcClose();
	
//	DBG(("PreKeyIndex:%4d, ", (WORD)PreKeyIndex));
//	DBG(("KeyIndex:%4d\n", (WORD)KeyIndex));
	switch(AdcKeyState)
	{
		case ADC_KEY_STATE_IDLE:
			if(CurKeyIndex == 0)
			{
				return IN_KEY_NONE;
			}

			PreKeyIndex = CurKeyIndex;
			AdcKeyWaitTimer = ADC_KEY_JTTER_TIME;
//			DBG(("GOTO JITTER!\n"));
			AdcKeyState = ADC_KEY_STATE_JITTER;
				
		case ADC_KEY_STATE_JITTER:
			if(PreKeyIndex != CurKeyIndex)
			{
//				DBG(("GOTO IDLE Because jitter!\n"));
				AdcKeyState = ADC_KEY_STATE_IDLE;
			}
			else if(AdcKeyWaitTimer == 0)
			{
//				DBG(("GOTO PRESS_DOWN!\n"));
				AdcKeyWaitTimer = ADC_KEY_CP_TIME;
				AdcKeyState = ADC_KEY_STATE_PRESS_DOWN;
			}
			break;

		case ADC_KEY_STATE_PRESS_DOWN:
			if(PreKeyIndex != CurKeyIndex)
			{
				//return key sp value
//				DBG(("ADC KEY SP!**********\n"));
				AdcKeyState = ADC_KEY_STATE_IDLE;
				return AdcKeyEvent[PreKeyIndex][0];
			}
			else if(AdcKeyWaitTimer == 0)
			{
				
				AdcKeyWaitTimer = ADC_KEY_HOLD_TIME; //Added by hwt ,for generate Continuous Press event Dec 07,2009 
				//return key cp value
//				DBG(("ADC KEY CP!*****************************\n"));
				AdcKeyState = ADC_KEY_STATE_CP;
				return AdcKeyEvent[PreKeyIndex][1];
			}
			break;

		case ADC_KEY_STATE_CP:
			if(PreKeyIndex != CurKeyIndex)
			{
				//return key cp value
//				DBG(("ADC KEY CPR!****************************************************\n"));
				AdcKeyState = ADC_KEY_STATE_IDLE;
				return AdcKeyEvent[PreKeyIndex][2];
			}
			else {
				if (AdcKeyWaitTimer == 0) {		
					AdcKeyWaitTimer = ADC_KEY_HOLD_TIME; //Added by hwt ,for generate Continuous Press event Dec 07,2009 
					return AdcKeyEvent[PreKeyIndex][3];
				}
			}
			break;
			
		default:
			AdcKeyState = ADC_KEY_STATE_IDLE;
			break;
	}
	return IN_KEY_NONE;
}
