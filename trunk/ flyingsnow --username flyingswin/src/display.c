#include "main.h"
#include "string.h"
//UCHAR DispRefresh;
UCHAR XDATA Num[10] = {0xFA,0x60,0xBC,0xF4,0x66,0xD6,0xDE,0x70,0xFE,0xF6};
UCHAR DispBuff[12];


VOID Start(VOID)
{
	LCD_CeL;
	LCD_WrH;
	_nop_();
	LCD_WrL;
	LCD_CeL;
	_nop_();
}

VOID SendBit(UCHAR Data,UCHAR cnt)    //Data 的高 cnt 位写入 HT1621，高位在前 
{ 
 UCHAR i; 
 UCHAR d,n;
 d = Data;
 n= cnt;
 
  for(i =0; i <n; i ++) { 
  	 if((d&0x80)==0) LCD_DaL; 
  	 else LCD_DaH; 
  	 LCD_WrL; 
  	 _nop_(); _nop_();_nop_();_nop_();
  	 LCD_WrH; 
  	 d<<=1; 
 } 
} 

VOID SendDataBit(UCHAR Data ,UCHAR cnt)
{
	UCHAR i;
	for(i = 0; i < cnt; i++) {
		if ((Data & 0x01) == 0)
		LCD_DaL;
		else LCD_DaH;
		LCD_WrL;
		_nop_(); _nop_();_nop_();_nop_();
		LCD_WrH;
		Data >>= 1;
	}
}

VOID SendCmd(UCHAR cmd)
{
	Start();
	SendBit(0x80,4);	//命令模式  Modify 07-09 SendBit(0x80,3);
	SendBit(cmd,8);	//写入命令				     SendDataBit(cmd,9);
	LCD_CeH;
}

VOID Write_LCD(UCHAR addr, UCHAR Data)
{
	Start();
	SendBit(0xa0,3);  //写入标志码"101" 
 	SendBit(addr<<2,6); //写入 6 位 addr 
 	SendDataBit(Data,4); //写入 Data 的低 4 位 
	LCD_CeH;
}

VOID Write_string(UCHAR addr,UCHAR *Data,UCHAR num)
{
	UCHAR i; 
	Start(); 
 	SendBit(0xa0,3);  //写入标志码"101" 
 	SendBit(addr<<2,6); //写入 6 位 addr 
    for(i =0; i <num; i ++,Data++)  //连续写入数据 
    { 
  		SendDataBit(*Data,8); 
 	} 
 	LCD_CeH; 


}








/*---------------------------------------------------------------------
Function name:	dispFrequency
Input:			band,frequency
Output:			N/A
Description:	displya  current frequency 
---------------------------------------------------------------------*/

void dispFrequency(enum BAND band,UINT freq)
{


	UCHAR i = 10;
	UCHAR num1,num3,num2;//,num4;

	while(i-- ) {
		DispBuff[i+1]=0x00;	
	}
	
	DispBuff[11] &= (UCHAR)~0x08;
	
	switch (band) {
		case Band_MW1:
		case Band_MW2:				
				//Radio Display
	//			while(i-- ) DispBuff[i+1]=0x00;
				DispBuff[1] = 0x26;				//display MW
				DispBuff[2] = 0x6B;
				DispBuff[3] = 0x9C;

				DispBuff[4] = Num[1];
				if(band == Band_MW2)
					DispBuff[4] = Num[2];
				
				num3 = (UCHAR)(freq /100) %10 ;
				num2 = (UCHAR)((freq /10 ) % 10);
				num1 = (UCHAR)(freq % 10);
				
				freq > 1000?(DispBuff[5] = Num[1]): (DispBuff[5] = 0x00);
				DispBuff[6] = Num[num3];
				DispBuff[7] = Num[num2];
				DispBuff[8] = Num[num1];
				DispBuff[9]=0x00;
				DispBuff[0]= 0x00; 				
				break;
		default:
				DispBuff[1] = 0x16;		//display FM	
				DispBuff[2] = 0x64;
				DispBuff[3] = 0xB2;
				
				DispBuff[4] = Num[1];
				if(band == Band_FM2)
					DispBuff[4] = Num[2];
				if(band == Band_FM3)
					DispBuff[4] = Num[3];
				//Radio Display
				num3 = (UCHAR)((freq /1000 ) % 10);
				num2 = (UCHAR)((freq /100 ) % 10);
				num1 = (UCHAR)((freq /10 ) % 10);
				freq >= 10000?(DispBuff[5] = Num[1]): (DispBuff[5] = 0x00);
				DispBuff[6] = Num[num3];
				DispBuff[7] = Num[num2];
				DispBuff[8] = Num[num1];
				DispBuff[0]= 0x40; 							
				break;
	}

}


/*******************************************
                   初始化HT1621 函数
函数原型:VOID DisplayInit()
功能:实现HT1621的初始化                

********************************************/
VOID DisplayInit()
{	

	UCHAR i;

	
	SendCmd(BIAS);
    SendCmd(SYSTEM);
	SendCmd(LCDON);
	//for test 
	for(i = 8;i <32 ;i++)
		Write_LCD(i,0x0);
	//	DispBuff[i] = 0x00;
	memset(DispBuff,0x00,12);
}


VOID DisplayMain(VOID)
{	
	dispFrequency(SaveBand,SaveFreq);
	Write_string(8,DispBuff,12);
	
}


