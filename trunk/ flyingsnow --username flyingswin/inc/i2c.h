#include "main.h"


#define SDA2_H		P4_4 = 1
#define SDA2_L		P4_4 = 0

#define SCL2_H		P3_7 = 1
#define SCL2_L		P3_7 = 0

#define SDA2_read	P4_4

//Define i2c i/o pins for SC7313

#define SCL1_H		P3_6 = 1
#define SDA1_H		P3_6 = 0

#define SCL1_L		P3_7 = 1
#define SDA1_L		P3_7 = 1

#define SDA1_read 	P3_6


BOOL I2C1_write(UCHAR i2c_adr,  UCHAR *buffer, UCHAR NumtoWrite);
BOOL I2C2_write(UCHAR i2c_adr,  UCHAR *buffer, UCHAR NumtoWrite);

BOOL I2C1_read(UCHAR i2c_adr,UCHAR *buffer, UCHAR NumtoRead);
BOOL I2C2_read(UCHAR i2c_adr,UCHAR *buffer, UCHAR NumtoRead);

void I2C_delay(UCHAR delay_time);/* delay function used for I2C*/

void I2C1_Start(void);

/******************* (C) COPYRIGHT 2009 SENLi *****END OF FILE****/

