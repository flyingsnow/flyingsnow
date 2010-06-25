#include "main.h"
#include "interrupt.h"

DWORD	DATA	gSysTick; 					//system tick counter
TIMER 	sBlinkTimer;


main()
{
	SysTickInit();
	TimeOutSet(&sBlinkTimer,1000);
	P0CR = 0xff;
	P0PCR = 0xff;
	P0_4 = 1;
	while(1) {
		if(IsTimeOut(&sBlinkTimer)) {
			P0_4 = ~P0_4;
			TimeOutSet(&sBlinkTimer,1000);
		}
	}
	return 0;
}
