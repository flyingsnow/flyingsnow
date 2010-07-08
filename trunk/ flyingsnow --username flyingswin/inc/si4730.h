#ifndef __SI4730_H
#define __SI4730_H

#include "i2c.h"

#define SI4730_Addr		0xC6


/*-------------------------------------------------
define for  Read signal status information and mask
--------------------------------------------------*/
#define 	Read_VALID		2
#define 	Read_PILOT		3
#define 	Read_RSSI		4				//current receive signal strength
#define 	Read_FREQOFFSET	7				//Signed frequency offset

#define 	Read_Bit_VALID	0x01			//Indicates valid channel 
#define 	Read_Bit_PILOT	0x80			//Indicates stereo pilot


#define RST_LOW		P4_3 = 0
#define RST_HIGH	P4_3 = 1

typedef enum OPERA_MODE {
    READ = 1,
    WRITE = 2
} T_OPERA_MODE;

typedef enum POWER_UP_TYPE {
    FM_RECEIVER = 1,
    AM_RECEIVER = 2
} T_POWER_UP_TYPE;

typedef enum ERROR_OP {
    OK = 1,
    I2C_ERROR = 2,
    LOOP_EXP_ERROR = 3
//    ERROR = 4
} T_ERROR_OP;

/* Exported functions ------------------------------------------------------- */


/**
* @brief	Initiates the boot process to move the device from powerdown to powerup mode.
* @par Parameters: power_up_type  Operating mode
*								
* @retval 
* @par Required preconditions:  1 = FM_RECEIVER , 2 =  AM_RECEIVER
*/

T_ERROR_OP Si4730_Power_Up(T_POWER_UP_TYPE power_up_type);



/**
* @brief	To Power Down the device.
* @par Parameters:None
*								
* @retval  OK or Error code
* @par Required preconditions:
*/
T_ERROR_OP Si4730_Power_Down(void);

	
/**
* @brief	Set property for si4730 
* @par Parameters: Property 
*								
* @retval 
* @par Required preconditions: 
*/
T_ERROR_OP Si4730_Set_Property(UCHAR *Si4730_set_property);



/**
* @brief	
* @par Parameters: 
*								
* @retval 
* @par Required preconditions: 
*/
T_ERROR_OP Si4730_Tune_Freq(UCHAR band,UINT freq);



/**
* @brief	Return status information about the received signal quality
* @par Parameters: band , for choose Command for AM or FM
*								
* @retval 
* @par Required preconditions: 
*/
UCHAR Si4730_RSQ_Status(UCHAR band , UCHAR Read_Status);

/**
* @brief	Return status information about the tuner 
* @par Parameters: 
*								
* @retval 
* @par Required preconditions: 
*/
// T_ERROR_OP Si4730_Test(void);


/**
* @brief	mute or unmute 4730
* @par Parameters: 
*								
* @retval 
* @par Required preconditions: 
*/
T_ERROR_OP Si4730_HardMute(bit MuteState);

#endif

