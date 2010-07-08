#include "main.h"

extern 	VOID  InitGpio(VOID);

void PowerInit(void)
 /**
  * @brief: Initial the IOs for System and power ,and system parmeter 	  
  * @par Parameters:
  * 							  
  * @retval 
  */

{
	//Init system status 
	if(ACC_PIN)
		System.AccState = ACC_ONTEST;
	else
		System.AccState = ACC_OFF;
	System.PowerMode = POWERMODE_POWEROFF;
	System.WorkMode.Save = WORKMODE_RADIO;
	System.WorkMode.Current = WORKMODE_RADIO;
	System.DispMode = DISPLAY_AUX;
}


void ACC_Check(void)
/**
 * @brief: check Acc pin state 	 
 * @par Parameters:
 *								 
 * @retval 
 */

{
	switch(System.AccState) {
	case ACC_OFF:
		if(ACC_PIN) {
			System.AccState = ACC_ONTEST;
		}
		break;
	case ACC_ONTEST:
		if(!ACC_PIN) {
			System.AccState = ACC_OFF;
		}
		else {
//			SysTickInit();
			InitGpio();
			SYS_ON();
			WaitUs(0xff);
			InitAdc();
			DisplayInit();
			System.AccState = ACC_ON;	
		}
		break;
	case ACC_ON:
		if (!ACC_PIN) {
			System.AccState = ACC_OFFTEST;
		}
		break;
	case ACC_OFFTEST:
		if (ACC_PIN) {
			System.AccState = ACC_ON;
		}
		else {				
			MUTE_AMP();	 //7377 mute
			if(System.PowerMode == POWERMODE_POWERON)	//save powermode for next acc on
				System.PowerMode = POWERMODE_POWERONREQ;
			System.AccState = ACCMODE_ACCOFF;
			if (System.WorkMode.Current == WORKMODE_AUX)
				System.WorkMode.Save = System.WorkMode.Current;
			else
				System.WorkMode.Save = WORKMODE_RADIO;	
			
			POWER_OFF();	//power off				
			SYS_OFF();		//sys off
			System.AccState = ACC_OFF;

		}
	}
}

	
	
void Power_main(void)	
/**
* @brief: main routine for power on/off  	
* @par Parameters:
*								
* @retval 
*/
{	
	
	switch(System.PowerMode) {			
	case POWERMODE_POWERON:   
		 break;   
		 
	case POWERMODE_POWERONREQ:	 
		 //here power on the tuner/audioprocessor 			 
		 POWER_ON();				
		 
		 if (System.WorkMode.Save == WORKMODE_AUX) 
 			 System.WorkMode.Current = System.WorkMode.Save;
 		 else 
 		 	System.WorkMode.Current = WORKMODE_RADIO;

	     if(System.WorkMode.Current == WORKMODE_AUX)  {
			WaitMs(500);
			SC7313_initial(Channel_Aux);
		 }
		 else {
			 WaitMs(200);
			 //Here boot the tuner...	 
//				 while ( Si4730_Test() != OK ) {
				 Tuner_Init(SaveBand,SaveFreq);	
//				 }
			 SC7313_initial(Channel_Radio);	
		 }
		 
		 UnMUTE_AMP();
		 System.PowerMode = POWERMODE_POWERON;				 

		 break;   
		 
	case POWERMODE_POWEROFF:   
		 break;   
		 
	case POWERMODE_POWEROFFREQ:			
 		 MUTE_AMP();			 
		 WaitMs(200);
		 Si4730_Power_Down();			 
		 POWER_OFF();							//set power-on pin low
		 TunerStatus = Status_Idle;							//reset radio status 
		 System.PowerMode = POWERMODE_POWEROFF;
		 //Modify by Hwt Dec,22,2009
		 System.WorkMode.Save = System.WorkMode.Current;
		 
		 
		 break;   
	}	
}	

