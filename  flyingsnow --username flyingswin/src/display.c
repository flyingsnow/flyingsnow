#include "main.h"

UCHAR DispRefresh;
UCHAR Num[10] = {0xFA,0x60,0xBC,0xF4,0x66,0xD6,0xDE,0x70,0xFE,0xF6};
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

VOID SendBit(UCHAR Data,UCHAR cnt)    //Data �ĸ� cnt λд�� HT1621����λ��ǰ 
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
	SendBit(0x80,4);	//����ģʽ  Modify 07-09 SendBit(0x80,3);
	SendBit(cmd,8);	//д������				     SendDataBit(cmd,9);
	LCD_CeH;
}

VOID Write_LCD(UCHAR addr, UCHAR Data)
{
	Start();
	SendBit(0xa0,3);  //д���־��"101" 
 	SendBit(addr<<2,6); //д�� 6 λ addr 
 	SendDataBit(Data,4); //д�� Data �ĵ� 4 λ 
	LCD_CeH;
}

VOID Write_string(UCHAR addr,UCHAR *Data,UCHAR num)
{
	UCHAR i; 
	Start(); 
 	SendBit(0xa0,3);  //д���־��"101" 
 	SendBit(addr<<2,6); //д�� 6 λ addr 
    for(i =0; i <num; i ++,Data++)  //����д������ 
    { 
  		SendDataBit(*Data,8); 
 	} 
 	LCD_CeH; 


}

/*******************************************
                   ��ʼ��HT1621 ����
����ԭ��:VOID DisplayInit()
����:ʵ��HT1621�ĳ�ʼ��                

********************************************/
VOID DisplayInit()
{	

	UCHAR i;

	
	SendCmd(BIAS);
    SendCmd(SYSTEM);
	SendCmd(LCDON);
	//for test 
	for(i = 8;i <20 ;i++)
		Write_LCD(i,0x0);
}


VOID DisplayMain(VOID)
{	
	
	Write_string(8,DispBuff,12);
	
}


