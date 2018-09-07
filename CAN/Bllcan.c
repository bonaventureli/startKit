/******************************************************************************
** Filename:    Bllcan.c
** Description: This files is designed for the layor of CAN Bus Handle
** Author:  Huangtingxuan
** Date: 2018.8.9
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/
//============================================================================
//
// Source file for the MSL CAN
//
//============================================================================
#include "Bllcan.h"
#include "UserSys.h"


#define ID_KESSY04_STATUS 				0x592 //Is pressed or not the fire buttom
#define ID_RESPONDSE_GW						0x200	//whether receive the respond of GW
#define ID_STATUS_CAR             0x000	//

/******************************************************************************
** Function:    BllCANPeriodTask
** Description: Periodic call
** Parameter:   None
** Return:      None
******************************************************************************/
void BllCANPeriodTask(void)    		// CAN Period Task
{
	uint8_t FlagReception = 0;

	FlagReception = MslCANReceive(&gsRxCANData);
	if(FlagReception == 0)
	{
		return;
	}
	switch(gsRxCANData.StdID)
	{
		case ID_KESSY04_STATUS:
			BllFireJudgeHandle(&gsRxCANData);
		break;
		
		case ID_RESPONDSE_GW:
			MslCANRespondTask(&gsRxCANData);
		break;
		
		case ID_STATUS_CAR:
			//MslCANRespondTask(&gsRxCANData);
		break;
		/*...*/
		
		default:
		
		break;
		
		
	}
	
}

/******************************************************************************
** Function:    BllFireJudgeHandle
** Description: To determine whether ignition
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void BllFireJudgeHandle(T_CANDATA *RxData)
{	
	/*scanf the status of fire buttom*/
	if(GetCarStatus(STATUS_KEY) == 1)
	{
		/*when the fire button was pressed*/
		if(BllGetValueCan(RxData,STATUS_FIRE_BUTTOM) == 0x01)           /*startbit:16 bitlen:1*/
		{
			/*Fire command*/
			MslCANCmdExecute(ENGINE_START);
		}
	}
}

/*...*/

uint8_t DataMask[] = {0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};
/******************************************************************************
** Function:    BllGetValueCan
** Description: Get value of Can data
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
uint8_t BllGetValueCan(T_CANDATA *RxData,uint8_t Select)
{
	uint8_t Retval,Bytestart,Bitstart,Valuelen;
	
	Bytestart = MaskMatrix[Select].StartByte;
	Bitstart  = MaskMatrix[Select].StartBit;
	Valuelen  = MaskMatrix[Select].BitLenth;
	
	Retval = (RxData->Data[Bytestart]>>Bitstart)&DataMask[Valuelen];

	return Retval;
}