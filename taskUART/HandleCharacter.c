/*Header	Address	Operate	UUID	Datalength	dataload	FCS
 * 1Byte	1Byte	2Byte	2Byte	1Byte		xxxByte		2Byte
 * 0x7E	
 * Address	ble->mcu 0x01
 * */
#define UART_HEADER	0x7E
#define UART_BLETOMCU	0x01
#define UART_MCUTOBLE	0x10
#define UART_OPERATE_BLE_BROADCAST	0x0101
#define UART_OPERATE_BLE_CONNECT	0x0201
#define UART_OPERATE_BLE_UNCONNECT	0x0301
#define UART_OPERATE_CHARACTER_NOTIFY	0x1002
#define INFO_UUID	0xFFF2
#define AUTH_UUID	0xFFF3
#define SESSION_UUID	0xFFF4
#define CMD_UUID	0xFFF5
typedef struct{
	uint8_t  Header;
	uint8_t  Address;
	uint16_t Operate;
	uint16_t UUID;
	uint8_t  Datalength;
	uint8_t  *pdataload;
	uint32_t dataload_len;
	uint16_t FCS;
} T_UartFrame;

uint8_t dataload[200];

uint8_t Handle_Character(uint8_t *Data, uint32_t frame_len){

	T_UartFrame *Framedata;
	Framedata->pdataload = dataload;
	Framedata = (T_UartFrame *) Data;

	if(Framedata->Header == UART_HEADER){
	if(Framedata->Address == UART_BLETOMCU){
	switch (Framedata->Operate){
		case UART_OPERATE_CHARACTER_NOTIFY:{
							
							switch (Framedata->UUID){
								case INFO_UUID:{
										       Framedata->dataload_len = 30;
										       Handle_info(Framedata->pdataload,Framedata->dataload_len);
										       break;
									       }
								case AUTH_UUID:{
										       Framedata->dataload_len = 55;
										       Handle_auth(Framedata->pdataload,Framedata->dataload_len);
										       break;
									       }
								case SESSION_UUID:{
										       Framedata->dataload_len = 113;
										       Handle_session(Framedata->pdataload,Framedata->dataload_len);
										       break;
									       }
								case CMD_UUID:{
										       Framedata->dataload_len = 16;
										       Handle_cmd(Framedata->pdataload,Framedata->dataload_len);
										       break;
									       }
								default:break;
							}
							break;
						   }	 
		case UART_OPERATE_BLE_UNCONNECT:{
							Handle_ble_unconnect();
							   break;
						   }
		case UART_OPERATE_BLE_CONNECT:{
						      Handle_ble_connect();
							   break;
						   }
		case UART_OPERATE_BLE_BROADCAST:{
							Handle_ble_broadcast();
							   break;
						   }
		default:break;
	}
	}
	}
}

void Handle_info(uint8_t *pdata,uint32_t loadlen);
void Handle_auth(uint8_t *pdata,uint32_t loadlen);
void Handle_session(uint8_t *pdata,uint32_t loadlen);
void Handle_cmd(uint8_t *pdata,uint32_t loadlen);
void Handle_ble_unconnect(void);
void Handle_ble_connect(void);
void Handle_ble_broadcast(void);
