/*********************************************************************
 * @fn      
 *
 * @brief   Process a pending Simple Profile characteristic value change
 *          event.
 *
 * @param   
 *
 * @return  None.
 */
 
#include "ingeek_ble_server.h"
#include "rscan.h"

uint8_t gSendata[200] = {0};
uint8_t gmemorySendata[200] = {0};
uint8_t ret_ingeek[10];
/* Create Can_FrameType for send and receive data */
    const Can_FrameType CANOPenDoor={
    //CiTBpA
    0x18,
    0,
    0,
    0,        
    
    //CiTBpB
    0x0000,                            
    0x000,                            
    0x8,    
    
    {
    0x12,                            //DB0
    0x34,                            //DB1
    0x56,                            //DB2
    0x78,                            //DB3
    //CiTBpD
    0x87,                            //DB4
    0x65,                            //DB5
    0x43,                            //DB6
    0x21                             //DB7
    }
    };
const Can_FrameType CANCloseDoor={
    //CiTBpA
    0x18,
    0,
    0,
    0,        
    
    //CiTBpB
    0x0000,                            
    0x000,                            
    0x8,    
    
    {
    0x12,                            //DB0
    0x34,                            //DB1
    0x56,                            //DB2
    0x78,                            //DB3
    //CiTBpD
    0x87,                            //DB4
    0x65,                            //DB5
    0x43,                            //DB6
    0x21                             //DB7
    }
    };
		
