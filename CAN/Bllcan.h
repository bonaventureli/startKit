/******************************************************************************
** Filename:    Bllcan.c
** Description: This files is designed for the layor of CAN Bus Handle
** Author:  Huangtingxuan
** Date: 2018-09-08
** Revise: 
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/
#ifndef _BLL_CAN_H_
#define _BLL_CAN_H_
#include "Mslcan.h"

typedef struct SignalPosition{
	uint8_t 	StartBit;
	uint8_t 	BitLenth;
}T_MASK_MATRIX;  //Used to Matrix of CAN
/*...*/

/*******************************************************/
/*list of function*/
void BllCANPeriodTask(void);
void BllFireJudgeHandle(T_CANDATA *RxData);
void MslGetDoorStatus(T_CANDATA *RxData);
void MslGetMainDoorLockStatus(T_CANDATA *RxData);
void MslGetSubDoorLockStatus(T_CANDATA *RxData);
void MslGetLeftBackDoorLockStatus(T_CANDATA *RxData);
void MslGetRightBackDoorLockStatus(T_CANDATA *RxData);
void MslGetTrunkStatus(T_CANDATA *RxData);
void MslGetEngineCoverStatus(T_CANDATA *RxData);
void MslGetMainDriverWindowStatus(T_CANDATA *RxData);
void MslGetSubDriverWindowStatus(T_CANDATA *RxData);
void MslGetLeftBackWindowStatus(T_CANDATA *RxData);
void MslGetRightBackWindowStatus(T_CANDATA *RxData);
void MslGetCarInsideTemperature(T_CANDATA *RxData);
void MslGetCarOutsideTemperature(T_CANDATA *RxData);
void MslGetSkyLightStatus(T_CANDATA *RxData);
void MslGetCarSpeed(T_CANDATA *RxData);
uint8_t BllGetValueCan(T_CANDATA *RxData,uint8_t Select);

#endif  //_BLL_CAN_H_
