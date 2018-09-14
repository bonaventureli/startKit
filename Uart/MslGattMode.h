//============================================================================
//
// Header file for the GATT source code
//
//============================================================================
#ifndef _MSL_GATT_MODE_H_
#define _MSL_GATT_MODE_H_
#include "r_typedefs.h"

#define CAR_STATUS		0

/*******************************************************/
/*used to descript the status of Car*/
typedef struct 
{
	uint8_t 	StatusFourDoor:4;
	uint8_t 	StatusMainDoorLock:1;
	uint8_t 	StatusSubDoorLock:1;
	uint8_t 	StatusLeftBackDoorLock:1;
	uint8_t 	StatusRightBackDoorLock:1;
	
	uint8_t 	StatusSkyLight:4;
	uint8_t 	StatusTrunk:1;
	uint8_t 	StatusEngineCover:1;
	uint8_t 	Reserved0:1;
	
	uint8_t		MainCarWindow:4;
	uint8_t		SubCarWindow:4;
	
	uint8_t		LeftBackWindow:4;
	uint8_t		RightBackWindow:4;
	
	uint8_t		CarInsideTemperature;
	uint8_t 	CarOutsideTemperature;
	uint8_t		EngineSpeed0;
	uint8_t		EngineSpeed1;
	uint8_t		CarSpeed;
	
	uint8_t 	Reserved1;
	uint8_t 	Reserved2;
}T_Car_Status; 

/*******************************************************/
/*used to save the data of sent*/
typedef struct GattStruct{
	uint8_t Header;
	uint8_t AddCode;
	uint8_t OptCode0;
	uint8_t OptCode1;
	uint8_t UUID0;
	uint8_t UUID1;
	uint8_t DataLen;
	uint8_t Data0;
}T_GATT_STRUCT;

/*******************************************************/
/*used to save the parameter of Gatt*/
typedef struct GattParaStruct{
	uint8_t OptCode0;
	uint8_t OptCode1;
	uint8_t UUID0;
	uint8_t UUID1;
}T_GATT_PARA_STRUCT;


/*******************************************************/
/*list of variable of extern*/
extern T_Car_Status gsCarStatus;

/*******************************************************/
/*list of function*/
void MslGattPeriodTask(void);
void MslGattUartSent(uint8_t Term,uint8_t *DataPayload,uint8_t DataLen);
void MslSetStatusSentTimes(uint8_t Times);

#endif
