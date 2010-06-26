#include "main.h"

VOID InitAdc(VOID)
{
	ADCH = 0x07;		//enable channle 0,1,2
	ADCON |= 0x80;		//enable adc
	ADT = 0x20;			//Tad = 4 Tsys 
}

/**
   * @brief Start ADC2 conversion
  */
static void ADC_Start(ADC_Channel_TypeDef ADC_Channel)
{	
	ADCON &= 0xF1;					//clear channel  	
	ADCON |= ADC_Channel << 1;		//set channel 
	ADCON |= 0x01;					 	//start AD convert.         
}


/**
   * @brief Get one sample of measured signal.
   */
static UINT ADC_GetConversionValue(void)
{
	UINT  temph;
	UCHAR templ;

	
	temph = ADDH;		//Read MSB
	templ = ADDL;		//Read LSB

	temph = temph << 2;

	return (UINT)temph|templ;

}


UINT AdcReadChannel(ADC_Channel_TypeDef ADC_Channel)
{
	UCHAR wait_conuter;
	
	ADC_Start(ADC_Channel);
	
	/* Waiting for AD convert finished (ECOF=1).      */
	while((ADCON & 0x01)&& wait_conuter < 0x80)	//loop_counter limit should guarantee at least 10us
		wait_conuter++;	
	
	return ADC_GetConversionValue();

}

