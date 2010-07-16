#include "main.h"


UCHAR singlestepTimer;				//keep singlesetep tuner state 5s
UCHAR ScanTimer;					//preset station scan conter 10s
UCHAR PresetFlag = FALSE;			//preset station number
enum BAND SaveBand = Band_FM1;		
enum BAND CurrentBand = Band_FM1;
UINT SaveFreq = Frequency_Default;
UINT CurrentFreq = Frequency_Default;
enum SEEKSTATE DATA SeekState = Seek_Configure;
enum STATUS_RADIO DATA TunerStatus = Status_Idle;		

bit direction,ast,singlestep,TunerMuteFlag;


static UINT LastBandFreq[5] = {
	MW_Threshold_Freq,MW_Threshold_Freq, FM_Threshold_Freq,FM_Threshold_Freq,FM_Threshold_Freq};

UINT  Preset_Freq[5][PresetNum] =  {
	{603,999,1404,594,990,1395},
	{603,999,1404,594,990,1395},
	{9030,9810,10610,9010,9800,10600},
	{9030,9810,10610,9010,9800,10600},
	{9030,9810,10610,9010,9800,10600}
};


/*---------------------------------------------
Function Name:	Tuner_Init
Input:			Band,frequency
Output:			
Description:		Tuner initialization
----------------------------------------------*/
void Tuner_Init(enum BAND band,UINT freq)
{	
//	UCHAR  CODE Si4730_Property_Rssi[6] = {0x12,0x00,0x14,0x04,0x00,0x0E};	
	switch(band) {
	case Band_FM1:
	case Band_FM2:
	case Band_FM3:	
		Si4730_Power_Up(FM_RECEIVER);
//			Si4730_Set_Property(Si4730_Property_Rssi);			
		break;
	case Band_MW1:
	case Band_MW2:			
		Si4730_Power_Up(AM_RECEIVER);
		break;
	}
 	
	Si4730_HardMute(TRUE);
	Si4730_Tune_Freq(band,freq);
	Si4730_HardMute(FALSE);
}





/*---------------------------------------------------------------------
Function name:	Tuner_BandSwitch
Input:			N/A
Output:			N/A
Description:	Switch bands among FM,MW, by means of rotation.
---------------------------------------------------------------------*/
void Tuner_BandSwitch(void)
{
	enum BAND band;
	
//	MUTE_AMP();
	Si4730_HardMute(TRUE);
	TunerMuteFlag = TRUE;
	Si4730_Power_Down();
#if 1	
	LastBandFreq[CurrentBand] = CurrentFreq;
	if (CurrentBand == Band_FM3)
		band = Band_MW1;
	else
		band = CurrentBand + 1;
	
	Tuner_Init(band, LastBandFreq[band]);
	
#else 
	if(CurrentBand > 1) {
		LastBandFreq[1] = CurrentFreq;
		band = Band_MW1;
		Tuner_Init(band, LastBandFreq[0]); 
		CurrentFreq = LastBandFreq[0];	

	}
	else {
		LastBandFreq[0] = CurrentFreq;
		band = Band_FM1;
		Tuner_Init(band, LastBandFreq[1]); 
		CurrentFreq = LastBandFreq[1];	

	}
	
#endif
	CurrentBand = band;
	CurrentFreq = LastBandFreq[band];
	SeekState = Seek_Configure;
	Si4730_HardMute(FALSE);		
	TunerMuteFlag = 0;
	PresetFlag = FALSE;

			
}





/*---------------------------------------------------------------------
Function name:	Ftun_TunetoPreset
Input:			preset
Output:			N/A
Description:	Tune to preset station
---------------------------------------------------------------------*/

void Tuner_TunetoPreset(enum PRESET preset)
{
#if 0
	switch(CurrentBand) 			//setting up of tuning step and edge limit subject to different bands
	{
	case Band_MW1:
	case Band_MW2:	
		CurrentFreq = Preset_Freq[0][preset];
					break;
	case Band_FM1:
	case Band_FM2:
	case Band_FM3:	
		CurrentFreq = Preset_Freq[1][preset];
					break;	
	}
#else	

	CurrentFreq = Preset_Freq[CurrentBand][preset];
#endif
	SaveBand = CurrentBand;						//Sep 24,2009 recover radio TunerStatus when ACC ON
	SaveFreq = CurrentFreq;
	
	Si4730_Tune_Freq(CurrentBand,CurrentFreq);
	
 	PresetFlag = (UCHAR)(preset);
}



/*---------------------------------------------------------------------
Function name:	Ftun_IsPreset
Input:			frequency
Output:			preset station
Description:	Judge if preset station or not
---------------------------------------------------------------------*/

enum PRESET Tuner_IsPreset (void)
{	
	UCHAR i;

	for (i=0;i<PresetNum;i++)	//query if it's a preset station
		if (SaveFreq == Preset_Freq[CurrentBand][i])
			return (i);
}


