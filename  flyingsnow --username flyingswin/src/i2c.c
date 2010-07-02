/*************************************************************************** 
Project  :  STM8S-TEF660X  
Compiler : COSMIC
Module   :  Stm8s_i2c.h 
Version  :  V 1.0 
Created  : Jun.29th,2009 
Author   :  Ivan HE / SENLi-HeiFei 
Description 
         -  IIC  file 
Modified 
	  -      
***************************************************************************/ 
#include "i2c.h"






/**
Send the star signal to device
Call parameter: None
return value:  None

*/	
void I2C1_Start(void)
{
	SDA1_H;
	SCL1_H;
	I2C_delay(1);
	SDA1_L;
	I2C_delay(1);
  SCL1_L;				
}

void I2C2_Start(void)
{
	SDA2_H;
	SCL2_H;
	I2C_delay(1);
	SDA2_L;
	I2C_delay(1);
  SCL2_L;			
}

/**
send stop signal to device
call parameter:None
return value:None
*/
void I2C1_Stop(void)
{

	SDA1_L;
	_nop_();
	SCL1_H;
	I2C_delay(1);
	SDA1_H;
	I2C_delay(1);
}
void I2C2_Stop(void)
{

	SDA2_L;
	_nop_();
	SCL2_H;
	I2C_delay(1);
	SDA2_H;
	I2C_delay(1);
}

/**
   Detect if the slave device send the ACK signal to host device
   call parameter:None
   return value: 
                TRUE: slave device answer ACK 
                FALSE: no slave device answer ACK
*/

BOOL I2C1_WaitAck(void) 	 
{
	SCL1_L;
	I2C_delay(1);
	SDA1_H;			
	I2C_delay(1);
	P3CR &= 0xBF;	//set p3_6 as input 
	SCL1_H;
	I2C_delay(1);
	if(SDA1_read)
	{
      SCL1_L;
	  P3CR |= 0x40;
      return FALSE;
	}
	SCL1_L;
	P3CR |= 0x40;
	return TRUE;
}

BOOL I2C2_WaitAck(void) 	 
{
	SCL2_L;
	I2C_delay(1);
	SDA2_H;			
	I2C_delay(1);
	P4CR &= 0xEF;	//set p4_4 as input 
	SCL2_H;
	I2C_delay(1);
	if(SDA2_read)
	{
      SCL2_L;
	  P4CR |= 0x10;
      return FALSE;
	}
	SCL2_L;
	P4CR |= 0x10;
	return TRUE;
}

#if 0
/*
send a ACK signal to slave device,used when master read data from slave
return value: None
call parameter:None
*/
void I2C1_Ack(void)
{	
	SCL1_L;
	I2C_delay(1);
	SDA1_L;
	I2C_delay(1);
	SCL1_H;
	I2C_delay(1);
	SCL1_L;
	I2C_delay(1);
}
#endif 

void I2C2_Ack(void)
{	
	SCL2_L;
	I2C_delay(1);
	SDA2_L;
	I2C_delay(1);
	SCL2_H;
	I2C_delay(1);
	SCL2_L;
	I2C_delay(1);
}

#if 0
/**
send a nACK signal to slave device,used when master read data from slave
return value: None
call parameter:None
*/

void I2C1_nAck(void)
{
	SCL1_L;
	I2C_delay(1);
	SDA1_H;
	I2C_delay(1);
	SCL1_H;
	I2C_delay(1);
	SCL1_L;
	I2C_delay(1);

}
#endif 

void I2C2_nAck(void)
{
	SCL2_L;
	I2C_delay(1);
	SDA2_H;
	I2C_delay(1);
	SCL2_H;
	I2C_delay(1);
	SCL2_L;
	I2C_delay(1);

}

/**
Send one byte data to slave device
return value: None
call parameter:
              char i2c_data: the data will be sent
*/

void I2C1_TxByte(char i2c_data)
{
	UCHAR i = 8;
	while(i--) {
		SCL1_L;
		I2C_delay(1); 
		if( i2c_data & 0x80)
			SDA1_H;
		else 
			SDA1_L;
		i2c_data <<= 1;
		I2C_delay(1);

		SCL1_H;
		I2C_delay(1);
	}
	SCL1_L;
}

void I2C2_TxByte(char i2c_data)
{
	UCHAR i = 8;
	while(i--) {
		SCL2_L;
		I2C_delay(1); 
		if( i2c_data & 0x80)
			SDA2_H;
		else 
			SDA2_L;
		i2c_data <<= 1;
		I2C_delay(1);

		SCL2_H;
		I2C_delay(1);
	}
	SCL2_L;
}

#if 0
/**
  Read one byte data from slave device
  return value: the UCHAR type data from slave deivce
  call parameter: None
*/

