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
