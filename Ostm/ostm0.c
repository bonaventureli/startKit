/******************************************************************************
* File Name     : ostm0.c
* Version       : 1.00
* Device(s)     : RH850F1L
* Debugger    : GreenHills
* Description   : This file include the UART function
* Creation Date : 2013/2/27
*******************************************************************************/
#include 	"iodefine.h"
#include  "r_typedefs.h"
#include  "Kit1.h"

void OSTM0_enable_int(void);

/******************************************************************************
** Function: 	void OSTM0_INIT(void)
** Description: Initialize OSTIMER.
** Parameter:	None
** Return:  	None
******************************************************************************/
void OSTM0_INIT(void)
{
	OSTM0.CMP = 40000;// 1ms Set the value with which the counter is compared,xxx
	//OSTM0.CMP =  0x3D0900U; //100ms -4000000
	//OSTM0.CMP = 0x1312D00U; //500ms
	OSTM0.CTL = 0X01U;	                     //Set interval mode,disable interrupt at counter start.
	OSTM0.EMU = 0X00U;	                     //The counter clock is stopped when the debugger acquires control of the microcontroller.                                   
	OSTM0_enable_int();
}


/******************************************************************************
** Function: 	void OSTM0_DEINIT(void)
** Description: Enable the interrupt.
** Parameter:	None
** Return:  	None
******************************************************************************/
void OSTM0_DEINIT(void)
{
	ICOSTM0 = 0x0007;						                //enable OS timer interrupt
}


/******************************************************************************
** Function: 	void OSTM0_START(void)
** Description: Start the counter .
** Parameter:	None
** Return:  	None
******************************************************************************/
void OSTM0_START(void)
{
	OSTM0.TS = 0X01;				  		                    //Start the counter.
	while((OSTM0.TE & 0X01) != 0X01);	                    //Wait whether the counter is enable.
}


void OSTM0_enable_int(void) 
{
	#if 0	//EI level
		TBOSTM0=0;
		RFOSTM0=0;
		MKOSTM0=0;
	#else	//FE level
		MKOSTM0=1; //mask EI exception
		FEINTFMSK&=(~0x4000);	//enable FEINT
	#endif
}



/******************************************************************************
** Function: 	void OSTM0_STOP(void)
** Description: Stop the counter.
** Parameter:	None
** Return:  	None
******************************************************************************/
void OSTM0_STOP(void)
{
    OSTM0.TT = 0x01;                                         //Stop the counter.
    while((OSTM0.TE & 0X01) != 0X00);                        //Wait whether the counter is disable.
}

/******************************************************************************
** Function: 	FEINT_interrupt( unsigned long feic )
** Description: Ostm0.
** Parameter:	feic
** Return:  	None
******************************************************************************/
#pragma interrupt FEINT_interrupt(priority=feint, callt=false, fpu=false)
void FEINT_interrupt( unsigned long feic )
{
    if((FEINTF&0x4000)==0x4000)
    {
			FEINTFC |= 0x00004000;
			
			/*ISR reality */
			SysTimeInc();
			
			__nop();
    }
}
		
