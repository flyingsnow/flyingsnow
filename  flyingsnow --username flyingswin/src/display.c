#include "main.h"
//#include "string.h"
//UCHAR DispRefresh;

UCHAR CODE EQ_Buff[25] = { 0x8A,0xfA,0x24,0x60,0x00,	//VOL
				 0x00,0xd6,0xd1,0x6D,0xd0,  //BAS
				 0x00,0x9E,0x59,0x60,0x50,  //TRE
				 0x00,0x8A,0xD1,0x6D,0xd0,	//BAL
				 0x00,0xeC,0xd1,0x64,0x16   //FAD
				};

UCHAR CODE VOLUM[7] = {0x80,0xc4,0xc6,0xe6,0xe7,0xf7,0xf7};
UCHAR CODE Num[10] = {0xFA,0x60,0xBC,0xF4,0x66,0xD6,0xDE,0x70,0xFE,0xF6};
UCHAR DATA DispBuff[12];

bit DispRefresh;


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

VOID DispFreq(VOID)
{
	UCHAR num1,num3,num2;//,num4;
	
	
	switch (SaveBand) {
		case Band_MW1:
		case Band_MW2:				
				//Radio Display
	//			while(i-- ) DispBuff[i+1]=0x00;
				DispBuff[1] = 0x26;				//display MW
				DispBuff[2] = 0x6B;
				DispBuff[3] = 0x9C;

				DispBuff[4] = Num[1];
				if(SaveBand== Band_MW2)
					DispBuff[4] = Num[2];
				
				num3 = (UCHAR)(SaveFreq/100) %10 ;
				num2 = (UCHAR)((SaveFreq/10 ) % 10);
				num1 = (UCHAR)(SaveFreq% 10);
				
				SaveFreq > 1000?(DispBuff[5] = Num[1]): (DispBuff[5] = 0x00);
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
				if(SaveBand== Band_FM2)
					DispBuff[4] = Num[2];
				if(SaveBand== Band_FM3)
					DispBuff[4] = Num[3];
				//Radio Display
				num3 = (UCHAR)((SaveFreq/1000 ) % 10);
				num2 = (UCHAR)((SaveFreq/100 ) % 10);
				num1 = (UCHAR)((SaveFreq/10 ) % 10);
				SaveFreq >= 10000?(DispBuff[5] = Num[1]): (DispBuff[5] = 0x00);
				DispBuff[6] = Num[num3];
				DispBuff[7] = Num[num2];
				DispBuff[8] = Num[num1];
				DispBuff[0]= 0x40; 							
				break;
	}

}

VOID DispClk(VOID)
{	
	UCHAR h1,h2;
	UCHAR m1,m2;	
		
 	h1 = t_hour/10;
	h2 = t_hour%10;
  	m1 = t_min /10;
  	m2 = t_min %10;
	
	DispBuff[9] = Num[m2];   
    DispBuff[8] = Num[m1];
	
    DispBuff[7] = Num[h2];		
    (h1 == 0) ?(DispBuff[6] &= 0x10):(DispBuff[6] |= Num[h1]);   
	
    if(t_halfsec%2) 
		DispBuff[0] = 0x00;
	else		
		DispBuff[0] = 0x20;
#if 0		

//	if(System.DispMode.Current == DISPMODE_CCLOCK && t_100ms > 4 ) {
		if( ClockStatus == CLK_CHour && t_100ms > 4 ) {
			DispBuff[7] = 0x00;   
			DispBuff[6] = 0x00;			
			DispBuff[0] = 0x20;
		}
		if(ClockStatus == CLK_CMin&& t_100ms > 4 ) {
			DispBuff[9] = 0x00;   
			DispBuff[8] = 0x00;
			DispBuff[0] = 0x20;
		}
//		DispBuff[11] |= 0x02;
//	}
   
   DispRefresh = 1;
#endif
}


//Volum indication 
static VOID DispVolum(VOID)
{
	if(isMute == 1) {
		DispBuff[11] &= 0x80;
		DispBuff[5] |= 0x10; 					
	}	
	else {
		DispBuff[11] |= VOLUM[CurrentVol/5];
		DispBuff[5] &= 0xEF;		 
	}
}


VOID DispEQ(VOID)
{	
	UCHAR i,j;
	
	i = 10;
	j = EQ_Item * 5;
	
	switch (EQ_Item) {
		
	case VOL_EQ:
//	DispBuff[11] = VOLUM[CurrentVol/5];
	
		DispBuff[9] = 0x00;		
		DispBuff[8] = Num[CurrentVol%10]; 
		(CurrentVol < 10 )? (DispBuff[7]=0x00):(DispBuff[7]= Num[CurrentVol/10]);	
		
		for(i = 0; i < 5; i++)
			DispBuff[5-i] = EQ_Buff[j+i];
	
		break;

	case BAS_EQ:
		if (SetBASS >= 7 ) {
			DispBuff[8] = Num[SetBASS - 7];
			DispBuff[7] = 0x00;
		}
		else {					
			DispBuff[8] = Num[7 - SetBASS];
			DispBuff[7] = 0x04;
		}
		for(i = 0; i < 5; i++)
			DispBuff[5-i] = EQ_Buff[j+i];
		break;

	case TRE_EQ:
		if (SetTREB >= 7 ) {
			DispBuff[8] = Num[SetTREB - 7];
			DispBuff[7] = 0x00;
		}
		else {
			
			DispBuff[8] = Num[7 - SetTREB];
			DispBuff[7] = 0x04;
		}
		for(i = 0; i < 5; i++)
			DispBuff[5-i] = EQ_Buff[j+i];
	
		break;
		
	case BAL_EQ:
		
		if (SetBAL >= 9 ) {
			DispBuff[9] = Num[SetBAL - 9];
			DispBuff[8] = 0x04;
			DispBuff[7] = 0x04;	
			if (SetBAL == 9) {
				DispBuff[9] = 0x04;
				DispBuff[8] = Num[SetBAL - 9];
			}					
		}
		else {					
			DispBuff[9] = 0x04;
			DispBuff[8] = 0x04;
			DispBuff[7] = Num[9 - SetBAL];
		}
		
		for(i = 0; i < 5; i++)
			DispBuff[5-i] = EQ_Buff[j+i];
	
		break;
//	case FAD_EQ:
//		break;
	
	default:
		break;
		
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
//	memset(DispBuff,0x00,12);
}


VOID DisplayMain(VOID)
{	
	UCHAR i = 11;
	
	if(DispRefresh) { 
		
		while(i-- ) {
			DispBuff[i]=0x00; 
		}
		
		switch(System.DispMode) {
		case DISPLAY_CLOCK:
			DispClk();
			break;

		case DISPLAY_RADIO:
			DispFreq();
			if(TunerStatus == Status_Idle) {
				//disp stereo bit	
			}
			break;

		case DISPLAY_AUX:
			DispBuff[5]=0x7E;		//A
			DispBuff[6]=0xEA;		//U 
			break;

		case DISPLAY_AUDIO:
			DispEQ();
			break;
		}
		if(System.PowerMode == POWERMODE_POWERON)
			DispVolum();
		
		Write_string(8,DispBuff,12);

		DispRefresh = 0;
	}
}
