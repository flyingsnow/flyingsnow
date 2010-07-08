#ifndef		__POWER_H
#define 	__POWER_H

#define SYS_ON()		P0_4 = 1
#define POWER_ON()		P0_5 = 1

#define SYS_OFF()		P0_4 = 0
#define POWER_OFF()		P0_5 = 0
	
#define ACC_PIN			P0_3

//Power manage adn work mode control
typedef enum _WORKMODE
{	
	WORKMODE_RADIO,			 
	WORKMODE_AUX	 
}WORKMODE;

typedef enum _POWER_MODE
{	
	POWERMODE_POWERON,
	POWERMODE_POWERONREQ,
	POWERMODE_POWEROFF, 
	POWERMODE_POWEROFFREQ
}POWER_MODE;

typedef enum _ACC_STATE
{
	ACC_OFF,
	ACC_OFFTEST,
	ACC_ON,
	ACC_ONTEST
}ACC_STATE;

typedef enum _ACC_MODE
{
	ACCMODE_ACCON,
	ACCMODE_ACCOFF
}ACC_MODE;



void PowerInit(void);

/**
* @brief: main routine for power on/off  	
* @par Parameters:
*								
* @retval 
*/
void Power_main(void);	


/**
 * @brief: check Acc pin state ,system +5V control 	 
 * @par Parameters:
 *								 
 * @retval 
 */
void ACC_Check(void);


#endif 
