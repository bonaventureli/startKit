#include "soft.h"

/*****************************************************************************
** Function:    SF_Strcmp
** Description: compare data1 and data2
** Parameter:   Data1 Data2 Len
** Return:      equal return 0 alse 1
** Example:
******************************************************************************/
uint16_t SF_Strcmp(uint8_t *Data1,uint8_t *Data2,uint16_t Len)
{
	uint16_t temp;
	for(temp = 0;temp < Len; temp++)
	{
		if(Data1[temp] != Data2[temp])
		{
			return 1;
		}
	}
	return 0;
}

/*****************************************************************************
** Function:    SF_CaculateDataLen
** Description: Used to Caculate the len of "0x0D,0x0A"  "AT+BCD\r\n"
** Parameter:   Data  Len
** Return:      return string len
** Example:
** "AT+BCD\r\n"
** return 8
******************************************************************************/
uint16_t SF_CaculateDataLen(uint8_t *Data,uint16_t Len)
{
  uint16_t Shift;
  uint16_t Temp;
  
  for(Temp=0;Temp<Len;Temp++)
  {
      if(Data[Temp] == 0x0D)
      {
        if(Data[Temp+1] == 0x0A)
        {
          Shift = Temp+2;
          break;
        }
      }
      else
      {
        
      }  
  }
	return Shift;
}

/*****************************************************************************
** Function:    SF_DataUnpackHandle
** Description: Unpack AT+ Packet as one command and excute the command
** Parameter:   None
** Return:      None
** Example:
** GDKALJGALJAT+LOCK\r\nDAKFJDLA
** After SF_DataUnpackHandle
** AT+LOCK\r\n
******************************************************************************/
void SF_DataUnpackHandle(uint8_t *Data,uint16_t Len)
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
						SF_DecodeATCmdTask(&Data[StartByte],ATCmdLen);
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
	//memset(gUartRxData,0,STRING_UARTRX_LEN);
}
/*****************************************************************************
** Function:    DecodeATCmdTask
** Description: Excute AT Command
** Parameter:   Data- data point  Len- data lenth
** Return:      decode result
** Example:
** AT+LOCK\r\n ->2	
** excute CAN Command 2
******************************************************************************/
void SF_DecodeATCmdTask(uint8_t* Data,uint16_t Len)
{
	uint8_t Cmd;
	if(Len > 5)
	{
		Cmd = CmdAnalyer(Data,Len);
		if(Cmd < CMD_NUM + 1)
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
** Example:
** AT+COOL_CAR\r\n  output 9
** AT+LOCK\r\n 		output 2
******************************************************************************/

//uint8_t SF_CmdAnalyer(uint8_t* Data,uint16_t Len)
//{
//	uint8_t Temp;
//	uint16_t DataSame;
	
//	for(Temp=0;Temp<CMD_NUM;Temp++)
//	{
//		DataSame = Strcmp(Data,(uint8_t*)&CmdData[Temp][0],strlen(&CmdData[Temp][0]));		
//		if(DataSame == 0)
//		{
//			break;
//		}
//	}
//	Temp++;
//	return Temp;
//}

uint8_t SF_CmdAnalyer1(uint8_t* Data,uint8_t** CmdDataA, uint16_t Len)
{
	uint8_t Temp;
	uint16_t DataSame;
	
	for(Temp=0;Temp<CMD_NUM;Temp++)
	{
		DataSame = Strcmp(Data,CmdDataA[Temp][0],strlen(&CmdDataA[Temp][0]));		
		if(DataSame == 0)
		{
			break;
		}
	}
	Temp++;
	return Temp;
}
/*****************************************************************************
** Function:    ExcuteATCmdTask
** Description: excute the cmd on the CAN bus
** Parameter:   Cmd- from decode
** Return:      void
** Example:
** 1
** 2
** 3
** 4
******************************************************************************/

void SF_ExcuteATCmdTask(uint8_t Cmd)
{
  switch(Cmd){
	case 1 :
		/*Unlock Door*/
		SF_MslCANCmdExecute(DOOR_UNLOCK );
	break;
	case 2 :
		/*Lock Door*/
	  SF_MslCANCmdExecute(DOOR_LOCK);
	break;
	
	default :
	break;
	
	
	}
	
}

/******************************************************************************
* Function Name : MslCANCmdExecute
* Description   : CAN Bus sent
* Argument      : CANData sent data;
* Return Value  : none
******************************************************************************/
void SF_MslCANCmdExecute(uint8_t Function)
{

}
/*****************************************************************************
** Function:    SF_UARTPotocolAnalyzer
** Description: Used to Checkout the Header of a frame
** Parameter:   Data point,Data lenth
** Return:      None
** Example:
** FOIUF8138FDSIAOJFDSAJIF78RQOUGIDJALFD94JFDA7E0110FFF10102FF
** 7E0110FFF10102FF
******************************************************************************/

void SF_UARTPotocolAnalyzer(uint8_t *Data, uint16_t DataLen)
{
	uint8_t *pData,Temp;
	if(DataLen > UART_REC_DATA_LEN) /*Exceed the limit lenth of a frame*/
	return;
	/*checkout the header*/
	for(Temp=0;Temp<DataLen;Temp++)
	{
		if(Data[Temp] == 0x7E)
		{
			if(Data[Temp+1] == 0x01)break;
		}
	}
	pData = &Data[Temp];

	if(DataLen-Temp>5)
	{
		USR_Handle_Character(pData, DataLen-Temp);
	}
}

/*****************************************************************************
** Function:    USR_Handle_Character
** Description: USR_Handle_Character
** Parameter:   Data frame_len
** Return:      None
** Example:
******************************************************************************/
uint8_t USR_Handle_Character( uint8_t *Data, uint32_t frame_len)
{
	T_UartFrame *Framedata;
	
	Framedata = (T_UartFrame *) Data;
	

	if(Framedata->Header == UART_HEADER){
		if(Framedata->Address == UART_BLETOMCU){
		  
			switch (Framedata->Operate){
				case UART_OPERATE_BLE_BROADCAST:{
					 USR_Handle_broadcast();
					 break;
				}
				case UART_OPERATE_BLE_UNCONNECT:{
					USR_Handle_disconnect();
					break;
				}
				case UART_OPERATE_CHARACTER_NOTIFY:{
					switch (Framedata->UUID){
						case INFO_UUID:
						{
							USR_Handle_info(&Data[7],0x48);
							break;
						}
							case AUTH_UUID:
						{
							USR_Handle_auth(&Data[7],0x37);
							break;
						}
						case SESSION_UUID:
						{
							USR_Handle_session(&Data[7],0x70);
							break;
						}
						case CMD_UUID:
						{
							USR_Handle_cmd(&Data[7],0x10);
							break;
						}
						default:
						break;
						}
					break;
					}
			}
		}
	}
return 0;	
}
void USR_Handle_info(uint8_t *data, uint32_t data_len){
	
}
void USR_Handle_auth(uint8_t *data, uint32_t data_len){
	
}
void USR_Handle_session(uint8_t *data, uint32_t data_len){
	
}
void USR_Handle_cmd(uint8_t *data, uint32_t data_len){
	
}
void USR_Handle_active(){
	
}
void USR_Handle_disconnect(){
	
}
void USR_Handle_broadcast(){
	
}