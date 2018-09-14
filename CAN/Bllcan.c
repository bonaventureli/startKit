/******************************************************************************
** Filename:    Bllcan.c
** Description: This files is designed for the layor of CAN Bus Handle
** Author:  Huangtingxuan
** Date: 2018-09-08
** Revise: 
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/
#include "Bllcan.h"
#include "UserSys.h"
#include "MslATMode.h"
#include "MslGattMode.h"

/*******************************************************/
/*list of CAN ID*/
#define ID_KESSY04_STATUS 				0x592 //Is pressed or not the fire buttom
#define ID_RESPONDSE_GW						0x200	//whether receive the respond of GW
#define ID_BCM_01			            0x65A	//engine cover
#define ID_ZV_02									0x583	//door status leftback rightback lock status
#define ID_TSG_FT_01              0x3D0 //main driver lock status & window status
#define ID_TSG_BT_01							0x3D1 //subdriver lock status & window status
#define ID_TSG_HFS_01							0x3CE	//leftback lock status & window status
#define ID_TSG_HBFS_01						0x3CF	//rightback lock status & window status
#define ID_MD1_Position						0x5E0 //skylight status
#define ID_KLIMA_SENSOR_02				0x5E1 //sensor status
#define	ID_KLIMA_03								0x66E	//temperature of inside car
/*******************************************************/
/*list of Term Num*/
#define STATUS_FIRE_BUTTOM 				0
#define STATUS_ENGINE_COVER  			1
#define STATUS_DOOR								2
#define STATUS_TRUNK							3
#define STATUS_PILOT_DOORLOCK					4
#define STATUS_PSSENGER_DOORLOCK			5
#define STATUS_LEFTBACK_DOORLOCK			6
#define STATUS_RIGHTBACK_DOORLOCK			7
#define STATUS_PILOT_WINDOW				8
#define STATUS_PSSENGER_WINDOW		9
#define STATUS_LEFTBACK_WINDOW		10
#define STATUS_RIGHTBACK_WINDOW		11
#define STATUS_SKYLIGHT_WINDOW		12
#define SPEED_VEHICLE  						13
#define SPEED_UNIT								14	
#define INSIDE_TEMPER							15
#define OUTSIDE_TEMPER						16
#define STATUS_FAN 								17

#define STATUS_OIL_VOLUME  			
#define STATUS_TEMPERATURN

/*******************************************************/
/*Mask data of CAN*/
const T_MASK_MATRIX MaskMatrix[] = {
/*NUM   StartBit  VauleLen  */
/*0*/  {16,		    1,	    }, //Fire Buttom status
/*1*/  {31,		    1,	    }, //engine cover 
/*2*/  {24,		    4,	    }, //door status 
/*3*/  {28,		    1,	    }, //Trunk status
/*4*/  {1,		    1,	    }, //main driver site door status
/*5*/  {1,		    1,	    }, //subdriver site door status
/*6*/  {50,		    1,	    }, //leftback door status
/*7*/  {52,		    1,	    }, //rightback door status
/*8*/  {24,		    8,	    }, //main driver site window status
/*9*/  {24,		    8,	    }, //subdriver site window status
/*10*/ {24,		    8,	    }, //leftback window status
/*11*/ {24,		    8,	    }, //rightback window status
/*12*/ {8,		    7,	    }, //skylight window status
/*13*/ {24,		    9,	    }, //vehicle speed
/*14*/ {17,		    1,	    }, //speed unit
/*15*/ {40,		    8,	    }, //car inside temperature
/*16*/ {8,		    8,	    }, //car outside temperature
/*17*/ {26,		    4,	    }, //fan status
/*...*/                                     
}; 

/*******************************************************/
/*list of function*/

