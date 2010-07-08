#ifndef _DISPLAY_H
#define _DISPLAY_H

//define pins for HT1621B 
#define LCD_CeH		P3_5 = 1
#define LCD_WrH		P3_7 = 1
#define LCD_DaH		P3_6 = 1

#define LCD_CeL		P3_5 = 0
#define LCD_WrL		P3_7 = 0
#define LCD_DaL		P3_6 = 0

//command code 
#define BIAS  0X52  //偏压
#define SYSTEM 0X02  //打开系统振荡器
#define SYSDIS 0X00 
#define LCDON 0X06

typedef enum	_DISPLAY_STATE
{
//	DISPLAY_STATE_IDLE = 0,			//do nothing	
	DISPLAY_AUX = 0,			
	DISPLAY_CLOCK,		//show time
	DISPLAY_RADIO,		//show radio information
	DISPLAY_AUDIO		//show audio set  information

} DISPLAY_STATE;

 
extern UCHAR CODE Num[10];
extern UCHAR DATA DispBuff[12];
extern bit DispRefresh;



/*******************************************
                   初始化HT1621 函数
函数原型:void DisplayInit()
功能:实现HT1621的初始化                

********************************************/
void DisplayInit(void);
void DisplayMain(void); 
#endif 
