#include "main.h"
#include "interrupt.h"

DWORD	DATA	gSysTick; 					//system tick counter
TIMER 	sBlinkTimer;

VOID  InitGpio(VOID)
{
	P0CR |= 0x30; //P0.4 P0.5 output  
	P3CR |= 0xE0; //P3.5 .6 .7 output
	P4CR |= 0x18; //p4.3 .4 output
	
}

VOID InitAdc(VOID)
{


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
	TimeOutSet(&sBlinkTimer,1000);
	while(1) {
		if(IsTimeOut(&sBlinkTimer)) {
			P0_4 = ~P0_4;
			TimeOutSet(&sBlinkTimer,1000);
		}
	}
	return 0;
}