/******************************************************************************
** Function:    BllCANPeriodTask
** Description: Periodic call
** Parameter:   None
** Return:      None
******************************************************************************/
void BllCANPeriodTask(void)    		// CAN Period Task
{
	uint8_t FlagReception = 0;

	MslAtPeriodTask();	 /*AT Task Period Task*/
	MslGattPeriodTask(); /*GATT Task Period Task*/
	
	/*judge to received new CAN data*/
	FlagReception = MslCANReceive(&gsRxCANData); 
	if(FlagReception != 0)
	{
		switch(gsRxCANData.StdID)
		{
			case ID_KESSY04_STATUS: //fire buttom pressed
				BllFireJudgeHandle(&gsRxCANData);	//one key fire
			break;
			
			case ID_RESPONDSE_GW:	//CAN resent handle 
				MslCANRespondTask(&gsRxCANData);
			break;
			
			case ID_ZV_02:	//Door status & lock status
				MslGetDoorStatus(&gsRxCANData);	//four door
				MslGetTrunkStatus(&gsRxCANData);	//trunk
				MslGetLeftBackDoorLockStatus(&gsRxCANData);	//leftback door
				MslGetRightBackDoorLockStatus(&gsRxCANData);	//rightback door
			break;
			
			case ID_BCM_01:	//Engine Cover status
				MslGetEngineCoverStatus(&gsRxCANData);	//engine cover
			break;
			
			case ID_TSG_FT_01:	//main driver door Lock status & window status
				MslGetMainDoorLockStatus(&gsRxCANData);	//main door
				MslGetMainDriverWindowStatus(&gsRxCANData);	//main window
			break;
			
			case ID_TSG_BT_01:	//sub driver door Lock status & window status
				MslGetSubDoorLockStatus(&gsRxCANData);	//sub door
				MslGetSubDriverWindowStatus(&gsRxCANData);	//sub window
			break;
			
			case ID_TSG_HFS_01:	//leftback door Lock status & window status
				MslGetLeftBackWindowStatus(&gsRxCANData);	//leftback window
			break;
			
			case ID_TSG_HBFS_01:	//rightback door Lock status & window status
				MslGetRightBackWindowStatus(&gsRxCANData);	//rightback window
			break;
			
			case ID_MD1_Position:	//leftback door Lock status & window status
				MslGetSkyLightStatus(&gsRxCANData);	//skylight window
			break;
			
			case ID_KLIMA_03:	//inside car temperature
				MslGetCarInsideTemperature(&gsRxCANData);	//get inside car temperature
			break;
			
			case ID_KLIMA_SENSOR_02:	//outside car temperature
				MslGetCarOutsideTemperature(&gsRxCANData);	//get outside car temperature
			break;
	
			/*...*/
			
			default:
			
			break;
			
		}
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
	/*when the fire button was pressed*/
	if(BllGetValueCan(RxData,STATUS_FIRE_BUTTOM) == 0x01)           /*startbit:16 bitlen:1*/
	{
		SetKeyStatus(0); 	//Clear key status
		MslAtCmdSent(REQ_KEYSITE);	//Sent Request of key status
		SetFireButtom(1);
	}
}

/******************************************************************************
** Function:    MslGetDoorStatus
** Description: To get the status of four door
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetDoorStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusFourDoor = BllGetValueCan(RxData,STATUS_DOOR);
}

/******************************************************************************
** Function:    MslGetMainDoorLockHandle
** Description: To get the status of main door lock
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetMainDoorLockStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusMainDoorLock = BllGetValueCan(RxData,STATUS_PILOT_DOORLOCK);
}

/******************************************************************************
** Function:    MslGetSubDoorLockHandle
** Description: To get the status of sub door lock
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetSubDoorLockStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusSubDoorLock = BllGetValueCan(RxData,STATUS_PSSENGER_DOORLOCK);
}

/******************************************************************************
** Function:    MslGetLeftBackDoorLockHandle
** Description: To get the status of leftback door lock
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetLeftBackDoorLockStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusLeftBackDoorLock = BllGetValueCan(RxData,STATUS_LEFTBACK_DOORLOCK);
}

/******************************************************************************
** Function:    MslGetRightBackDoorLockHandle
** Description: To get the status of leftback door lock
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetRightBackDoorLockStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusRightBackDoorLock = BllGetValueCan(RxData,STATUS_RIGHTBACK_DOORLOCK);
}

/******************************************************************************
** Function:    MslGetTrunkStatusHandle
** Description: To get the status of car trunk
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetTrunkStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusTrunk = BllGetValueCan(RxData,STATUS_TRUNK);
}

/******************************************************************************
** Function:    MslGetEngineCoverStatusHandle
** Description: To get the status of engine cover
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetEngineCoverStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusEngineCover = BllGetValueCan(RxData,STATUS_ENGINE_COVER);
}

/******************************************************************************
** Function:    MslGetEngineCoverStatusHandle
** Description: To get the status of car trunk
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetMainDriverWindowStatus(T_CANDATA *RxData)
{
	gsCarStatus.MainCarWindow = BllGetValueCan(RxData,STATUS_PILOT_WINDOW)/20;
}

/******************************************************************************
** Function:    MslGetEngineCoverStatusHandle
** Description: To get the status of car trunk
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetSubDriverWindowStatus(T_CANDATA *RxData)
{
	gsCarStatus.SubCarWindow = BllGetValueCan(RxData,STATUS_PSSENGER_WINDOW)/20;
}

/******************************************************************************
** Function:    MslGetLeftBackWindowStatus
** Description: To get the status of car trunk
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetLeftBackWindowStatus(T_CANDATA *RxData)
{
	gsCarStatus.LeftBackWindow = BllGetValueCan(RxData,STATUS_LEFTBACK_WINDOW)/20;
}

/******************************************************************************
** Function:    MslGetRightBackWindowStatus
** Description: To get the status of car trunk
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetRightBackWindowStatus(T_CANDATA *RxData)
{
	gsCarStatus.RightBackWindow = BllGetValueCan(RxData,STATUS_RIGHTBACK_WINDOW)/20;
}

/******************************************************************************
** Function:    MslGetSkyLightStatus
** Description: To get the status of skylight
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetSkyLightStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusSkyLight = BllGetValueCan(RxData,STATUS_SKYLIGHT_WINDOW)/10;
}

/******************************************************************************
** Function:    MslGetCarInsideTemperature
** Description: To get the status of skylight
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetCarInsideTemperature(T_CANDATA *RxData)
{
	gsCarStatus.CarInsideTemperature = BllGetValueCan(RxData,INSIDE_TEMPER);
}

/******************************************************************************
** Function:    MslGetCarOutsideTemperature
** Description: To get the status of skylight
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetCarOutsideTemperature(T_CANDATA *RxData)
{
	gsCarStatus.CarOutsideTemperature = BllGetValueCan(RxData,OUTSIDE_TEMPER);
}

/******************************************************************************
** Function:    MslGetCarSpeed
** Description: To get the speed of car
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetCarSpeed(T_CANDATA *RxData)
{
	if(BllGetValueCan(RxData,SPEED_UNIT) == 1)
		gsCarStatus.CarSpeed = BllGetValueCan(RxData,SPEED_VEHICLE)/1000;
	else
		gsCarStatus.CarSpeed = BllGetValueCan(RxData,SPEED_VEHICLE);
}

/*******************************************************/
/*basic function list*/
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
	
	Bytestart = MaskMatrix[Select].StartBit/8;
	Bitstart  = MaskMatrix[Select].StartBit%8;
	Valuelen  = MaskMatrix[Select].BitLenth;
	
	Retval = (RxData->Data[Bytestart]>>Bitstart)&DataMask[Valuelen];

	return Retval;
}