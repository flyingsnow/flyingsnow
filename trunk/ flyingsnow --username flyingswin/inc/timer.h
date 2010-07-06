#if !defined(__TIMER_H__)
#define __TIMER_H__



/*
**********************************************************
*					GLOBAL DATA TYPE
**********************************************************
*/
// define softtimer structure.
typedef struct	_TIMER
{
	DWORD	TimeOutVal; 								//time out value
	DWORD	RecTickVal;			  						//softtimer setting value
	BOOL	IsTimeOut;									//time out flag
} TIMER;



/*
**********************************************************
*					GLOBAL VARIABLE
**********************************************************
*/
#ifdef	TIMER_GLOBALS
#define	TIMER_EXT
#else
#define	TIMER_EXT	extern
#endif
//TIMER_EXT	DWORD	DATA	gSysTick; 					//system tick counter


/*
**********************************************************
*					GLOBAL FUNCTIONS PROTOTYPE
**********************************************************
*/
//
// Set time out value of softtimer.
//
VOID
TimeOutSet(
	TIMER	*timer,
	DWORD 	timeout			//The maximum value is 0x80000000 ms (about 24 days)
	);


//
// Check whether time out.
//
BOOL
IsTimeOut(
	TIMER 	*timer
	);


//
// set timer count
//
VOID
TimerCountSet(
	BYTE 	div
	);

#endif

