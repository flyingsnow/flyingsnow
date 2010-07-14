#include "main.h"

UCHAR Si4730_reg_data[9]; 


 
/***********************************************
Before reset make sure following for 2-wire mode:
 RCLK = 32KHz~40MHz, set by "GPIO_ModeSetup(RCLK,3)"=>32.768KHz
 GPO1 = 1;
 GPO2 = 0;
************************************************/
 void ResetSi4730(void)
 {
	 
	 //SDA2_H;
	 //SCL2_H;
	 
	 RST_LOW;
	 //  SCL2_H;				 //delay 100us	 
	 WaitUs(0x100);					 
	 RST_HIGH;
	 //  Delay(50);
	 //  SDA2_H; 
	 //  Delay(50);
	 
}

T_ERROR_OP Si4730_Wait_CTS(void) 

/**
* @brief	Wait CTS = 1,clear to send next command
* @par Parameters None
*								
* @retval  OK or Error 
* @par Required preconditions:  
*/	
{	
	UINT loop_counter = 0;
#if 1
	do
	{	
		I2C2_read(SI4730_Addr, &(Si4730_reg_data[0]), 1);	
		loop_counter++;
	}
	while(((Si4730_reg_data[0]&0x80) == 0) && (loop_counter < 0xff));  //loop_counter limit should guarantee at least 300us
		
	if(loop_counter >= 0xff) {
//		ResetSi4730();
//		Si4730_Power_Down();
//		Tuner_Init(SaveBand,SaveFrequency);	
//		if (RST_4730() != OK)
			return LOOP_EXP_ERROR;
	}
#else

	loop_counter = 1500;
	while(loop_counter--)
		;
#endif
	return OK;

}






T_ERROR_OP Si4730_Wait_STC(void)
/**
* @brief	Wait STC = 1,Seek/Tune Complete
* @par Parameters None
*								
* @retval  OK or Error 
* @par Required preconditions:  
*/		
{
	UINT loop_counter = 0;
	UCHAR Si4730_get_int_status[] = {0x14};	

	
	/* wait CTS = 1 and then wait STCINT = 1 */
	do
	{	
		//send CMD
		
		I2C2_write(SI4730_Addr, Si4730_get_int_status, 1);

	 	/* wait CTS = 1  */
		if(Si4730_Wait_CTS() != OK)
			return LOOP_EXP_ERROR;
	  
	  	I2C2_read(SI4730_Addr, &(Si4730_reg_data[0]), 1);   

	  	loop_counter++;
	}
	while(((Si4730_reg_data[0]&0x01) == 0) && (loop_counter < 0xffff));  //loop_counter(0xffff) limit should guarantee at least: for FM,worst case = 60ms*((108-87.5)/0.1+1)= 12s to wait STC = 1
																																			 //																										   for AM,worst case = 80ms*((1710-520)/9+1)= 11s to wait STC = 1
	if(loop_counter == 0xffff)
		return LOOP_EXP_ERROR;	

	return OK;

}


#if 0
/**************************************

Si47XX_Get_Rev(): read rev info: you should read out: 

{0x80,0x14,0x31,0x42,0x00,0x00,0x31,0x42,0x41} for Si4720/1B firmware, Chip rev = A(0x41).

note! only can read it correctly after power up!

***************************************/
T_ERROR_OP Si4730_Test(void)
{
	unsigned short loop_counter = 0;
	unsigned char Si47XX_get_rev[] = {0x10,0x00};	
	Si4730_reg_data[0] = 0;
	//send CMD
 	I2C2_write(SI4730_Addr, &(Si47XX_get_rev[0]), 2);

	//wait CTS = 1
	do
	{	
		I2C2_read(SI4730_Addr, &(Si4730_reg_data[0]), 1);	
		loop_counter++;
	}
	while(((Si4730_reg_data[0]&0x80) == 0) && (loop_counter < 0xff));  //loop_counter limit should guarantee at least 300us

	if(loop_counter >= 0xff)
		return LOOP_EXP_ERROR;


	//read rev info: you should read out: {0x80,0x14,0x31,0x42,0x00,0x00,0x31,0x42,0x41} for 1B firmware
		
	I2C2_read(SI4730_Addr, &(Si4730_reg_data[0]), 4);

	if(Si4730_reg_data[3] != 0x30 )
		return ERROR;
		
	return OK;
}
#endif

T_ERROR_OP Si4730_Power_Up(T_POWER_UP_TYPE power_up_type)
/**
* @brief	Initiates the boot process to move the device from powerdown to power up mode.
* @par Parameters: power_up_type  Operating mode
*								
* @retval 
* @par Required preconditions:  1 = FM_RECEIVER , 2 =  AM_RECEIVER
*/


{
	UCHAR Si4730_power_up[] = {0x01,0x41,0x05};
	
	switch(power_up_type)
	{
		case FM_RECEIVER:		
			Si4730_power_up[1] = 0xD0;
			Si4730_power_up[2] = 0x05;  //note! for Si4730(AN385rev0.2/0.3), it's 0x05(from 0.C to 1.0)
																	//note! for Si4704/5(AN326rev0.2):
																	//0x05:analog audio
																	//0xb0:digital audio(Si4705 only with xoscen=0)
																	//0xb5:both (Si4705 only with xoscen=0)			
			break;		

	    case AM_RECEIVER:   		
	    	Si4730_power_up[1] = 0xD1;
	    	Si4730_power_up[2] = 0x05;
	    	break;
   	
  }
    	
	ResetSi4730();

	//send CMD
	I2C2_write(SI4730_Addr, Si4730_power_up, 3);

//	DELAY(POWER_SETTLING);	//if use internal crystal, need wait 500ms, Si4730_power_up[1] = 0x(5x)
	WaitUs(100);
	//wait CTS = 1
	if(Si4730_Wait_CTS() != OK)
		return LOOP_EXP_ERROR;		

		
	return OK;
}





