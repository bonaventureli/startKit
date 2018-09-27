/******************************************************************************

* file Name : HAL_can.c
* version   : 0.4
* Argument  : lifei
* data  : 2018/8/24
* describe: 
*PIN Define:
	CAN1	CAN0_TX 		P0_0  2nd Alternative
	CAN1 	CAN0_RX 		P0_1  2nd Alternative
	CAN2 	CAN1_TX     		P10_7 4th Alternative
	CAN2	CAN1_RX			P10_6 4th Alternative
	Baud Rate: 500Kbps
	CAN0
	0010 2th output P0_0   CAN0TX
	0011 2th input  P0_1   CAN0RX
	0011 2th input  P10_0  CAN0RX
	0010 2th output P10_1  CAN0TX
	CAN1
	0110 4th output P10_7  CAN1TX
	0111 4th input  P10_6  CAN1RX
	CAN2
	0000 1st output P0_4   CAN2TX
	0001 1st input  P0_5   CAN2RX
	PFCAEn_m 	PFCEn_m 	PFCn_m 	PMn_m 	Function
	0 		0 		0 	1 	1st alternative function / Input
	0 		0 		0 	0 	1st alternative function / Output
	0 		0 		1 	1 	2nd alternative function / Input
	0 		0 		1 	0 	2nd alternative function / Output
	0 		1 		0 	1 	3rd alternative function / Input
	0 		1 		0 	0 	3rd alternative function / Output
	0 		1 		1 	1 	4th alternative function / Input
	0 		1 		1 	0 	4th alternative function / Output
	1 		0 		0 	1 	5th alternative function / Input
	1 		0 		0 	0 	5th alternative function / Output
	1 		0 		1 	X 	Setting prohibited
	1 		1 		X 	X 	Setting prohibited
******************************************************************************/
#include  "r_typedefs.h"
#include  "iodefine.h"
#include "HAL_can.h"
#include "MslATMode.h"
#include "MslGattMode.h"

