#include "clock.h"

bit SetFlag;		//set hour or min
bit ClockState = FALSE;		//False-display clock ,True-set clock
UCHAR SetClockTimer;


void ClockMain(void)
{
	switch(gKeyEvent) {
	case IN_KEY_CLK_SP:
		if(ClockState == FALSE) {	//display clock 
			if(System.DispMode != DISPLAY_CLOCK) {
				SetClockTimer = 10;
				System.DispMode = DISPLAY_CLOCK;
			}
			else {
				SetClockTimer = 0;
			}
		}
		else {				//setting clock
			if(SetFlag)
				SetFlag = FALSE;
			else 
				SetFlag = TRUE;
			SetClockTimer = 10;
		}

		break;

	case IN_KEY_CLK_CP:
		if(System.DispMode == DISPLAY_CLOCK) {
			if(ClockState == FALSE) {
				ClockState = TRUE;
				SetClockTimer = 10;
			}
			else {
				ClockState = FALSE;
				SetClockTimer = 10;
			}
		}

		break;


	case IN_KEY_NEXT_SP:
	case IN_KEY_NEXT_CP:
	case IN_KEY_NEXT_CPC:
		if(ClockState) {	//setting 
			if(!SetFlag){	//set hour
				if(t_hour < 23)
					t_hour++;
				else 
					t_hour = 0;
			}
			else {			//set min
				if(t_min < 59)
					t_min++;
				else
					t_min = 0;
			}
			
		}
		else {
			SetClockTimer = 0;
			ClockState = FALSE;
		}
		Blink = FALSE;
		SetClockTimer = 10;
		break;

	case IN_KEY_PRE_SP:
	case IN_KEY_PRE_CP:
	case IN_KEY_PRE_CPC:
		if(ClockState) {	//setting 
			if(!SetFlag){	//set hour
				if(t_hour > 0)
					t_hour--;
				else 
					t_hour = 23;
			}
			else {			//set min
				if(t_min > 0)
					t_min--;
				else
					t_min = 59;
			}
			
		}
		else {
			SetClockTimer = 0;
			ClockState = FALSE;
		}
		Blink = FALSE;
		SetClockTimer = 10;
		break;

	case IN_KEY_NONE:
	case IN_KEY_PRE_CPR:
	case IN_KEY_NEXT_CPR:

		break;

	default:
		SetClockTimer = 0;
		ClockState = FALSE;
		break;

	}

	if(System.DispMode == DISPLAY_CLOCK && SetClockTimer == 0) {
		if(ClockState == FALSE) {
			if(System.PowerMode == POWERMODE_POWERON) {
				if(System.WorkMode.Current == WORKMODE_RADIO)
					System.DispMode = DISPLAY_RADIO;
				else 
					System.DispMode = DISPLAY_AUX;		
			}
		}
		else {
			SetFlag = 0;
			ClockState = FALSE;
			SetClockTimer = 10;
		}
	}
}
