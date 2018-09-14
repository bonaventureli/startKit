/******************************************************************************
** Filename:    MslATMode.h
** Description: This files is used to the AT+MODE Functions
** Author:  Huangtingxuan
** Date: 2018-09-08
** Revise: 
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/
#ifndef _MSL_AT_MODE_H_
#define _MSL_AT_MODE_H_
#include "r_typedefs.h"

#define CMD_NUM   18
#define CMD_LEN   30

#define MCU_CMD_NUM   10
#define MCU_CMD_LEN   30

#define REQ_KEYSITE   0
/*************************************************************/
extern char ATCmdRH850toBLE[MCU_CMD_NUM][MCU_CMD_LEN];

/*************************************************************/
void MslAtPeriodTask(void);
void MslAtCmdSent(uint8_t Term);
uint8_t GetFireButtom(void);
void SetFireButtom(uint8_t Status);
void ReRequestStatusOfKey(void);

uint8_t CmdAnalyer(uint8_t* Data,uint16_t Len);
uint8_t RssiAnalyer(uint8_t* Data,uint16_t Len);

#endif