T_CANDATA gsTxCANData;
T_CANDATA gsRxCANData;
Can_FrameType CANRecData1;
Can_FrameType CANRecData2;
Can_FrameType CANRecData3;
uint8_t CmpStdData[8];   // to be compare the data of Receive
uint32_t Mask[] = {0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};
volatile uint8_t gFlagNewData1 = 0;
volatile uint8_t gFlagNewData2 = 0;
volatile uint8_t gFlagNewData3 = 0;
const can_cre_type RX_RULE_TABLE_LIST[RX_RULE_NUM] = {
/* CAN 0 reception rules */
    {0x00000010UL, 0xDFFFFFF0UL, 0x00008000UL, 0x00000001UL}, /* NO.000 : std data frm ID = H'10~1F will be accepted, destination Rx Buffer0 */
    {0x00000020UL, 0xDFFFFFF0UL, 0x00008100UL, 0x00000000UL}, /* NO.001 : std data frm ID = H'20~2F will be accepted, destination Rx Buffer1 */
    {0x00000030UL, 0xDFFFFFF0UL, 0x00008200UL, 0x00000000UL}, /* NO.002 : std data frm ID = H'30~3F will be accepted, destination Rx Buffer2 */
    {0x00000040UL, 0xDFFFFFF0UL, 0x00008300UL, 0x00000000UL}, /* NO.003 : std data frm ID = H'40~4F will be accepted, destination Rx Buffer3 */
    {0x00000050UL, 0xDFFFFFFFUL, 0x00008400UL, 0x00000000UL}, /* NO.004 : std data frm ID = H'50 will be accepted, destination Rx Buffer4 */
    {0x00000060UL, 0xDFFFFFFFUL, 0x00008500UL, 0x00000000UL}, /* NO.005 : std data frm ID = H'60 will be accepted, destination Rx Buffer5 */
    {0x00000200UL, 0xDFFFFFFFUL, 0x00000000UL, 0x00000001UL}, /* NO.006 : std data frm ID = H'200 will be accepted, destination RX FIFO buffer0 */
    {0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000001UL}, /* NO.007 : std data frm ID = H'80~8F will be accepted, destination RX FIFO buffer0 */
    {0x90000010UL, 0xDFFFFFF0UL, 0x00008800UL, 0x00000000UL}, /* NO.008 : Ext data frm ID = H'10000010-1F will be accepted, destination Rx Buffer8 */
    {0x90000020UL, 0xDFFFFFF0UL, 0x00008900UL, 0x00000000UL},
    {0x90000030UL, 0xDFFFFFF0UL, 0x00008a00UL, 0x00000000UL},
    {0x90000040UL, 0xDFFFFFF0UL, 0x00008b00UL, 0x00000000UL},
    {0x40000010UL, 0xDFFFFFF0UL, 0x00008c00UL, 0x00000000UL}, //std remote ID = H'010-1F
    {0x40000020UL, 0xDFFFFFF0UL, 0x00008d00UL, 0x00000000UL}, //std remote ID = H'020-2F 
    {0xc0000030UL, 0xDFFFFFFFUL, 0x00008e00UL, 0x00000000UL}, //ext remote ID = H'00000030
    {0xc0000040UL, 0xDFFFFFFFUL, 0x00008f00UL, 0x00000000UL}, 
/* CAN 1 reception rules */
    {0x00000010UL, 0xDFFFFFF0UL, 0x00009000UL, 0x00000000UL}, /* NO.000 : std data frm ID = H'10~1F will be accepted, destination Rx Buffer H10 */
    {0x00000020UL, 0xDFFFFFF0UL, 0x00009100UL, 0x00000000UL}, /* NO.001 : std data frm ID = H'20~2F will be accepted, destination Rx Buffer H11 */
    {0x00000030UL, 0xDFFFFFF0UL, 0x00009200UL, 0x00000000UL}, /* NO.002 : std data frm ID = H'30~3F will be accepted, destination Rx Buffer H12 */
    {0x00000040UL, 0xDFFFFFF0UL, 0x00009300UL, 0x00000000UL}, /* NO.003 : std data frm ID = H'40~4F will be accepted, destination Rx Buffer H13 */
    {0x00000050UL, 0xDFFFFFFFUL, 0x00009400UL, 0x00000000UL}, /* NO.004 : std data frm ID = H'50 will be accepted, destination Rx Buffer H14 */
    {0x00000060UL, 0xDFFFFFFFUL, 0x00009500UL, 0x00000000UL}, /* NO.005 : std data frm ID = H'60 will be accepted, destination Rx Buffer H15 */
    {0x00000200UL, 0xDFFFFFFFUL, 0x00000000UL, 0x00000002UL}, /* NO.006 : std data frm ID = H'200 will be accepted, destination RX FIFO buffer1 */
    {0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000002UL}, /* NO.007 : std data frm ID = H'80~8F will be accepted, destination RX FIFO buffer1 */
		//{0x00000080UL, 0xDFFFFFFFUL, 0x00009700UL, 0x00000000UL}, /* NO.007 : std data frm ID = H'80 will be accepted, destination Rx Buffer H17 */
/* CAN 2 reception rules */
    {0x00000010UL, 0xDFFFFFF0UL, 0x00009000UL, 0x00000000UL}, /* NO.000 : std data frm ID = H'10~1F will be accepted, destination Rx Buffer H10 */
    {0x00000020UL, 0xDFFFFFF0UL, 0x00009100UL, 0x00000000UL}, /* NO.001 : std data frm ID = H'20~2F will be accepted, destination Rx Buffer H11 */
    {0x00000030UL, 0xDFFFFFF0UL, 0x00009200UL, 0x00000000UL}, /* NO.002 : std data frm ID = H'30~3F will be accepted, destination Rx Buffer H12 */
    {0x00000040UL, 0xDFFFFFF0UL, 0x00009300UL, 0x00000000UL}, /* NO.003 : std data frm ID = H'40~4F will be accepted, destination Rx Buffer H13 */
    {0x00000050UL, 0xDFFFFFFFUL, 0x00009400UL, 0x00000000UL}, /* NO.004 : std data frm ID = H'50 will be accepted, destination Rx Buffer H14 */
    {0x0000006UL, 0xDFFFFFFFUL, 0x00009500UL, 0x00000000UL}, /* NO.005 : std data frm ID = H'60 will be accepted, destination Rx Buffer H15 */
    {0x00000200UL, 0xDFFFFFFFUL, 0x00000000UL, 0x00000004UL}, /* NO.006 : std data frm ID = H'200 will be accepted, destination RX FIFO buffer2 */
    {0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000004UL}, /* NO.007 : std data frm ID = H'80~8F will be accepted, destination RX FIFO buffer2 */
		//{0x00000080UL, 0xDFFFFFFFUL, 0x00009700UL, 0x00000000UL}, /* NO.007 : std data frm ID = H'80 will be accepted, destination Rx Buffer H17 */

};

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


