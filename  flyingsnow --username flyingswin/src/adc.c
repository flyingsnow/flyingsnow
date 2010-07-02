#include "main.h"

VOID InitAdc(VOID)
{
	ADCH = 0x06;		//enable channle 1,2
	ADCON |= 0x80;		//enable adc
	ADT = 0x27;			//Tad = 4* Tsys 
}



UINT AdcReadChannel(ADC_Channel_TypeDef ADC_Channel)
{
	UINT  temph;
	UCHAR templ;
	UCHAR wait_conuter;
	
	ADCON |= 0x80;		//enable adc

	ADCON &= 0xF1;					//clear channel 	
	ADCON |= ADC_Channel << 1;		//set channel 
	ADCON |= 0x01;					//start AD convert. 

	/* Waiting for AD convert finished    */
	while((ADCON & 0x01)&& wait_conuter < 0x80)	//loop_counter limit should guarantee at least 10us
		wait_conuter++;	

//	while(templ--)
//		;
	
	temph = ADDH;		//Read MSB
	templ = ADDL;		//Read LSB

	temph = temph << 2;

	return (UINT)temph|templ;


}

