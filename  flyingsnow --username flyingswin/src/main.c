#include "main.h"
#include "interrupt.h"

DWORD	DATA	gSysTick; 					//system tick counter

UCHAR t_sec = 0;
UCHAR t_min = 0;
UCHAR t_hour = 0;					//clock 

TIMER 	sBlinkTimer;
KEY_EVENT	gKeyCode = IN_KEY_NONE;

VOID  InitGpio(VOID)
{
	P0CR |= 0x30; //P0.4 P0.5 output  
	P3CR |= 0xE0; //P3.5 .6 .7 output
	P4CR |= 0x18; //p4.3 .4 output
	
}



main()
{
	UINT Wait = 0x80;
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
	TimeOutSet(&sBlinkTimer,1000);
	Si4730_Power_Up(FM_RECEIVER);
	
	while(1) {
#if 0		
		DisplayMain();
		gKeyCode = AdcKeyEventGet();
		if(gKeyCode != IN_KEY_NONE) {
			_nop_();
		}
#endif 
	if(IsTimeOut(&sBlinkTimer))	{
		Wait = AdcReadChannel(ADC_CHANNEL_2);
		if(Wait < 600) {
			DispBuff[6] = Num[(Wait/100)];
			DispBuff[7] = Num[((Wait/10)%10)];
			DispBuff[8] = Num[(Wait%10)];
			DisplayMain();
			Si4730_Test();
		}
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

