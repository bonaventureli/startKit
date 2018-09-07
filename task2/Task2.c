#include "Task2.h"

#include "Bllcan.h"
#include "Mslcan.h"
//#include "BspTimer.h"
#include "UserSys.h"
#include "Uart33.h"
#include "ostm0.h"
#include "Kit1.h"
#include "ostm0.h"
#include "LinTask.h"
#include "BspWatchDog.h"
#include    "rlin3_api.h"
#include    "iodefine.h"

#define CMD_NUM   16
#define CMD_LEN   30

const char CmdData[CMD_NUM][CMD_LEN]={
/* BLE-->MCU CMD Sets */
"AT+UNLOCK\r\n",           /* 1 */
"AT+LOCK\r\n",             /* 2 */
"AT+TRUNK_OPEN\r\n",       /* 3 */
"AT+FIRE\r\n",             /* 4 */
"AT+LEFT_DOOR\r\n",        /* 5 */
"AT+RIGHT_DOOR\r\n",       /* 6 */
"AT+FIND_CAR\r\n",         /* 7 */
"AT+UNFIRE\r\n",           /* 8 */
"AT+COOL_CAR\r\n",         /* 9 */
"AT+HOT_CAR\r\n",          /* 10 */

/* BLE-->MCU Status Sets */
"AT+OPEN_SKYLIGHT\r\n",  	 /* 11 */
"AT+CLOSE_SKYLIGHT\r\n", 	 /* 12 */
"AT+OPEN_CARWINDOW\r\n", 	 /* 13 */
"AT+CLOSE_CARWINDOW\r\n",	 /* 14 */
"AT+INCAR\r\n", 					 /* 15 */
"AT+OUTCAR\r\n",					 /* 16 */
};

uint8_t gUartRxData[STRING_UARTRX_LEN];

/*****************************************************************************
** Function:    Task2
** Description: Used to AT+ Mode
** Parameter:   None
** Return:      None
******************************************************************************/
void Task2(void)
{
  uint16_t DataLen;
	char Start[] = "Enter AT+ Mode ...\r\n";
	  /*Hardware Init*/
	R_CLOCK_Init();                       /* Clock initialize    */
	//RLIN3_Init(RLIN3_0);
	RS_CAN_init();                        /* RS-CAN initialize   */
	//LIN1_EN;
	OSTM0_INIT();						  /* OSTM0 initialize    */
	__EI();										/*Open Interrupt*/
	OSTM0_START(); 						/* OSTM0 start    */
	
	R_WDT_Init();
	RLIN33_init();
	R_TAUD_Init();
  R_TAUD_Start();
	BspPinInit();
	
	Uart3SentByDMA(Start,strlen(Start));

	while(1)
	{
		MslLinkRotationTask();   	// Link Bus Task
		BllCANPeriodTask();    		// CAN Period Task
		
		DataLen = Uart3GetData(gUartRxData); // Get Data From Uart
		if(DataLen != 0)
		{	
			DataUnpackHandle(gUartRxData,DataLen);			
		}
	}
}

/*****************************************************************************
** Function:    DataUnpackHandle
** Description: Unpack AT+ Packet as one command and excute the command
** Parameter:   None
** Return:      None
******************************************************************************/
void DataUnpackHandle(uint8_t *Data,uint16_t Len)
{
	uint16_t Temp,ATCmdLen;
	uint16_t UnReadLen = Len;
	uint16_t StartByte = 0,EndByte = 0;
	
	/*Ergodic the data of Uart received */
	while(UnReadLen > 5 )
	{
		for(Temp=EndByte;Temp<Len;Temp++)
		{
			if(Data[Temp]==0x41)  //"A"
			{
				if(Data[Temp+1]==0x54)  //"T"
				{
					if(Data[Temp+2]==0x2B)  //"+"
					{
						StartByte = Temp;
					}
				}
			}
			else if(Data[Temp]==0x0D)  //"\r"
			{
				if(Data[Temp+1]==0x0A)	//"\n"
				{
					EndByte = Temp+1;
					break;
				}
			}
		}
		/*No data need to read */
		if(Temp > Len-2)
		{
			UnReadLen = 0;
		}
		/*Caculate the lenth of AT+ command */
		ATCmdLen = EndByte - StartByte + 1;
		UnReadLen = Len - EndByte - 1;
		/*Excute AT+ command */
		DecodeATCmdTask(&Data[StartByte],ATCmdLen);
	}
	/*Clear RXBuff*/
	memset(gUartRxData,0,STRING_UARTRX_LEN);
}