/******************************************************************************
* Function Name : CANDataLoad
* Description   : Load Data
* Argument      : Data will to be Load; StartByte
* Return Value  : none
******************************************************************************/
void CANDataLoad(uint8_t* Data,uint8_t StartByte,uint8_t StartBit,uint8_t BitLenth,uint32_t SignValue)
{
	uint8_t TempData;
  
	TempData = Data[StartByte];
	
	TempData &= ~(Mask[BitLenth]<<StartBit);
	TempData |= SignValue<<StartBit;
	
	Data[StartByte] = TempData;
}
/******************************************************************************
* Function Name : CANDataFill
* Description   : Fill Data to Buffer
* Argument      : CANData Buffer Space; Function list of function
* Return Value  : none
******************************************************************************/
void CANDataFill(T_CANDATA *CANData,uint8_t Function)
{
  CANDataLoad(CANData->Data,											// Input Data
							CANCmdMatrix[Function].StartByte,		// StartByte
							CANCmdMatrix[Function].StartBit,		// StartBit
							CANCmdMatrix[Function].BitLenth,		// BitLenth
							CANCmdMatrix[Function].SignValue);	// SignValue
	if(CANCmdMatrix[Function].StdID != 0)
	{
		CANData->StdID = CANCmdMatrix[Function].StdID;  // Frame ID
	}
}
/******************************************************************************
* Function Name : MslCANDataSent
* Description   : CAN Data sent
* Argument      : CANData sent data;
* Return Value  : none
******************************************************************************/
void MslCANDataSent(T_CANDATA *CANData)
{
  Can_FrameType TxFrame;
	uint8_t Num;
	
	if(CAN_TX_RESPOND_EN == 1)
	{
		/*Set Sent Busy*/
		CANData->SentBusy = 1;
		CANData->SendTime = GetTimeMs();
	}
	
	TxFrame.ID = CANData->StdID;		// load ID info
	TxFrame.THLEN = 0;
	TxFrame.RTR = 0;
	TxFrame.IDE = 0;

	TxFrame.TS = 0x0000;
	TxFrame.LBL = 0x000;
	TxFrame.DLC = 0x8;
	memcpy(&TxFrame.DB[0],&CANData->Data[0],8); // load Data info

	for(Num=0;Num < NUM_CANFRAME_ONCE;Num++)
	{
		HAL_Can_CnTxframeByTxBuf(CAN_NUM,Num+1,&TxFrame);
	}
}

