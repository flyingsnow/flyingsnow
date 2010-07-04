#include "main.h"

#define	SC7313ADDR	0x88

signed char CurrentVol = DEFVOL;
signed char SetBASS = 7;
signed char SetTREB = 7;
signed char SetBAL = 9;
//signed char SetFAD = 9;

UCHAR DispVol;
BOOL  ChangeVolFlag;


UCHAR EQ_Item = 0;
#if 1
UCHAR CODE EQ_Buff[25] = { 0x8A,0xfA,0x24,0x60,0x00,	//VOL
				 0x00,0xd6,0xd1,0x6D,0xd0,  //BAS
				 0x00,0x9E,0x59,0x60,0x50,  //TRE
				 0x00,0x8A,0xD1,0x6D,0xd0,	//BAL
				 0x00,0xeC,0xd1,0x64,0x16   //FAD
				};
#else 
u8 EQ_Buff[25] = {  0x00,0x60,0x24,0xFA,0x8A,   //VOL
					0xD0,0x6D, 0xD1,0xD6,0x00,   //BAS
					0x50, 0x60,0x59,0x9E,0x00,   //TRE
					0xD0,0x6D,0xD1,0x8A,0x00,   //BAL
					0x16,0x64,0xD1,0xEC,0x00   //FAD
				 };
#endif

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

