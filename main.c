/******************************************************************************
** Filename:    main.c
** Description: The main function of the projection
** Author:  Huangtingxuan
** Date: 2018.8.14
** Device(s) : R7F701025xAFP RH850/F1L
** Tool-Chain : CubeSuite+(V6.01.00)
** Description : This file is a sample of the peripheral function.
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
*******************************************************************************

******************************************************************************
Includes <System Includes> 
******************************************************************************/
#include    "r_typedefs.h"
#include 	  "rscan.h"
#include 	  "can_table.h"
#include    "Mslcan.h"
#include 		"Task1.h"
#include    "Task2.h"
#include    "ostm0.h"
#include 		"UserSys.h"
#include    "TaskUART.h"
/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern  void        R_CLOCK_Init( void );
/******************************************************************************
* Function Name : void main( void )
* Description   : This function is sample main processing.
* Argument      : none
* Return Value  : none
******************************************************************************/

void main( void )
{
  while ( 1U )
  {
		#if taskli
		Task1();
		#elif taskhuang
		Task2();
		#elif APPCAN
		TaskCAN();
		#elif APPLIN
		AppLIN();
		#elif APPUART
		TaskUART();
		#elif APPTIME
		TaskTIME();
		#endif	
	}
}


