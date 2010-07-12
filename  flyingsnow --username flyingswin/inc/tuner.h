#ifndef __TUNER_H
#define __TUNER_H


/*----------------------------------------
define for tuning step,frequency range,
Seek level,ifcounter for all bands.
-----------------------------------------*/
#define FMTuneStep		10
#define FMSeekStep		10
#define FMmin			8750  //09-08-12 modify ..
#define FMmax			10800


#define MWTuneStep		9
#define MWSeekStep		9
#define MWmin			531
#define MWmax			1602

/*--------------------------------------------------
define for initialization of frequency, band  
---------------------------------------------------*/
#define FM_Threshold_Freq	FMmin
#define Band_Default 		Band_FM1
#define Frequency_Default	FM_Threshold_Freq
#define MW_Threshold_Freq	MWmin
 
#define PresetNum			6
	
#define AST_ON 				1
/*---------------------------------------------------
Public enum type definition
----------------------------------------------------*/
enum BAND {Band_MW1,Band_MW2,Band_FM1,Band_FM2,Band_FM3};
enum DIRECTION {DOWN,UP};				//seek action direction 
enum PRESET {PRESET1,PRESET2,PRESET3,PRESET4,PRESET5,PRESET6,NOPRESET};
/*---------------------------------------------------------------------------------------------------
SEEKSTATE is the status while controlling the whole process during the seek action
---------------------------------------------------------------------------------------------------*/
enum SEEKSTATE {Seek_Configure,Seek_Idle,Seek_Request,	
				Seek_Change_Freqency,Seek_Check_Valid,Seek_AMS};

enum STATUS_RADIO{Status_Idle, Status_Seek, Status_AST,Status_Single,Status_Scan};



extern UINT SaveFreq;
extern enum BAND SaveBand;

extern enum STATUS_RADIO DATA TunerStatus;		

/*-----------------------------------------------
Function declaration
------------------------------------------------*/
void Tuner_Init(enum BAND band,UINT freq);
//void Tuner_Seek(enum DIRECTION direction, UCHAR ast, UCHAR singlestep);
void Tuner_BandSwitch(void);
void Tuner_SaveCurrentStation(enum PRESET preset);
void Tuner_TunetoPreset(enum PRESET preset);
void TunerMain(void);
enum PRESET Tuner_IsPreset (void);

void Tuner_Seek(bit direction, bit ast, bit singlestep);

#endif 