/*****************************************************************************
** Function:    DecodeATCmdTask
** Description: Excute AT Command
** Parameter:   Data- data point  Len- data lenth
** Return:      decode result
******************************************************************************/
void DecodeATCmdTask(uint8_t* Data,uint16_t Len)
{
	uint8_t Cmd;
	if(Len < 5)
	{
		Cmd = CmdAnalyer(Data,Len);
		if(Cmd < CMD_NUM - 1)
		{
			/* Sent CAN packets*/
			ExcuteATCmdTask(Cmd);
		}
	}
}

/*****************************************************************************
** Function:    CmdAnalyer
** Description: decode the uart command
** Parameter:   Data- data point  Len- data lenth
** Return:      decode result
******************************************************************************/
uint8_t CmdAnalyer(uint8_t* Data,uint16_t Len)
{
	uint8_t Temp;
	uint16_t DataSame;
	
	for(Temp=0;Temp<CMD_NUM;Temp++)
	{
		DataSame = Strcmp(Data,(uint8_t*)&CmdData[Temp][0],strlen(&CmdData[Temp][0]));		
		if(DataSame == 0)
		{
			break;
		}
	}
	Temp++;
	return Temp;
}

/*****************************************************************************
** Function:    RssiAnalyer
** Description: decode the uart Rssi
** Parameter:   Data- data point  Len- data lenth
** Return:      decode result
******************************************************************************/
uint8_t RssiAnalyer(uint8_t* Data,uint16_t Len)
{
	uint8_t RssiValue = 0;
	uint16_t DataSame;
	char Rssi[] = "rssi:-";
			
	DataSame = Strcmp(Data,(uint8_t*)Rssi,strlen(Rssi));		
	if(DataSame == 0)
	{
		RssiValue = Data[strlen(Rssi)];
	}
	return RssiValue;
}

/*****************************************************************************
** Function:    ExcuteATCmdTask
** Description: excute the cmd on the CAN bus
** Parameter:   Cmd- from decode
** Return:      void
******************************************************************************/
void ExcuteATCmdTask(uint8_t Cmd)
{
  switch(Cmd){
	case 1 :
		/*Unlock Door*/
		MslCANCmdExecute(DOOR_UNLOCK );
	break;
	case 2 :
		/*Lock Door*/
	  MslCANCmdExecute(DOOR_LOCK);
	break;
	case 3 :
	  /*Unlock Trunk*/
	  MslCANCmdExecute(TRUNK_UNLOCK);
	break;
	case 4 :
		/*Fire*/
	break;
	case 5 :
		/*Left Door*/
	break;
	case 6 :
		/*Right Door*/
	break;
	case 7 :
		/*Find Car*/
		MslCANCmdExecute(FIND_CAR);
	break;
	/*......*/
	/*......*/
	case 11 :
		/*Open skyLight*/
		MslCANCmdExecute(SKYLIGHT_OPEN);
		__nop();
	break;
	case 12 :
		/*Close skyLight*/
		MslCANCmdExecute(SKYLIGHT_CLOSE);
		__nop();
	break;
	case 13 :
		/*Open Windows*/
		MslCANCmdExecute(WINDOWNS_UP);
		__nop();
	break;
	case 14 :
		/*Close Windows*/
		MslCANCmdExecute(WINDOWNS_DOWN);
		__nop();
	break;
	case 15 :
		/*Car Inside*/
		SetKeyStatus(1);
		__nop();
	break;
	case 16 :
		/*Car Outside*/
		SetKeyStatus(0);
		__nop();
	break;
	default :
	break;
	
	
	}
	
}