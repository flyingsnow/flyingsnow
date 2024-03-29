#ifndef __AUDIO_H
#define __AUDIO_H

#define VOL_MAX  31		//音量上限 
#define BASS_MAX    14		//低音上限 
#define TREB_MAX    14		//高音上限 
#define BAL_MAX     14		//左右平衡上限 
#define FAD_MAX     10		//前后平衡上限 
#define LOUD_MAX    1		//响度上限 

#define VOL_EQ		0
#define BAS_EQ		1
#define TRE_EQ		2
#define BAL_EQ		3
#define FAD_EQ		4

#define MAXVOL		0
#define DEFVOL		10	
#define MINVOL		63


#define Channel_Aux 	0x42
#define Channel_Radio	0x40

#define MUTE_AMP()			P3_4 = 0
#define UnMUTE_AMP()		P3_4 = 1

extern UCHAR  EQ_Item;
extern signed char CurrentVol;
extern signed char SetBASS;
extern signed char SetTREB;
extern signed char SetBAL;
extern bit isMute;
extern UCHAR VolDispTimer;


void SC7313_initial(UCHAR channel);
void SC7313_Driver(UCHAR temp);
void AudioMain(void);
//void VolTimer(void);
void EQ_Display(void);



#endif 
