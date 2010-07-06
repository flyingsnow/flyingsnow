#include "main.h"

#define	SC7313ADDR	0x88

signed char CurrentVol = DEFVOL;
signed char SetBASS = 7;
signed char SetTREB = 7;
signed char SetBAL = 7;
//signed char SetFAD = 9;

UCHAR DispVol;
BOOL  ChangeVolFlag;


UCHAR EQ_Item = 0;

UCHAR CODE EQ_Buff[25] = { 0x8A,0xfA,0x24,0x60,0x00,	//VOL
				 0x00,0xd6,0xd1,0x6D,0xd0,  //BAS
				 0x00,0x9E,0x59,0x60,0x50,  //TRE
				 0x00,0x8A,0xD1,0x6D,0xd0,	//BAL
				 0x00,0xeC,0xd1,0x64,0x16   //FAD
				};
UCHAR CODE VOLUM[7] = {0x80,0xc4,0xc6,0xe6,0xe7,0xf7,0xf7};

UCHAR CODE BASS_TABLE[15]={0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0xE,0xD,0xC,0xB,0xA,0x9,0x8};
UCHAR CODE Attenuators[19] = {0x1F,0x1C,0x18,0x14,0x10,0x0C,0x08,0x04,0x0,0x0,
					  0x04,0x08,0x0C,0x10,0x14,0x14,0x18,0x1C,0x1F};



void SC7313_Driver(UCHAR temp)
{
	I2C1_write(SC7313ADDR,&temp,1);
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



void EQ_Display(void)
{
	
	UCHAR i,j;
	
	i = 10;
	j = EQ_Item * 5;

	while(i-- ) DispBuff[i+1]=0x00;
	DispBuff[0] = 0x00;
	DispBuff[11] &= 0xF7;
	
	switch (EQ_Item) {
			case VOL_EQ:

			DispBuff[11] = VOLUM[CurrentVol/5];
			
				DispBuff[9] = 0x00;		
				DispBuff[8] = Num[CurrentVol%10]; 
				(CurrentVol < 10 )? (DispBuff[7]=0x00):(DispBuff[7]= Num[CurrentVol/10]);		
				
				for(i = 0; i < 5; i++)
					DispBuff[5-i] = EQ_Buff[j+i];
			
				break;
		
			case BAS_EQ:
				if (SetBASS >= 7 ) {
					DispBuff[8] = Num[SetBASS - 7];
					DispBuff[7] = 0x00;
				}
				else {					
					DispBuff[8] = Num[7 - SetBASS];
					DispBuff[7] = 0x04;
				}
				for(i = 0; i < 5; i++)
					DispBuff[5-i] = EQ_Buff[j+i];
				break;
		
			case TRE_EQ:
				if (SetTREB >= 7 ) {
					DispBuff[8] = Num[SetTREB - 7];
					DispBuff[7] = 0x00;
				}
				else {
					
					DispBuff[8] = Num[7 - SetTREB];
					DispBuff[7] = 0x04;
				}
				for(i = 0; i < 5; i++)
					DispBuff[5-i] = EQ_Buff[j+i];
			
				break;
				
			case BAL_EQ:
				
				if (SetBAL >= 9 ) {				
					DispBuff[9] = Num[SetBAL - 9];
					DispBuff[8] = 0x04;
					DispBuff[7] = 0x04;	
					if (SetBAL == 9) {
						DispBuff[9] = 0x04;
						DispBuff[8] = Num[SetBAL - 9];
					}					
				}
				else {					
					DispBuff[9] = 0x04;
					DispBuff[8] = 0x04;
					DispBuff[7] = Num[9 - SetBAL];
				}
				
				for(i = 0; i < 5; i++)
					DispBuff[5-i] = EQ_Buff[j+i];
			
				break;
		//	case FAD_EQ:
		//		break;
			
			default:
				break;
				
			break;
		

	}
	

}

void AudioMain(void)
{


}
