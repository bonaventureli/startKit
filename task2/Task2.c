#include "Task2.h"

#include "Bllcan.h"
#include "Mslcan.h"
#include "MslATMode.h"
#include "UserSys.h"
#include "Uart33.h"
#include "ostm0.h"
#include "Kit1.h"
#include "ostm0.h"
#include "LinTask.h"
#include "BspWatchDog.h"

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
	uint16_t Temp,IsHeader;
	volatile uint16_t ATCmdLen,UnReadLen = Len;
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
						IsHeader = 1;
						__nop();
					}
				}
			}
			else if(Data[Temp]==0x0D)  //"\r"
			{
				if(Data[Temp+1]==0x0A)	//"\n"
				{
					EndByte = Temp+1;
					if(IsHeader)
					{
						IsHeader = 0;
						/*Caculate the lenth of AT+ command */
						ATCmdLen = EndByte - StartByte + 1;	
						/*Excute AT+ command */
						DecodeATCmdTask(&Data[StartByte],ATCmdLen);
						break;
					}
					else
					{
						__nop();
					}
				}
			}
		}
		UnReadLen = Len - StartByte - ATCmdLen;
		
		/*No data need to read */
		if(Temp > Len-2)
		{
			UnReadLen = 0;
		}
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
	if(Len > 5)
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
** Function:    ExcuteATCmdTask
** Description: excute the cmd on the CAN bus
** Parameter:   Cmd- from decode
** Return:      void
******************************************************************************/
void ExcuteATCmdTask(uint8_t Cmd)
{
  MslSetStatusSentTimes(2);
	
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
		MslCANCmdExecute(WINDOWNS_DOWN);
		__nop();
	break;
	case 14 :
		/*Close Windows*/
		MslCANCmdExecute(WINDOWNS_UP);
		__nop();
	break;
	case 15 :
		/*Car Inside*/
		SetKeyStatus(1);
		if(GetFireButtom() == 1)
		{
			SetFireButtom(0);
			MslCANCmdExecute(ENGINE_START);
		}
	break;
	case 16 :
		/*Car Outside*/
		SetKeyStatus(0);
		__nop();
	break;
	case 17 :
		/*Welcome*/
		MslCANCmdExecute(WELCOME);
	break;
	case 18 :
		/*pass verify*/
	break;
	
	default :
	break;
	}
	
}