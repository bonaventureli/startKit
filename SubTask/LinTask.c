/******************************************************************************
* File Name     : LinTask.c
* Version       : 1.00
* Device(s)     : RH850F1L
* Debugger    : GreenHills
* Description   : This file include the UART function
* Creation Date : 2013/2/27
*******************************************************************************/
#include 	"iodefine.h"
#include  "r_typedefs.h"
#include  "LinTask.h"
#include  "BspTimer.h"
#include  "rlin3_api.h"

void MslLinkRotationTask(void)
{
	static uint32_t LastLinSentTime = 0;
	static uint8_t NodeNum = 0;
	uint32_t TempTime;
	/*Get Current Time*/
  TempTime = GetTimeMs(); 
	/*timeout >= 10s */
	if((TempTime - LastLinSentTime) > 4)
	{
		LastLinSentTime = GetTimeMs(); 
		RLIN3_Init(RLIN3_0);
	  RLIN3_Tx_Frame(RLIN3_0,Lin_NAD_Info[NodeNum],Lin_Tx_Buffer);
		NodeNum++;
		if(NodeNum >3)
		{
			NodeNum = 0;
		}
		
	}
	
}