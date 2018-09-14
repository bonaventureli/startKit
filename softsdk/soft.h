#ifndef _SOFT_H_
#define _SOFT_H_

#include "r_typedefs.h"
#include "UserSys.h"
#if 0
typedef signed   char      int8_t;
typedef signed   short     int16_t;
typedef signed   long      int32_t;
typedef signed   long long int64_t;

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned long      uint32_t;
typedef unsigned long long uint64_t;
#endif

#define CMD_NUM   16
#define CMD_LEN   30

const char CmdDataAA[CMD_NUM][CMD_LEN]={
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

#define UART_HEADER     0x7E
#define UART_BLETOMCU   0x01
#define UART_MCUTOBLE   0x10

#define UART_OPERATE_BLE_BROADCAST      0x0101
#define UART_OPERATE_BLE_CONNECT        0x0102
#define UART_OPERATE_BLE_UNCONNECT      0x0103
#define UART_OPERATE_CHARACTER_NOTIFY   0x0210

#define UART_SEND_OPERATE_CHARACTER_READ		0x0104
#define UART_RECEIVE_OPERATE_CHARACTER_READ		0x0204

#define UART_SEND_OPERATE_CHARACTER_WRITE		0x0108
#define UART_RECEIVE_OPERATE_CHARACTER_WRITE		0x0208

#define UART_SEND_OPERATE_CHARACTER_NOTIFY		0x0110
#define UART_RECEIVE_OPERATE_CHARACTER_NOTIFY		0x0210


#define INFO_UUID       0xF2FF
#define AUTH_UUID       0xF3FF
#define SESSION_UUID    0xF4FF
#define CMD_UUID        0xF5FF
#define CARSTATE_UUID        0xF6FF

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

#define UART_REC_DATA_LEN 200

 typedef struct{
	uint8_t	Header;
	uint8_t	Address;
	uint16_t Operate;
	uint16_t UUID;
	uint8_t	Datalength;
	uint8_t	*dataload;
	uint16_t	FCS;
	
	uint8_t	dataload_len;
}T_UartFrame;
uint16_t SF_Strcmp(uint8_t *Data1,uint8_t *Data2,uint16_t Len);
uint16_t SF_CaculateDataLen(uint8_t *Data,uint16_t Len);
void SF_DataUnpackHandle(uint8_t *Data,uint16_t Len);
void SF_DecodeATCmdTask(uint8_t* Data,uint16_t Len);
uint8_t SF_CmdAnalyer(uint8_t* Data,uint16_t Len);
void SF_ExcuteATCmdTask(uint8_t Cmd);
void SF_MslCANCmdExecute(uint8_t Function);
void SF_UARTPotocolAnalyzer(uint8_t *Data, uint16_t DataLen);

uint8_t USR_Handle_Character(uint8_t *Data, uint32_t frame_len);
void USR_Handle_info(uint8_t *data, uint32_t data_len);
void USR_Handle_auth(uint8_t *data, uint32_t data_len);
void USR_Handle_session(uint8_t *data, uint32_t data_len);
void USR_Handle_cmd(uint8_t *data, uint32_t data_len);
void USR_Handle_active();
void USR_Handle_disconnect();
void USR_Handle_broadcast();
#endif
