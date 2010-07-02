#if !defined(__ADC_KEY_H__)
#define __ADC_KEY_H__



/*
**********************************************************
*					GLOBAL MACRO
**********************************************************
*/
#define	KEY_EVENT_BASE_ADDR		0xA0



/*
**********************************************************
*					GLOBAL DATA TYPE
**********************************************************
*/
//
//define key event.
//
typedef enum _KEY_EVENT
{
	IN_KEY_NONE = 0x00,
		
	IN_KEY_MOD_SP = KEY_EVENT_BASE_ADDR,
	
	IN_KEY_VOL_ADD,
	IN_KEY_VOL_CADD,
	
	IN_KEY_VOL_SUB,	
	IN_KEY_VOL_CSUB, 
	
	IN_KEY_NEXT_SP,
	IN_KEY_NEXT_CP,
	IN_KEY_NEXT_CPC,
	IN_KEY_NEXT_CPR,
	
	IN_KEY_PRE_SP,
	IN_KEY_PRE_CP,	
	IN_KEY_PRE_CPC,
	IN_KEY_PRE_CPR,
	

	IN_KEY_AST_SP,
	IN_KEY_AST_CP,

	IN_KEY_CLK_SP,
	IN_KEY_CLK_CP,

	IN_KEY_BAND_SP,


	IN_KEY_PWR_SP,
	IN_KEY_PWR_CP,
	
	IN_KEY_SEL_SP,
	IN_KEY_SEL_CP,

	IN_KEY_P1_SP,
	IN_KEY_P1_CP,
	
	IN_KEY_P2_SP,
	IN_KEY_P2_CP,
	
	IN_KEY_P3_SP,
	IN_KEY_P3_CP,

	IN_KEY_P4_SP,
	IN_KEY_P4_CP,

	IN_KEY_P5_SP,
	IN_KEY_P5_CP,

	IN_KEY_P6_SP,
	IN_KEY_P6_CP

} KEY_EVENT;


/**
  * @brief ADC analog channel selection
  */
typedef enum {
  ADC_CHANNEL_0  = (UCHAR)0x00, /* Analog channel 0 */
  ADC_CHANNEL_1  = (UCHAR)0x01, /* Analog channel 1 */
  ADC_CHANNEL_2  = (UCHAR)0x02, /* Analog channel 2 */
  ADC_CHANNEL_3  = (UCHAR)0x03, /* Analog channel 3 */
  ADC_CHANNEL_4  = (UCHAR)0x04, /* Analog channel 4 */
  ADC_CHANNEL_5  = (UCHAR)0x05  /* Analog channel 5 */
} ADC_Channel_TypeDef;




typedef enum _ADC_KEY_STATE
{
	ADC_KEY_STATE_IDLE,
	ADC_KEY_STATE_JITTER,
	ADC_KEY_STATE_PRESS_DOWN,
	ADC_KEY_STATE_SP,
	ADC_KEY_STATE_CP,
	ADC_KEY_STATE_CPR

} ADC_KEY_STATE;

/*
**********************************************************
*					GLOBAL FUNCTION PROTOTYPE
**********************************************************
*/

VOID InitAdc(VOID);

UINT AdcReadChannel(ADC_Channel_TypeDef ADC_Channel);

//
// Initialize adc key scan (GPIO) operation.
//
VOID
AdcKeyScanInit(VOID);


//
// Key process, image key value to key event.
//
KEY_EVENT
AdcKeyEventGet(VOID);

#endif 