/*---------------------------------------------------------------------
Function name:	Tuner_SaveCurrentStation
Input:			frequency,preset
Output:			N/A
Description:	Save current frequency to preset station
---------------------------------------------------------------------*/
void Tuner_SaveCurrentStation(enum PRESET preset)
{	
	Preset_Freq[CurrentBand][preset] = CurrentFreq;
	Tuner_TunetoPreset(preset);
	PresetFlag = preset;
}


 
/*---------------------------------------------------------------------
Function name:	Tuner_Seek
Input:			direction,auto_store,single_step
Output:			N/A
Description:	Tuning action including single step tune, seek, 
				auto search and store
---------------------------------------------------------------------*/
//void Tuner_Seek( enum DIRECTION direction, UCHAR ast, UCHAR singlestep)
void Tuner_Seek(bit direction, bit ast, bit singlestep)

{	
	
	UINT Rssi;	
	UCHAR j;
	
	static UCHAR i=0;						//Total numbers of preset stations
	static UINT step,max,min;
	static UINT freq;
	static UINT stepnum;					//Step numbers going on of seeking action
	static UCHAR preset_level[6]; 			//stores level, 
											
	
	switch (SeekState) {
		
		case Seek_Idle:
			Si4730_Tune_Freq(CurrentBand,CurrentFreq);
			stepnum = 0;
			if(TunerStatus != Status_Single) {
				TunerStatus = Status_Idle;
				SeekState = Seek_Configure;
			}
			break;

		case Seek_Configure:
			switch(CurrentBand)				//setting up of tuning step and edge limit subject to different bands
			{
			case Band_MW1:
			case Band_MW2:	
							step = MWSeekStep;
							max = MWmax;
							min = MWmin;
							break;
			case Band_FM1:
			case Band_FM2:
			case Band_FM3:	step = FMSeekStep;
							max = FMmax;
							min = FMmin;
							break;

			}
#if AST_ON	
			if (ast == TRUE) {
				for(j=0; j<6; j++)							//Clear preset stations buffer
					preset_level[j] = 0;
			}
			stepnum = 0;
			i = 0;
			j = 0;
#endif 
			SeekState = Seek_Request;
			if (singlestep == TRUE)								//Single step tuning up or down
				TunerStatus = Status_Single;
			else
				if (ast == TRUE)								//Auto search and store
					TunerStatus = Status_AST;
				else											//Seek up or down
					TunerStatus = Status_Seek;	
			break;
			
		case Seek_Request:
			if (direction == UP)
				freq=CurrentFreq+step;
			else
				freq=CurrentFreq-step; 
			if (freq > max)
				freq= min;
			if (freq < min)
				freq = max;
			
			if (ast == TRUE)						//Auto search and store
				stepnum++;							//for AMS
				
			SeekState = Seek_Change_Freqency;	//Sep 15 09, chang to Seek_Request ,do clcye seek  
			break;
				
		case Seek_Change_Freqency:
			Si4730_Tune_Freq(CurrentBand,freq);		
			
			CurrentFreq = freq;	
			
			if (singlestep == FALSE)							//Seek action, check RSQ TunerStatus VALID
				SeekState = Seek_Check_Valid;
			else												//Single step tuning action, turn to Idle
			{	
				SeekState = Seek_Idle;
			}
			break;

		case Seek_Check_Valid:			
			if(Si4730_RSQ_Status(CurrentBand,Read_VALID))
				{
					if(ast == TRUE)
						SeekState = Seek_AMS;
					else
						SeekState = Seek_Idle;
				}
			else
				SeekState = Seek_Request;
			break;
			
		case Seek_AMS:
			if(stepnum <= (max-min)/step)
			{			
				Rssi = Si4730_RSQ_Status(CurrentBand,Read_RSSI);
				if(i > 6) {
					for(j = 0; j < PresetNum; j++) {
						if(Rssi > preset_level[j]) {
							preset_level[j] = Rssi;
							Preset_Freq[CurrentBand][j] = CurrentFreq;
							break;
						}
					}
				}
				else {
					Preset_Freq[CurrentBand][i] = CurrentFreq;										
				}
				i++;
				SeekState = Seek_Request;
			}
			else
			{													
				Tuner_TunetoPreset(PRESET1);
				SeekState = Seek_Configure;				
				TunerStatus =  Status_Idle;
			}
			break;

		default:
			break;
	}
}


