#include "main.h"

#define	SC7313ADDR	0x88

UCHAR EQ_Item = 0;
signed char CurrentVol = DEFVOL;
signed char SetBASS = 7;
signed char SetTREB = 7;
signed char SetBAL = 7;
//signed char SetFAD = 9;

bit isMute;
bit MuteState;
UCHAR VolDispTimer;
bit  ChangeVolFlag;



UCHAR CODE BASS_TABLE[15]={0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0xE,0xD,0xC,0xB,0xA,0x9,0x8};
UCHAR CODE Attenuators[19] = {0x1F,0x1C,0x18,0x14,0x10,0x0C,0x08,0x04,0x0,0x0,
					  0x04,0x08,0x0C,0x10,0x14,0x14,0x18,0x1C,0x1F};



void SC7313_Driver(UCHAR temp)
{
	I2C1_write(SC7313ADDR,&temp,1);
}


/*******************************************************************
					 sc7313C音效控制函数			   
函数原型: void SC7313_Driver(BYTE TempSign, BYTE temp)

功能:	  设置SC7313芯片的控制  
********************************************************************/

void SC7313_EQ(
	BYTE TempSign,	
	BYTE temp
	)
{
	
	BYTE SetVaule;
	switch(TempSign) {
	
		case VOL_EQ:
			SetVaule = temp;	
			break;
	
		case BAS_EQ:
			SetVaule = 0x60 | BASS_TABLE[temp];
			break;
	
		case TRE_EQ:
			SetVaule = 0x70 | BASS_TABLE[temp];
			break;
			
		case BAL_EQ:
			if (temp <= 7)			
				SetVaule = (0xA0 | Attenuators[temp]);		
			else 
					SetVaule = (0x80 | Attenuators[temp]);
			break;
			
	//	case FAD_EQ:
	//		break;
	
		default:		
			break;
			
	}	
	
	SC7313_Driver(SetVaule);
	
	System.DispMode = DISPLAY_AUDIO;
	ChangeVolFlag = TRUE;
	VolDispTimer = 10;
}


/*******************************************************************
                     sc7313C初始化函数               
函数原型: void  SC7313_initial(void);  
功能:     设置SC7313芯片的初始化值（包括音量、通道选择、通道的输出衰减控制、高低音）.  
********************************************************************/

void SC7313_initial(UCHAR channel)
{	

	WaitUs(0xff);
	SC7313_Driver(channel);		//Select channel 1 ,LOUD on
	WaitUs(0xff);
	SC7313_Driver(MINVOL - CurrentVol*2);
	WaitUs(0xff);
	
	SC7313_Driver(0x80);		//Speaker LF
	WaitUs(0xff);
	SC7313_Driver(0xa0);		//Speaker RF
	WaitUs(0xff);

	if (SetBAL <= 7)			
		SC7313_Driver(0xA0 | Attenuators[SetBAL]);		
	else 
			SC7313_Driver(0x80 | Attenuators[SetBAL]);

/* 
	
	WaitUs(0xff);
	if (SetBAL <= 7)			
		SC7313_Driver(0xA0 | Attenuators[SetBAL]);		
	else 
			SC7313_Driver(0x80 | Attenuators[SetBAL]);
	WaitUs(0xff);
	SC7313_speaker(0xc0);
	WaitUs(0xff);
	SC7313_speaker(0xe0);
*/
	WaitUs(0xff);
	SC7313_Driver( 0x60 | BASS_TABLE[SetBASS]);			//Bass
	WaitUs(0xff);
	SC7313_Driver(0x70 | BASS_TABLE[SetTREB]);			//Treble
}

void AudioMain(void)
{
	UCHAR eq_temp;


	if (isMute != MuteState) {	
		if(isMute == TRUE) {
			MUTE_AMP(); 
		}
		else { 
			UnMUTE_AMP();
		}
		MuteState = isMute;
	}
	
	if (gKeyEvent != IN_KEY_NONE) {
		switch(gKeyEvent) { 				
			
		case IN_KEY_SEL_SP:
			ChangeVolFlag = TRUE;
			VolDispTimer = 10;
			System.DispMode = DISPLAY_AUDIO;
			if (EQ_Item == BAL_EQ)
				EQ_Item = VOL_EQ;
			else
				EQ_Item += 1;		
			break;
					
		case IN_KEY_VOL_ADD:			
			switch(EQ_Item) {	
			case VOL_EQ:
				if(++CurrentVol > VOL_MAX) { 
					CurrentVol = VOL_MAX;	
					eq_temp = MAXVOL;
				}
				else 
					eq_temp = MINVOL - CurrentVol*2;
				break;

		case BAS_EQ:					
			if(++SetBASS > BASS_MAX)
				SetBASS= BASS_MAX;					
			eq_temp = SetBASS;
			break;

		case TRE_EQ:
			if(++SetTREB > TREB_MAX)
				SetTREB = TREB_MAX;	
				eq_temp = SetTREB;
			break;
		case BAL_EQ:					
			if(++SetBAL > BAL_MAX)
				SetBAL = BAL_MAX;	
				eq_temp = SetBAL;
			break;
				
		//	case FAD_EQ:
		//		break;					
		}
		SC7313_EQ(EQ_Item,eq_temp);			
//		ChangeVolFlag = TRUE;	
//		VolDispTimer = 10;			//display volum 5s 
		if (isMute == TRUE) {
			isMute = FALSE;	
//			UnMUTE_AMP();
		}
		break;
			
		case IN_KEY_VOL_SUB:			
			switch(EQ_Item) {			
			case VOL_EQ:
				if(CurrentVol <= 1) {
					CurrentVol = 0;	
					eq_temp = MINVOL;
				}
				else {
					CurrentVol--;
					eq_temp = MINVOL - CurrentVol*2;
				}
				break;
				
		case BAS_EQ:					
			if(--SetBASS < 1)
				SetBASS = 0;					
			eq_temp = SetBASS;
			break;
	
		case TRE_EQ:
			if(--SetTREB < 1 )
				SetTREB = 0;				
			eq_temp = SetTREB;
			break;
			
		case BAL_EQ:					
			if(--SetBAL < 1)
				SetBAL = 0; 				
			eq_temp = SetBAL;
			break;
				
		//	case FAD_EQ:
		//		break;
		}			
		SC7313_EQ(EQ_Item,eq_temp);			
//		ChangeVolFlag = TRUE;
//		VolDispTimer = 10;
		if (isMute == TRUE) {
			isMute = FALSE;	
//			UnMUTE_AMP();
		}
			break;
		
		default:
			if(VolDispTimer) {
				EQ_Item = VOL_EQ;
				VolDispTimer = 0;
			}
			break;	
	}
	}

	if(System.DispMode == DISPLAY_AUDIO) {
		if(VolDispTimer == 0 && ChangeVolFlag == TRUE) {
			EQ_Item = VOL_EQ;
			ChangeVolFlag = FALSE;
			if(System.WorkMode.Current == WORKMODE_RADIO)
				System.DispMode = DISPLAY_RADIO;
			else 
				System.DispMode = DISPLAY_AUX;	
		}
	}
}
