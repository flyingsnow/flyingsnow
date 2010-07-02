#include "main.h"
#include "interrupt.h"

DWORD	DATA	gSysTick; 					//system tick counter

UCHAR t_sec = 0;
UCHAR t_min = 0;
UCHAR t_hour = 0;					//clock 

TIMER 	DATA sBlinkTimer;
KEY_EVENT	gKeyCode = IN_KEY_NONE;

VOID  InitGpio(VOID)
{
	P0CR |= 0x30; //P0.4 P0.5 output  
	P3CR |= 0xF0; //P3_4.5 .6 .7 output
	P4CR |= 0x18; //p4.3 .4 output
	
}



main()
{
	UCHAR Wait = 0x80;
	EA = 0;
	CLKCON = 0xC8;
	while(Wait--){
		_nop_();
	}
	CLKCON |= 0x04;
	SysTickInit();
	InitGpio();
	InitAdc();
	SYS_ON();
	POWER_ON();
	AdcKeyScanInit();
	DisplayInit();
	DisplayInit();
	Si4730_Power_Up(FM_RECEIVER);
	Si4730_Tune_Freq(Band_FM1,9240);
	Delay(0xffff);
	SC7313_initial(Channel_Radio);	
	TimeOutSet(&sBlinkTimer,1000);
	UnMUTE_AMP();
	Wait = 0;
	while(1) {
#if 0		
		DisplayMain();
		gKeyCode = AdcKeyEventGet();
		if(gKeyCode != IN_KEY_NONE) {
			_nop_();
		}
#endif 
	gKeyCode = AdcKeyEventGet();
	if(gKeyCode != IN_KEY_NONE){
		SC7313_initial(Channel_Radio);	
		Si4730_Tune_Freq(Band_FM1,8950);
//		P0_5 = ~P0_5;
		DispBuff[6] = Num[(gKeyCode/100)];
		DispBuff[7] = Num[((gKeyCode/10)%10)];
		DispBuff[8] = Num[(gKeyCode%10)];
		DisplayMain();
	}
	if(IsTimeOut(&sBlinkTimer))	{
	//	Wait = AdcReadChannel(ADC_CHANNEL_2);
	//	Wait = AdcKeyEventGet();
	//	if(Wait!= IN_KEY_NONE) {
	//		DispBuff[6] = Num[(Wait/100)];
	//		DispBuff[7] = Num[((Wait/10)%10)];
////			DispBuff[8] = Num[(t_sec%10)];
//			DisplayMain();
//			Si4730_Test();
	//	}
	//	P0_5 = ~P0_5;
		TimeOutSet(&sBlinkTimer,1000);
		}	

	}
	return 0;
}


void Delay( UINT DelayTime )
{
	UINT i = DelayTime;
	while( i-- );
}

