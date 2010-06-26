/*
*****************************************************************************
*					Mountain View Silicon Tech. Inc.
*	Copyright 2007, Mountain View Silicon Tech. Inc., ShangHai, China
*					All rights reserved.
*
* Filename:			type.h
* Description:		Header file for type definition.
*
* Change History:
*			Jeff Dai	- 12/xx/2006 - V0.1
*						- created
*			Grey		- 12/25/2006 - V0.2
*						- modify code for 78
*			Jack		- 8/xx/2007 - V0.3
*						- change code layout
******************************************************************************
*/
#if !defined(__TYPE_H__)
#define __TYPE_H__



/*
**********************************************************
*					GLOBAL CONSTANT
**********************************************************
*/
#define FALSE			0
#define TRUE			1

#define	SUCCESS			0
#define	FAILURE			1

#define					IN
#define					OUT

#define	BYTE_HIGH		0			
#define	BYTE_LOW		1



/*
**********************************************************
*					GLOBAL MACRO
**********************************************************
*/
#define	XDATA			xdata
#define	DATA			data
#define	CODE			code

#define XBYTE ((unsigned char volatile xdata *) 0)

/*
**********************************************************
*					GLOBAL DATA TYPE
**********************************************************
*/
typedef void				VOID, *PVOID;
typedef char				BOOL;
typedef char				BOOLEAN;
typedef char				*PCHAR;
typedef char				CHAR;
typedef unsigned char		BYTE, UCHAR, *PUCHAR;
typedef unsigned int		UINT;
typedef unsigned short		USHORT, *PUSHORT;
typedef unsigned short		WORD, *PWORD;
typedef unsigned long		DWORD, ULONG, *PULONG;
typedef volatile UCHAR		*PVUCHAR;
typedef volatile USHORT		*PVUSHORT;
typedef volatile ULONG 		*PVULONG;


typedef signed char 		SBYTE;
typedef signed int			SWORD;
typedef signed long			SDWORD;


#endif

