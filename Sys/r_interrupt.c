/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* (c) 2014 Renesas Electronics Corporation All rights reserved.
*******************************************************************************/


/******************************************************************************
* File Name     : r_interrupt.c
* Version       : 1.0
* Device(s)     : R7F701035xAFP RH850/F1L
* Tool-Chain    : CubeSuite+(V2.01.00)
* Description   : This file is a sample of the interrupt.
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 20.03.2014 1.00    First Release
******************************************************************************/
/******************************************************************************
Includes <System Includes> , ÅgProject IncludesÅh
******************************************************************************/

#pragma interrupt priority7_interrupt( enable=false , priority=EIINT_PRIORITY7 , callt=false , fpu=false )

#include    "r_typedefs.h"
#include    "iodefine.h"
#include    "rscan.h"
#include    "Uart33.h"
#include    "BspTimer.h"

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void    priority7_interrupt( uint32_t );

		
/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
* Function Name : void priority7_interrupt( uint32_t )
* Description   : This function is priority (7) interrupt processing. (direct vector)
* Argument      : regEIIC_value     : Interrupt factor
* Return Value  : none
******************************************************************************/

extern Can_FrameType CAN_Trans_Data;

void priority7_interrupt( uint32_t regEIIC_value )
{
	IntTimerCallback(regEIIC_value);
	IntUart3Callback(regEIIC_value);
	IntCANRecvCallback(regEIIC_value);
	ISRWDTCallBack(regEIIC_value);
}