/******************************************************************************
* Function Name : MslCANReceive
* Description   : CAN Bus Receive
* Argument      : CANData used to save data;
* Return Value  : which channel was Received
******************************************************************************/
uint8_t MslCANReceive(T_CANDATA *CANData)
{
	uint8_t Retval = 0;
	
	if(CAN_NUM == gFlagNewData1)
	{
		gsRxCANData.StdID = CANRecData1.ID;
		memcpy(&gsRxCANData.Data[0],&CANRecData1.DB[0],8);
		
		gFlagNewData1 = 0;
		Retval = 1;
	}
	else if(CAN_NUM == gFlagNewData2)
	{
		gsRxCANData.StdID = CANRecData2.ID;
		memcpy(&gsRxCANData.Data[0],&CANRecData2.DB[0],8);
	
		gFlagNewData2 = 0;
		Retval = 2;
	}
	else if(CAN_NUM == gFlagNewData3)
	{
		gsRxCANData.StdID = CANRecData3.ID;
		memcpy(&gsRxCANData.Data[0],&CANRecData3.DB[0],8);
	
		gFlagNewData3 = 0;
		Retval = 3;
	}	
	return Retval;
}

/******************************************************************************
* Function Name : CANDataReset
* Description   : Reset the CAN Buffer
* Argument      : CANData used to save data;
* Return Value  : void
******************************************************************************/
void CANDataReset(T_CANDATA *CANData)
{
	memset(CANData->Data,0,8);
	CANData->StdID = 0;
}

/******************************************************************************
* Function Name : MslCANCmdExecute
* Description   : CAN Bus sent
* Argument      : CANData sent data;
* Return Value  : none
******************************************************************************/
void MslCANCmdExecute(uint8_t Function)
{
	/*Reset Data*/
	CANDataReset(&gsTxCANData);
	/*Load Function Opcode into Buff*/
	CANDataFill(&gsTxCANData,Function);
	CANDataFill(&gsTxCANData,FRAME_ASSEMBLY_1);
	CANDataFill(&gsTxCANData,FRAME_ASSEMBLY_2);
	/*...*/
	
	/*Sent CAN Data Twice*/
	MslCANDataSent(&gsTxCANData);
	if(CAN_TX_RESPOND_EN == 1)
	{
		gsTxCANData.ReSentCount = 0;
	}	
}

/******************************************************************************
* Function Name : MslCANRespondTask
* Description   : decode 0x200
* Argument      : RxData the reseived data;
* Return Value  : none
******************************************************************************/
void MslCANRespondTask(T_CANDATA *RxData)
{
	if(CAN_TX_RESPOND_EN == 1)
	{
		MslCANCmdRespond(&gsTxCANData,RxData);
	}
}

/******************************************************************************
* Function Name : MslCANCmdRespond
* Description   : CAN Bus sent
* Argument      : TxData will sent data;RxData reseived data
* Return Value  : none
******************************************************************************/
static void MslCANCmdRespond(T_CANDATA *TxData,T_CANDATA *RxData)
{	
	if(TxData->SentBusy != 1)
	{
		return;
	}
	
	if(RxData->Data[2] == 0x01)
	{
		TxData->SentBusy = 0;
	}
}

/******************************************************************************
* Function Name : MslCANPeriodTask
* Description   : Period Task
* Argument      : handle the Can Task;
* Return Value  : none
******************************************************************************/
void MslCANPeriodTask(void)
{
	if(gsTxCANData.SentBusy == 1)
	{
		if(GetTimeMs() - gsTxCANData.SendTime > RESENT_TIME)
		{
			/*Sent CAN Data Twice*/
			MslCANDataSent(&gsTxCANData);
			gsTxCANData.ReSentCount++;
			if(gsTxCANData.ReSentCount > (RESENT_COUNT - 1))
			{
				gsTxCANData.ReSentCount = 0;
				gsTxCANData.SentBusy = 0;
			}
		}
	}
}

