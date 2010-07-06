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
#define BIAS  0X52  //ƫѹ
#define SYSTEM 0X02  //��ϵͳ����
#define SYSDIS 0X00 
#define LCDON 0X06


 
extern UCHAR CODE Num[10];
extern UCHAR DispBuff[12];



/*******************************************
                   ��ʼ��HT1621 ����
����ԭ��:void DisplayInit()
����:ʵ��HT1621�ĳ�ʼ��                

********************************************/
void DisplayInit(void);

void dispFrequency(enum BAND band,UINT freq);
void DisplayMain(void); 
#endif 
