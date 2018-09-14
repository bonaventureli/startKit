/******************************************************************************
** Filename:    MslGattMode.c
** Description: This files is used to the GATT Functions
** Author:  Huangtingxuan
** Date: 2018-09-08
** Revise: 
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/
//============================================================================
// Includes
//============================================================================
#include "MslGattMode.h"
#include "Uart30.h"
#include "iodefine.h"
#include "r_typedefs.h"

T_Car_Status gsCarStatus;

T_GATT_PARA_STRUCT GattTransPara[5] = {
//   	 Opt0		Opt1		UUID0		UUID1
/*0*/	{0x50,	0x04,		0xFF,		0xF4},
};

uint8_t  gStaSentTimes = 0;
static uint32_t gLastTimeGattSent = 0;
/******************************************************************************
** Function:    MslSetStatusSentTimes
** Description: Used for GATT MODE Sent
** Parameter:   Times
** Return:      None
******************************************************************************/
void MslSetStatusSentTimes(uint8_t Times)
{
	gStaSentTimes = Times;
	if((GetTimeMs() - gLastTimeGattSent) > 1000)
	{
		gLastTimeGattSent = GetTimeMs();
	}
}

/******************************************************************************
** Function:    MslGattUartSent
** Description: Used for GATT MODE Sent
** Parameter:   Term
** Return:      None
******************************************************************************/
void MslGattPeriodTask(void)
{
	if(gStaSentTimes != 0)
	{
		if((GetTimeMs() - gLastTimeGattSent) > 700)
		{
			gStaSentTimes--;
			gLastTimeGattSent = GetTimeMs();
			MslGattUartSent(0,(uint8_t*)&gsCarStatus,sizeof(T_Car_Status));
		}
	}
	
}

uint8_t GattSendBuff[200];
/******************************************************************************
** Function:    MslGattUartSent
** Description: Used for GATT MODE Sent
** Parameter:   Term
** Return:      None
******************************************************************************/
void MslGattUartSent(uint8_t Term,uint8_t *DataPayload,uint8_t DataLen)
{
	T_GATT_STRUCT* GattSendStruct;

	GattSendStruct = (T_GATT_STRUCT*)GattSendBuff;
	
	GattSendStruct->Header 	 	= 0x7E;
	GattSendStruct->AddCode 	= 0x10;
	GattSendStruct->OptCode0 	= GattTransPara[Term].OptCode0;
	GattSendStruct->OptCode1	= GattTransPara[Term].OptCode1;
	GattSendStruct->UUID0			= GattTransPara[Term].UUID0;
	GattSendStruct->UUID1 		= GattTransPara[Term].UUID1;
	
	GattSendStruct->DataLen  	= DataLen;
	memcpy(&GattSendStruct->Data0,DataPayload,DataLen);
	
	GattSendBuff[7+DataLen] 	= 0xFF;
	GattSendBuff[8+DataLen] 	= 0xFF;
	
	Uart3Sent(GattSendBuff,DataLen+9);
}