T_ERROR_OP Si4730_Power_Down(void)
/**
* @brief	To Power Down the device.
* @par Parameters:None
*								
* @retval  OK or Error code
* @par Required preconditions:
*/
{
	UCHAR CODE Si4730_power_down[1] = {0x11};	

	//send CMD
	I2C2_write(SI4730_Addr, Si4730_power_down,1);

	//wait CTS = 1	
	if(Si4730_Wait_CTS() != OK)
		return LOOP_EXP_ERROR;	
	
	return OK;
}






T_ERROR_OP Si4730_Set_Property(UCHAR *Si4730_set_property)
	
/**
* @brief	Set property for si4730 
* @par Parameters: Property 
*								
* @retval 
* @par Required preconditions: 
*/
{	
	//send CMD
	I2C2_write(SI4730_Addr, Si4730_set_property, 6);

	//wait CTS = 1
	if(Si4730_Wait_CTS() != OK)
		return LOOP_EXP_ERROR;	

	return OK;

}




T_ERROR_OP Si4730_Tune_Freq(UCHAR band,UINT freq)
/**
* @brief	tune to a preset frequency
* @par Parameters:  band 
*				   freq			
* @retval 
* @par Required preconditions: 
*/

{	
//	UINT minfeq,maxfeq;
	UCHAR Si4730_tune_freq[5] = {0x20,0x00,0x00,0x00,0x00};	//0x27F6=10230KHz
	
//	UINT loop_counter = 5000;
	switch(band) {
		case Band_FM1:
		case Band_FM2:
		case Band_FM3:				
//			minfeq = FMmin;
//			maxfeq = FMmax;
			Si4730_tune_freq[0] = 0x20;
			break;
		case Band_MW1:
		case Band_MW2:			
//			minfeq = MWmin;
//			maxfeq = MWmax;
			Si4730_tune_freq[0] = 0x40;
			break;
	}	
/*
	if (freq < minfeq) {
		freq = minfeq;
	}
	if (freq > maxfeq) {
		freq = maxfeq;
	}
*/
//save the frequency infomation 
	SaveBand = band;
	SaveFreq = freq; 

	Si4730_tune_freq[3] = (freq&0x00ff);
	Si4730_tune_freq[2] = (freq&0xff00) >> 8;
	

	//send CMD
	I2C2_write(SI4730_Addr, Si4730_tune_freq, 5);

#if 1
	//wait CTS = 1
	if(Si4730_Wait_CTS() != OK)
		return LOOP_EXP_ERROR;

	//wait STC = 1 ,SEEK/TUNE Complete	
	if(Si4730_Wait_STC() != OK) 
		return ERROR;	
#else 
	while(loop_counter--)
		;

#endif
	return OK;

}


UCHAR Si4730_RSQ_Status(UCHAR band , UCHAR Read_Status)
/**
* @brief	Return status information about the received signal quality
* @par Parameters: band , for choose Command for AM or FM
*								
* @retval 
* @par Required preconditions: 
*/
{
	UCHAR Si4730_rsq_status[] = {0x23,0x00};	
//	UCHAR loop_counter = 1500;
	
	switch(band) {
		case Band_FM1:
		case Band_FM2:
		case Band_FM3:				
			Si4730_rsq_status[0] = 0x23;
			break;
		case Band_MW1:
		case Band_MW2:			
			Si4730_rsq_status[0] = 0x43;
			break;
	}	
	//send CMD
	I2C2_write(SI4730_Addr, Si4730_rsq_status, 2);

	//wait CTS = 1
#if 1	
	Si4730_Wait_CTS();
#else
	while(loop_counter--)
		;
#endif
	//Read RSQ status
	I2C2_read(SI4730_Addr, &(Si4730_reg_data[0]), 8);	
			
	switch (Read_Status) {
		case Read_VALID:
			return (Si4730_reg_data[Read_VALID] & Read_Bit_VALID);
			break;
			
		case Read_PILOT:
			return (Si4730_reg_data[Read_PILOT] & Read_Bit_PILOT);
			break;
			
		case Read_RSSI:
			return Si4730_reg_data[Read_RSSI];
			break;
			
		case Read_FREQOFFSET:
			return Si4730_reg_data[Read_FREQOFFSET];
			
			break;
 		default:
			break;

	}
}

T_ERROR_OP Si4730_HardMute(bit MuteState)
/**
* @brief	Mutes the audio output
* @par Parameters: MuteState 
*					TRUE: mute	FALSE: unmute
*								
* @retval 
* @par Required preconditions: 
*/

{
	
	BYTE Si4730_Property_data[6] = {0x12,0x00,0x40,0x01,0x00,0x00};	

	if(MuteState) {
		Si4730_Property_data[5] = 0x03;
	}
	else {		
		Si4730_Property_data[5] = 0x00;
	}

	
	if (Si4730_Set_Property(Si4730_Property_data) != OK)
			return LOOP_EXP_ERROR;	
		
	return OK;

}
