/******************************************************************************
** Filename:    Bllcan.h
** Description: This files is designed for the layor of CAN Bus Handle
** Author:  Huangtingxuan
** Date: 2018.8.9
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/
//============================================================================
//
// Header file for the BLL CAN
//
//============================================================================
#ifndef _BLL_CAN_H_
#define _BLL_CAN_H_
#include "Mslcan.h"

typedef struct SignalPosition{
	uint8_t StartByte;
	uint8_t StartBit;
	uint8_t BitLenth;
}T_MASK_MATRIX;  //Used to Matrix of CAN

#define STATUS_FIRE_BUTTOM 		0
#define STATUS_OIL_VOLUME  		1
#define STATUS_DOOR				 		2
#define STATUS_TRUNK			 		2
#define STATUS_WINDOW			  	2
#define STATUS_TEMPERATURN		2
/*...*/
/*...*/
/*...*/

static T_MASK_MATRIX MaskMatrix[] = {
/*NUM   StartByte StartBit  VauleLen  */
/*0*/  {2,        0,		    1,	    }, //Fire Buttom status
/*1*/  {2,        0,		    8,	    }, //Oil volume 
/*2*/  {2,        0,		    8,	    }, //Door status 
/*3*/  {2,        0,		    8,	    }, //Trunk status
/*4*/  {2,        0,		    8,	    }, //Window status
/*5*/  {2,        0,		    8,	    }, //Temperature status 
/*6*/  {0,        0,		    0,	    }, // 
/*7*/  {0,        0,		    0,	    }, // 
/*...*/                                     
}; 

void BllCANPeriodTask(void);
void BllFireJudgeHandle(T_CANDATA *RxData);
uint8_t BllGetValueCan(T_CANDATA *RxData,uint8_t Select);

#endif  //_BLL_CAN_H_
