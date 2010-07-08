#include "main.h"

enum BAND CurrentBand = Band_FM1;
enum BAND SaveBand = Band_FM1;

UINT CurrentFreq = Frequency_Default;
UINT SaveFreq = Frequency_Default;

enum SEEKSTATE DATA SeekState = Seek_Configure;

enum STATUS_RADIO DATA TunerStatus = Status_Idle;		

bit direction,ast,singlestep,TunerMuteFlag;


static UINT LastBandFreq[2] = {
	MW_Threshold_Freq, FM_Threshold_Freq};

UINT  Preset_Freq[2][PresetNum] =  {
	{603,999,1404,594,990,1395},
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
 
	Si4730_Tune_Freq(band,freq);
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
	TunerMuteFlag = 1;
	LastBandFreq[CurrentBand] = CurrentFreq;
#if 0	
	if (CurrentBand == Band_FM3)
		band = Band_MW1;
	else
		band = CurrentBand + 1;
#endif 
	if(CurrentBand > 1) 
		band = Band_MW1;
	else
		band = Band_FM1;
	

	Tuner_Init(band, LastBandFreq[band]);

	CurrentBand = band;
	CurrentFreq = LastBandFreq[band];	
	SaveBand = CurrentBand;
	SaveFreq = CurrentFreq; 
	Si4730_HardMute(FALSE);		
	TunerMuteFlag = 0;
//	UnMUTE_AMP();
			
}





/*---------------------------------------------------------------------
Function name:	Ftun_TunetoPreset
Input:			preset
Output:			N/A
Description:	Tune to preset station
---------------------------------------------------------------------*/

void Tuner_TunetoPreset(enum PRESET preset)
{
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


	SaveBand = CurrentBand;						//Sep 24,2009 recover radio TunerStatus when ACC ON
	SaveFreq = CurrentFreq;
	
	Si4730_Tune_Freq(CurrentBand,CurrentFreq);
//	UnMUTE_AMP();			//Oct 8,2009 ,when Auto seek end ,the 7377 is in mute TunerStatus
 //	PresetFlag = (u8)(preset);
}

#if 1

 
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
	UCHAR j=0;
	
	static UCHAR i=0;						//Total numbers of preset stations
	static UINT step,max,min;
	static UINT freq;
	static UINT stepnum;					//Step numbers going on of seeking action
	static UCHAR preset_level[6]; 			//stores level, 
											
	
	switch (SeekState) {
		
		case Seek_Idle:
			Si4730_Tune_Freq(CurrentBand,CurrentFreq);
//			if (TunerStatus != Status_Single) {
//				UnMUTE_AMP();					//exit mute
//			}
			stepnum = 0;
			TunerStatus = Status_Idle;
			SeekState = Seek_Configure;
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
			while(preset_level[j])							//Clear preset stations buffer
				preset_level[j++] = 0;
			//PresetFlag = FALSE;
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
			//save the frequency infomation 
			//	SaveBand = band;
			//	SaveFreq = freq; 
			if (singlestep == FALSE)							//Seek action, check RSQ TunerStatus VALID
				SeekState = Seek_Check_Valid;
			else												//Single step tuning action, turn to Idle
			{	
				SeekState = Seek_Request;
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
				i++;
				if(i > 6) {
					for(j = 0; j < PresetNum; j++){
						if(Rssi > preset_level[j])
							preset_level[j] = Rssi;
						    if(CurrentBand == Band_MW1 || CurrentBand == Band_MW2)
								Preset_Freq[0][j] = CurrentFreq;					
							else 
								Preset_Freq[1][j] = CurrentFreq;
					}
				}				
				SeekState = Seek_Request;
			//	SeekState = Seek_Idle;
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

#endif 


void TunerMain(void)
{
	if(gKeyEvent != IN_KEY_NONE) {
		switch(gKeyEvent) {

		case IN_KEY_NEXT_SP:
			direction = 1;
			ast = 0;
			singlestep = 0;
			TunerStatus = Status_Seek;
			SeekState = Seek_Configure;
			break;

		case IN_KEY_NEXT_CP:
			direction = 1;
			ast = 0;
			singlestep = 1;
			TunerStatus = Status_Single;
			SeekState = Seek_Configure; 			

			break;

		case IN_KEY_PRE_SP:
			direction = 0;
			ast = 0;
			singlestep = 0;
			TunerStatus = Status_Seek;
			SeekState = Seek_Configure; 			
			break;

		case IN_KEY_PRE_CP:
			direction = 0;
			ast = 0;
			singlestep = 1;
			TunerStatus = Status_Single;
			SeekState = Seek_Configure; 			

			break;

		case IN_KEY_BAND_SP:		
			Tuner_BandSwitch();
			TunerStatus = Status_Idle;
			SeekState = Seek_Configure; 			
			break;

		case IN_KEY_AST_SP:

			break;

		case IN_KEY_AST_CP:
			direction = 1;
			ast = 1;
			singlestep = 0;
			TunerStatus = Status_AST;
			SeekState = Seek_Configure; 	
			break;

		case IN_KEY_P1_SP:
		case IN_KEY_P2_SP:
		case IN_KEY_P3_SP:
		case IN_KEY_P4_SP:
		case IN_KEY_P5_SP:
		case IN_KEY_P6_SP:		
			TunerStatus = Status_Idle;
			SeekState = Seek_Configure; 	
			Tuner_TunetoPreset(gKeyEvent - KEY_EVENT_BASE_ADDR);
			break;

		case IN_KEY_P1_CP:
		case IN_KEY_P2_CP:
		case IN_KEY_P3_CP:
		case IN_KEY_P4_CP:
		case IN_KEY_P5_CP:
		case IN_KEY_P6_CP:
			TunerStatus = Status_Idle;
			SeekState = Seek_Configure; 	
			Tuner_TunetoPreset(gKeyEvent - IN_KEY_P6_SP);
			break;
		}
	}
	if( TunerStatus == Status_Idle) {
		if(TunerMuteFlag == 1)	{		
			Si4730_HardMute(0);
			TunerMuteFlag = 0;
		}
	}
	else if(TunerStatus != Status_Scan) {
		Tuner_Seek(direction,ast,singlestep);
		Si4730_HardMute(1);
		TunerMuteFlag = 1;
		}

#if AST_ON	//here handle preset radio station scan 
		else {


		}
#endif 
}