void TunerMain(void)
{
	if(gKeyEvent != IN_KEY_NONE) {
		switch(gKeyEvent) {

		case IN_KEY_NEXT_SP:
			if(ClockState == TRUE) break;	//setting clock
			if(TunerStatus == Status_Single) {
				direction = TRUE;
				SeekState = Seek_Request;			
				singlestepTimer = 10;	//5s
			}			
		  	else if(TunerStatus == Status_Seek && direction == TRUE) {
				TunerStatus = Status_Idle;
				}
				else {
					direction = TRUE;
					TunerStatus = Status_Seek;
					if(TunerMuteFlag == FALSE)	{		
						Si4730_HardMute(TRUE);
						TunerMuteFlag = TRUE;
					}
					isMute = FALSE;
				}
			SeekState = Seek_Configure;
			break;

		case IN_KEY_NEXT_CP:
			if(ClockState == TRUE) break;	//setting clock
			direction = TRUE;
			TunerStatus = Status_Single;
			SeekState = Seek_Configure; 
			singlestepTimer = 10;	//5s
			if(TunerMuteFlag == FALSE)	{		
				Si4730_HardMute(TRUE);
				TunerMuteFlag = TRUE;
			}
			isMute = FALSE;
			break;
			
		case IN_KEY_NEXT_CPC:
			if(ClockState == TRUE) break;	//setting clock
			direction = TRUE;
			TunerStatus = Status_Single;
			SeekState = Seek_Request; 			
			singlestepTimer = 10;	//5s
			break;
			
		case IN_KEY_NEXT_CPR:
			if(ClockState == TRUE) break;	//setting clock
			SeekState = Seek_Idle; 			
			singlestepTimer = 10;	//5s
			break;

		case IN_KEY_PRE_SP:
			if(ClockState == TRUE) break;	//setting clock
			if(TunerStatus == Status_Single) {
				direction = FALSE;
				SeekState = Seek_Request;			
				singlestepTimer = 10;	//5s			
			}
			else if(TunerStatus == Status_Seek && direction == FALSE) {
					TunerStatus = Status_Idle;
				}
				else {
					direction = FALSE;
					if(TunerMuteFlag == FALSE) {		
						Si4730_HardMute(TRUE);
						TunerMuteFlag = TRUE;
					}
					isMute = FALSE;
					TunerStatus = Status_Seek;
				}
			SeekState = Seek_Configure; 			
			break;

		case IN_KEY_PRE_CP:
			if(ClockState == TRUE) break;	//setting clock
			direction = FALSE;
			TunerStatus = Status_Single;
			singlestepTimer = 10;
			if(TunerMuteFlag == FALSE) {		
				Si4730_HardMute(TRUE);
				TunerMuteFlag = TRUE;
			}
			isMute = FALSE;
			SeekState = Seek_Configure; 			
			break;

		case IN_KEY_PRE_CPC:			
			if(ClockState == TRUE) break;	//setting clock
			direction = FALSE;
			TunerStatus = Status_Single;
			SeekState = Seek_Request; 			
			singlestepTimer = 10;	//5s
			break;

		case IN_KEY_PRE_CPR:
			if(ClockState == TRUE) break;	//setting clock
			SeekState = Seek_Idle;			
			singlestepTimer = 10;	//5s
			break;

		case IN_KEY_BAND_SP:
			
			SC7313_Driver(0x43);		// kill pop ...
			
			Tuner_BandSwitch();
			TunerStatus = Status_Idle;
			SeekState = Seek_Configure;
			isMute = FALSE;
			
			SC7313_Driver(Channel_Radio);
			break;
			
		case IN_KEY_AST_SP:
			if(TunerStatus != Status_Scan) {
				ScanTimer = 0;
				PresetFlag = PRESET6;			
				TunerStatus = Status_Scan;
			}
			else {
				TunerStatus = Status_Idle;
			}
			SeekState = Seek_Configure;
			break;

		case IN_KEY_AST_CP:
			direction = TRUE;
			TunerStatus = Status_AST;
			SeekState = Seek_Configure; 
			if(TunerMuteFlag == FALSE)	{		
				Si4730_HardMute(TRUE);
				TunerMuteFlag = TRUE;
			}
			isMute = FALSE;
			break;

		case IN_KEY_P1_SP:
		case IN_KEY_P2_SP:
		case IN_KEY_P3_SP:
		case IN_KEY_P4_SP:
		case IN_KEY_P5_SP:
		case IN_KEY_P6_SP:		
			TunerStatus = Status_Idle;
			SeekState = Seek_Configure; 	
			Tuner_TunetoPreset(gKeyEvent - 0xA1);
			isMute = FALSE;
			break;

		case IN_KEY_P1_CP:
		case IN_KEY_P2_CP:
		case IN_KEY_P3_CP:
		case IN_KEY_P4_CP:
		case IN_KEY_P5_CP:
		case IN_KEY_P6_CP:
			TunerStatus = Status_Idle;
			SeekState = Seek_Configure; 	
			Tuner_SaveCurrentStation(gKeyEvent - 0xA6);
			isMute = FALSE;
			break;
		}
	}

	switch(TunerStatus) {
	case Status_Idle:		
		if(TunerMuteFlag == TRUE) {		
			Si4730_HardMute(FALSE);
			TunerMuteFlag = FALSE;
		}
		break;

	case Status_Seek:
		Tuner_Seek(direction,FALSE,FALSE);
		break;

	case Status_Single:
		if(singlestepTimer == 0)
			TunerStatus = Status_Idle;
		Tuner_Seek(direction,FALSE,TRUE);

		break;

	case Status_AST:		
		Tuner_Seek(direction,TRUE,FALSE);
		break;

	case Status_Scan:
		if(ScanTimer == 0) {
			if(PresetFlag == PRESET6)
				PresetFlag = PRESET1;
			else 
				PresetFlag++;
			Tuner_TunetoPreset(PresetFlag);
			ScanTimer = 20;
		}
		
		break;
		
	default:break;
	}
}
