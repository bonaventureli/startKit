/******************************************************************************
** Filename:    Mslcan.h
** Description: This files is designed for the layor of CAN Bus Handle
** Author:  Huangtingxuan
** Date: 2018.8.9
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/
//============================================================================
//
// Header file for the MSL CAN
//
//============================================================================
#ifndef _MSL_CAN_H_
#define _MSL_CAN_H_

#include "r_typedefs.h"
#include "rscan.h"

/****************************************************************************/
#define NUM_CANFRAME_ONCE 		1   // the value: 1~15

#define CAN_TX_RESPOND_EN 		1		// the value: 1/0
#define RESENT_TIME   				1000	// the value: 100~700(ms)
#define RESENT_COUNT  				1		// the value: 1~n

/****************************************************************************/

typedef struct CanMatrix{
	uint32_t  StdID;
	uint8_t StartByte;
	uint8_t StartBit;
	uint8_t BitLenth;
	uint8_t SignValue;
}T_CAN_MATRIX;  //Used to Matrix of CAN

typedef struct CANData{
  uint32_t StdID;
  uint8_t	 Data[8];
	uint8_t	 SentBusy;
	uint32_t SendTime;
	uint8_t  ReSentCount;
	
}T_CANDATA;  // The Define of CAN Data Struct
#define FRAME_PROPERTY  	0  // Config the can Data property
#define LAMP_BLINK    		1
#define DOOR_LOCK     		2
#define DOOR_UNLOCK     	3
#define WHISTLE       		4 // Beep
#define TRUNK_UNLOCK  		5
#define WINDOWNS_UP  	 		6
#define WINDOWNS_DOWN	 		7
#define FIND_CAR					4
#define SKYLIGHT_OPEN			8
#define SKYLIGHT_CLOSE 		9
#define WELCOME       		10
#define ENGINE_START  		11
/*Add Frame Option*/
#define FRAME_ASSEMBLY_1	12
#define FRAME_ASSEMBLY_2	13
/*CAN Matrix*/
static T_CAN_MATRIX CANCmdMatrix[] = {
/*NUM    StdID   StartByte StartBit VauleLen Value */
/*0*/  { 0x000,  0,        0,		    0,	     0x00}, // IDLE/FRAME_PROPERTY
/*1*/  { 0x100,  2,        0,		    8,	     0x01}, // LAMP_BLINK
/*2*/  { 0x100,  2,        0,		    8,	     0x02}, // DOOR_UNLOCK
/*3*/  { 0x100,  2,        0,		    8,	     0x03}, // DOOR_LOCK
/*4*/  { 0x100,  2,        0,		    8,	     0x0E}, // Hupe/Beep
/*5*/  { 0x100,  2,        0,		    8,	     0x0A}, // Tail openning
/*6*/  { 0x100,  2,        0,		    8,	     0x08}, // WINDOWNS_Raise 
/*7*/  { 0x100,  2,        0,		    8,	     0x09}, // WINDOWNS_Drop
                                             
/*8*/  { 0x100,  2,        0,		    8,	     0x0C}, // Skylight Openning
/*9*/  { 0x100,  2,        0,		    8,	     0x0D}, // Skylight Closing
/*10*/ { 0x100,  2,        0,		    8,	     0x12}, // Welcome... 
/*11*/ { 0x100,  2,        0,		    8,	     0x11}, // Engine Start...
                                             
/*12*/ { 0x000,  0,        0,		    1,	     0x01}, // FRAME_ASSEMBLY_1...
/*13*/ { 0x000,  1,        0,		    8,	     0x60}, // FRAME_ASSEMBLY_2...	
};   // The define of CAN Matrix    

extern T_CANDATA gsTxCANData;
extern T_CANDATA gsRxCANData;

/*CAN Period Task*/
void MslCANRespondTask(T_CANDATA *RxData);
void MslCANPeriodTask(void);

/*Handle SDK Decode CMD ,will call MslCANCmdExecute   */
void MslCANSentFromSDK(uint8_t Opcodes);
/*CAN CMD Execute*/
void MslCANCmdExecute(uint8_t Function);

static void MslCANCmdRespond(T_CANDATA *TxData,T_CANDATA *RxData);

void CANDataFill(T_CANDATA *CANData,uint8_t Function);
void MslCANDataSent(T_CANDATA *CANData);
uint8_t MslCANReceive(T_CANDATA *CANData);

void MslCANRxTxDemo(void);
void MslCANDemoOneFrame(void);


#endif