/******************************************************************************
* Function Name : MslCANSentFromSDK
* Description   : Service for Can Bus after SDK decode
* Argument      : Opcodes used to save data;
* Return Value  : which channel was Received
******************************************************************************/
void MslCANSentFromSDK(uint8_t Opcodes)
{
	switch(Opcodes)
	{
		case 0x01:	/*Unlock*/
			MslCANCmdExecute(DOOR_UNLOCK);
		break;
			
		case 0x02:	/*Lock*/
			MslCANCmdExecute(DOOR_LOCK);
		break;
		
		case 0x03:	/*Trunk Open*/
		  MslCANCmdExecute(TRUNK_UNLOCK);
		break;
		
		/*...*/
		
		case 0x07:	/*Find Car*/
			MslCANCmdExecute(WHISTLE);
		break;
		
		case 0x0B:	/*Open Air*/
			MslCANCmdExecute(SKYLIGHT_OPEN);
		break;
			
		case 0x0C:	/*Close Air*/
			MslCANCmdExecute(SKYLIGHT_CLOSE);
		break;
		
		case 0x0D:	/*Up Windows*/
			MslCANCmdExecute(WINDOWNS_UP);
		break;
		
		case 0x0E:	/*Down Windows*/
			MslCANCmdExecute(WINDOWNS_DOWN);
		break;

		
	}
}





/******************************************************************************
* Function Name : MslCANRxTxDemo
* Description   : CAN Bus Receive
* Argument      : CANData used to save data;
* Return Value  : which channel was Received
******************************************************************************/
void MslCANRxTxDemo(void)
{
	if(1 == gFlagNewData1)
	{
		gsRxCANData.StdID = CANRecData1.ID;
		memcpy(&gsRxCANData.Data[0],&CANRecData1.DB[0],8);
		HAL_Can_CnTxframeByTxBuf(1,1,&CANRecData1);
		gFlagNewData1 = 0;
	}
	else if(2 == gFlagNewData2)
	{
		gsRxCANData.StdID = CANRecData2.ID;
		memcpy(&gsRxCANData.Data[0],&CANRecData2.DB[0],8);
		HAL_Can_CnTxframeByTxBuf(2,1,&CANRecData2);
		gFlagNewData2 = 0;
	}
	else if(3 == gFlagNewData3)
	{
		gsRxCANData.StdID = CANRecData3.ID;
		memcpy(&gsRxCANData.Data[0],&CANRecData3.DB[0],8);
	  HAL_Can_CnTxframeByTxBuf(3,1,&CANRecData3);
		gFlagNewData3 = 0;
	}	

}

/******************************************************************************
* Function Name : MslCANSentDemo
* Description   : Used to test CAN Sent Mode
* Argument      : void;
* Return Value  : void;
******************************************************************************/
void MslCANSentDemo(void)
{
	Can_FrameType TxFrame;
	uint8_t Num;
	static uint32_t LastRecordTime;
	uint8_t DataTemp[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};

	while(1)
	{
		if(GetTimeMs()-LastRecordTime<100)
		{
			LastRecordTime = GetTimeMs();
			
			TxFrame.ID = 0x81;		// load ID info
			TxFrame.THLEN = 0;
			TxFrame.RTR = 0;
			TxFrame.IDE = 1;

			TxFrame.TS = 0x0000;
			TxFrame.LBL = 0x000;
			TxFrame.DLC = 0x8;
			memcpy(&TxFrame.DB[0],DataTemp,8); // load Data info
			
			for(Num=0;Num<15;Num++)
			{
				HAL_Can_CnTxframeByTxBuf(1,Num,&TxFrame);
			}
			for(Num=0;Num<15;Num++)
			{
				HAL_Can_CnTxframeByTxBuf(2,Num,&TxFrame);
			}
			for(Num=0;Num<15;Num++)
			{
				HAL_Can_CnTxframeByTxBuf(3,Num,&TxFrame);
			}
		}
	}
}

/*when CANInitFinish ,Sent One Frame to Bus as test*/
void MslCANDemoOneFrame(void)
{
	T_CANDATA CanData={0x81,{0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88}};
	MslCANDataSent(&CanData);
}
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
//line 918