/*
* Function:    Handle_Character_A
* Description:        
* Parameter:   Data  frame_len
* Return:      uint8_t
* auther: lifei 
* change time£º2018/8/31
*/
uint8_t Handle_Character_A( uint8_t *Data, uint32_t frame_len)
{
		
	uint8_t SendSTATUSData[11] = {0x7E,0x10,0x10,0x02,0xFF,0xF1,0x01,0x02,0xFF,0xFF};	
	uint8_t sec_status=0;
	uint8_t send_status;
	uint8_t HEAD;
	uint8_t Address;
	uint16_t uuidID[1];
	uint16_t OperateID[1];
	uint32_t ret=0;
	
	unsigned int outlen;
	
	HEAD = Data[0];
	

	if (HEAD == 0x7E){
		Address = Data[1];
		if (Address == 0x01){
		OperateID[0] = ((uint16_t)Data[2])<<8;
		OperateID[0] |= (uint16_t)Data[3];
			if(OperateID[0] == 0x0101){
				ret = ingeek_get_sec_status();
				sec_status = (uint8_t)ret;
				if(sec_status == 255){
					//Uart3Sent(&send_status,1);
					return 21;
				}

				if(((sec_status > 0)&&(sec_status < 255))||(sec_status == 0)){
					T_INTOTOBLE *memorySendINFOData;
					uint8_t reply_data[40];
					
					ingeek_se_final();
					ingeek_se_init();

				
					memorySendINFOData = (T_INTOTOBLE*)gmemorySendata;
					ret = ingeek_pull_info(&reply_data[0], &outlen);
					
					if(ret == SUCCESS){
					memorySendINFOData->Header = 0x7E;
					memorySendINFOData->Address = 0x10;
					memorySendINFOData->Operate1 = 0x10;
					memorySendINFOData->Operate2 = 0x02;
					memorySendINFOData->UUID0 = 0xFF;
					memorySendINFOData->UUID1 = 0xF2;
					memorySendINFOData->Datelength = (uint8_t)outlen+1;
					memorySendINFOData->dataload[0] = 0x00;
					if((outlen == 29) ){
					memcpy(&(memorySendINFOData->dataload[1]),reply_data,outlen);
					}
					else{
						return 6;
					}
					memorySendINFOData->FCS0 = 0xFF;
					memorySendINFOData->FCS1 = 0xFF;
					
					Uart3Sent(memorySendINFOData,(memorySendINFOData->Datelength)+9);
					}
				}
			 }
			else if(OperateID[0] == 0x0301){
				ingeek_se_final();
				ingeek_se_init();
			}	
			else if(OperateID[0] == 0x1002){
			uuidID[0] = ((uint16_t)Data[4])<<8;
			uuidID[0] |= (uint16_t)Data[5];
			switch(uuidID[0]){
				case DKSERVER_INFO_UUID:
				{
				T_INTOTOBLE *INFOData;	
				T_INTOTOBLE *SendINFOData;
				SendINFOData = (T_INTOTOBLE*)gSendata;
				INFOData = (T_INTOTOBLE*)Data;

				if(INFOData->Datelength == 0x48){
				ret = ingeek_push_info(INFOData->dataload, INFOData->Datelength);//0
				if(ret == SUCCESS){
					uint8_t reply_data[40];
					if(ingeek_get_sec_status() == CARINFO_VALID){
					ret = ingeek_pull_info(&reply_data[0], &outlen);
					if(ret == SUCCESS){
					SendINFOData->Header = 0x7E;
					SendINFOData->Address = 0x10;
					SendINFOData->Operate1 = 0x10;
					SendINFOData->Operate2 = 0x02;
					SendINFOData->UUID0 = 0xFF;
					SendINFOData->UUID1 = 0xF2;
					SendINFOData->Datelength = (uint8_t)outlen+1;
					SendINFOData->dataload[0] = 0x00;
					if((outlen == 29) ){
					memcpy(&(SendINFOData->dataload[1]),reply_data,outlen);
					}
					else{
						return 6;
					}
					SendINFOData->FCS0 = 0xFF;
					SendINFOData->FCS1 = 0xFF;
					}
					else{
						return 8;
					}
					if(ingeek_get_sec_status() == READ_INFO){
						Uart3Sent(SendINFOData,(SendINFOData->Datelength)+9);	
					}
					}
				  }
				  else{
					  return 7;
				  }
				}
				else{
					return 5;
				}
				
				break;
			}
				case DKSERVER_AUTH_UUID:
				{
				T_AUTHTOBLE *AUTHData;
				
				AUTHData = (T_AUTHTOBLE*)Data;

				if(ingeek_get_sec_status() == READ_INFO){
					if(AUTHData->Datelength == 55){
						ret = ingeek_push_auth(AUTHData->dataload, AUTHData->Datelength, (unsigned char*)1, (unsigned int*)1);
						if(ret == 0x0000){
							Uart3Sent(SendSTATUSData,10);
						}
						else{
							return 12;
						}
					}
					else{
						return 11;
					}			
				}
				else{
					return 10;
				}
				}
				break;
				
				case DKSERVER_SESSION_UUID:
				{
				T_RECEIVESESSION *RecevieSESSIONData;
				T_RECEIVESESSION *SendSESSIONData;
				uint8_t reply_data[200];//112
				ret = ingeek_get_sec_status();//0x03
				ret_ingeek[3] = (uint8_t)ret;
				SendSESSIONData = (T_RECEIVESESSION*)gSendata;
				RecevieSESSIONData = (T_RECEIVESESSION*)Data;
				if(ingeek_get_sec_status() == WRITE_AUTH){
					if(RecevieSESSIONData->Datelength == 112){
						ret = ingeek_push_session(RecevieSESSIONData->dataload, RecevieSESSIONData->Datelength, reply_data, &outlen);
						if(ingeek_get_sec_status() == WRITE_SESSION){
				 SendSESSIONData->Header = 0x7E;
				 SendSESSIONData->Address = 0x10;
				 SendSESSIONData->Operate1 = 0x10;
				 SendSESSIONData->Operate2 = 0x02;
				 SendSESSIONData->UUID0 = 0xFF;
				 SendSESSIONData->UUID1 = 0xF4;
				 SendSESSIONData->Datelength = (uint8_t)outlen+1;
				 SendSESSIONData->dataload[0] = 0x03;
				 if(outlen == 112){
				 memcpy(&(SendSESSIONData->dataload[1]),reply_data,outlen);
				 }
				 else{
					 return 16;
				 }
				 SendSESSIONData->FCS0 = 0xFF;
				 SendSESSIONData->FCS1 = 0xFF;
				 Uart3Sent(SendSESSIONData,SendSESSIONData->Datelength+9);
				 }
				 else{
					 return 15;
				 }
					}
					else{
						return 14;
					}
					
				}
				else{
					return 13;
				}

				}
				break;
				
				case DKSERVER_CMD_UUID:
				{
				T_RECEIVECMD *RecevieCMDData;
				T_RECEIVECMD *SendCMDData;
				uint8_t reply_data[200];//112
				DK_Cmd_Meg struct_cmd;
				uint8_t cmd;
				SendCMDData = (T_RECEIVECMD*)gSendata;
				RecevieCMDData = (T_RECEIVECMD*)Data;
				if(RecevieCMDData->Datelength == 16){
				ret = ingeek_command_input_action(RecevieCMDData->dataload, RecevieCMDData->Datelength, &struct_cmd);
				if(ret == 0x0000){
				cmd = (uint8_t)(struct_cmd.command);
				Uart3Sent(&cmd,1);  
				MslCANSentFromSDK(cmd);	
				
				ret = ingeek_command_output_action(&struct_cmd,reply_data, &outlen);
				if(ret == SUCCESS){
				SendCMDData->Header = 0x7E;
				SendCMDData->Address = 0x10;
				SendCMDData->Operate1 = 0x10;
				SendCMDData->Operate2 = 0x02;
				SendCMDData->UUID0 = 0xFF;
				SendCMDData->UUID1 = 0xF5;
				SendCMDData->Datelength = outlen;
				if(outlen == 16){
				memcpy(SendCMDData->dataload,reply_data,outlen);
				}
				else{
					return 20;
				}
				SendCMDData->FCS0 = 0xFF;
				SendCMDData->FCS1 = 0xFF;
				Uart3Sent(SendCMDData,SendCMDData->Datelength+9); 
				}
				else{
					return 19;
				}
				
				}
				else{
					return 18;
				}
				
				}
				else{
					return 17;
				}
				
				
				}
				break;

				default:
				break;
				}
				}
				else{
				return 3;
			}
			}
			else{
				return 2;
			}
	}
	else{
		return 1;
	}
return 0;	
}


