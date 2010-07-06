#ifndef __AUDIO_H
#define __AUDIO_H

#define VOLUME_MAX  31		//�������� 
#define BASS_MAX    14		//�������� 
#define TREB_MAX    14		//�������� 
#define BAL_MAX     14		//����ƽ������ 
#define FAD_MAX     10		//ǰ��ƽ������ 
#define LOUD_MAX    1		//������� 

#define VOL_EQ		0
#define BAS_EQ		1
#define TRE_EQ		2
#define BAL_EQ		3
#define FAD_EQ		4

#define DEFVOL		16	
#define MINVOL		63


#define Channel_Aux 	0x42
#define Channel_Radio	0x40

#define MUTE_AMP()			P3_4 = 0
#define UnMUTE_AMP()		P3_4 = 1

void SC7313_initial(UCHAR channel);
//void SC7313_Driver(UCHAR temp);
void AudioMain(void);
//void VolTimer(void);
void EQ_Display(void);



#endif 