UCHAR I2C1_RxByte(void)
{

   UCHAR I2cRxData;   
   UCHAR i = 8;   
   I2cRxData = 0;   
  	
	SDA1_H;
	P3CR &= 0xBF;		//set p3_6 as input mode	
	while (i--) {  
		I2cRxData <<= 1;  
		SCL1_L;   
		I2C_delay(1);
		SCL1_H;
		I2C_delay(1);
		if(SDA1_read)  {   
			I2cRxData |=0x01; 
		}	
	}  
	P3CR |= 0x01 << 6;
	SCL1_L;  
	return(I2cRxData);   
	
	
}
#endif 

UCHAR I2C2_RxByte(void)
{

   UCHAR I2cRxData;   
   UCHAR i = 8;   
   I2cRxData = 0;   
  	
   SDA2_H;
   P4CR &= 0xEF;
   while (i--) {  
  	I2cRxData <<= 1;  
    SCL2_L;   
    I2C_delay(1);
	SCL2_H;
	I2C_delay(1);
	 
 	if(SDA2_read)  {   
      I2cRxData |=0x01;  
	}
  }
  P4CR |= 0x10;
  SCL2_L;  
  return(I2cRxData);   
	
	
}

/**
 send a group of data to slave device
 return value: BOOL type,
             TRUE: the data sent to slave device successly 
             FALSW:the data not sent to slave device successly
 call parameter:
            UCHAR i2c_adr: address of the slave device data will be sent to
            UCHAR * buffer: a point point to the data buffer
            UCHAR Numto Write: the data number (byte)  
  */

BOOL I2C1_write(UCHAR i2c_adr,UCHAR * buffer, UCHAR NumtoWrite)
{
	I2C1_Start();
	I2C1_TxByte(i2c_adr & 0xFE);
	if (!I2C1_WaitAck()) {
		I2C1_Stop();
		return FALSE;
	}
	while (NumtoWrite) {
		I2C1_TxByte(*buffer);
		I2C1_WaitAck();
		buffer++;
		NumtoWrite--;
	}
	I2C1_Stop();

	return TRUE;

}

BOOL I2C2_write(UCHAR i2c_adr,UCHAR * buffer, UCHAR NumtoWrite)
{
	I2C2_Start();
	I2C2_TxByte(i2c_adr & 0xFE);
	if (!I2C2_WaitAck()) {
		I2C2_Stop();
		return FALSE;
	}
	while (NumtoWrite) {
		I2C2_TxByte(*buffer);
		I2C2_WaitAck();
		buffer++;
		NumtoWrite--;
	}
	I2C2_Stop();

	return TRUE;

}

#if 0

/**
  read data from slave device
  return value: BOOL,
               TRUE: read the data successly
               FALSE: read the data unsuccessly
  call parameter:
               UCHAR i2c_adr:   the address of the slave divce
               UCHAR * buffer:  the pion poin to the buffer for the read data
               UCHAR NumtoRead: the data number be read.
*/


BOOL I2C1_read(UCHAR i2c_adr,UCHAR * buffer,UCHAR NumtoRead)
{
	
	UCHAR i;
	I2C1_Start();
	I2C1_TxByte(i2c_adr +1 );
	if (!I2C1_WaitAck()) {
		I2C1_Stop();
		return FALSE;
	}
	 
	for(i=0;i<NumtoRead-1;i++) {
		*buffer = I2C1_RxByte();
		I2C1_Ack();
		buffer++;	
	}
		*buffer = I2C1_RxByte();
		I2C1_nAck();
		I2C1_Stop();

	return TRUE;

}
#endif 

BOOL I2C2_read(UCHAR i2c_adr,UCHAR * buffer,UCHAR NumtoRead)
{
	
	UCHAR i;
	I2C2_Start();
	I2C2_TxByte(i2c_adr +1 );
	if (!I2C2_WaitAck()) {
		I2C2_Stop();
		return FALSE;
	}
	 
	for(i=0;i<NumtoRead-1;i++) {
		*buffer = I2C2_RxByte();
		I2C2_Ack();
		buffer++;	
	}
		*buffer = I2C2_RxByte();
		I2C2_nAck();
		I2C2_Stop();

	return TRUE;

}




/* delay function used for I2C 
return value: None
call parameter: UCHAR delay_time: delay time length
*/
void I2C_delay(UCHAR delay_time)
{
#if 0
   UCHAR i=50;   /* modify the i by the main clock frequency */ 
   while(delay_time--){
     while(i)i--;
     i=50;
   } 
#endif 
	UCHAR i=5;			//modify   50  
   while(i) 
   { 
     i--; 
   } 
   
}

/******************* (C) COPYRIGHT 2009 SENLi *****END OF FILE****/


