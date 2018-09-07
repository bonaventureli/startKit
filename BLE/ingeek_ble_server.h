#ifndef INGEEK_BLE_SERVER_H
#define INGEEK_BLE_SERVER_H


#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
 #include "digital_key_api.h"

 #include <string.h>
/*********************************************************************
 * CONSTANTS
 */
#define SUCCESS 0
// Profile Parameters
#define DKSERVER_CHAR_STATUS		0  // R uint8 - Profile Characteristic 0 value
#define DKSERVER_CHAR_INFO			1  // R uint8 - Profile Characteristic 1 value
#define DKSERVER_CHAR_AUTH			2  // W uint8 - Profile Characteristic 2 value
#define DKSERVER_CHAR_SESSION		3  // W uint8 - Profile Characteristic 3 value
#define DKSERVER_CHAR_CMD			4  // RW uint8 - Profile Characteristic 4 value
#define DKSERVER_CHAR_RSSI			5  // W uint8 - Profile Characteristic 5 value
#define DKSERVER_CHAR_VERSION       6  // RW uint8 - Profile Characteristic 6 value
#define DKSERVER_CHAR_MSG           7  // RW uint8 - Profile Characteristic 7 value
#define DKSERVER_CHAR_RTC           8  // RW uint8 - Profile Characteristic 8 value

// Digital key Service UUID
#define DKSERVER_SERV_UUID          0xFFF0

// Key Pressed UUID
#define DKSERVER_STATUS_UUID           	0xFFF1
//#define DKSERVER_STATUS_UUID           	0xF1FF
#define DKSERVER_INFO_UUID           	0xFFF2
#define DKSERVER_AUTH_UUID           	0xFFF3
#define DKSERVER_SESSION_UUID			0xFFF4
#define DKSERVER_CMD_UUID				0xFFF5
#define DKSERVER_RSSI_UUID				0xFFF6
#define DKSERVER_VERSION_UUID           0xFFF7
#define DKSERVER_MSG_UUID               0xFFF8
#define DKSERVER_RTC_UUID               0xFFF9

// Digital key profile Services bit fields
#define DKSERVER_SERVICE               0x00000001

// Length of info Characteristic in bytes
#define DKSERVER_INFO_LEN           128
// Length of auth Characteristic in bytes
#define DKSERVER_AUTH_LEN          	128
//Length of session Characteristic in bytes
#define DKSERVER_SESSION_LEN       	128
//Length of cmd Characteristic in bytes
#define DKSERVER_CMD_LEN         	20
//Length of rssi Characteristic in bytes
#define DKSERVER_RSSI_LEN        	20
//Length of version Characteristic in bytes
#define DKSERVER_VER_LEN            25
//Length of MSG Characteristic in bytes
#define DKSERVER_MSG_LEN            128
//Length of RTC Characteristic in bytes
#define DKSERVER_RTC_LEN            128

/////////////////////////////
#define SERVAPP_NUM_ATTR_SUPPORTED             32  //22

#define SERVAPPONE             0101
#define SERVAPPTWO             0301

/*********************************************************************
 * TYPEDEFS
 */
 
typedef signed   char      int8_t;
typedef signed   short     int16_t;
typedef signed   long      int32_t;
typedef signed   long long int64_t;

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned long      uint32_t;
typedef unsigned long long uint64_t;


 
 typedef struct {
	uint8_t	Header;
	uint8_t	Address;
	uint8_t	Operate1;
	uint8_t	Operate2;
	uint8_t UUID0;
	uint8_t UUID1;
	uint8_t	Datelength;
	uint8_t	dataload[30];
	uint8_t	FCS0;
	uint8_t	FCS1;	
} T_INTOTOBLE;
 

 typedef struct{
	uint8_t	Header;
	uint8_t	Address;
	uint8_t	Operate1;
	uint8_t	Operate2;
	uint8_t UUID0;
	uint8_t UUID1;
	uint8_t	Datelength;
	uint8_t	dataload[30];
	uint8_t	FCS0;
	uint8_t	FCS1;	
}T_STATUSTOBLE;

 typedef struct{
	uint8_t	Header;
	uint8_t	Address;
	uint8_t	Operate1;
	uint8_t	Operate2;
	uint8_t UUID0;
	uint8_t UUID1;
	uint8_t	Datelength;
	uint8_t	dataload[55];
	uint8_t	FCS0;
	uint8_t	FCS1;	
}T_AUTHTOBLE;

 typedef struct{
	uint8_t	Header;
	uint8_t	Address;
	uint8_t	Operate1;
	uint8_t	Operate2;
	uint8_t UUID0;
	uint8_t UUID1;
	uint8_t	Datelength;
	uint8_t	dataload[113];
	uint8_t	FCS0;
	uint8_t	FCS1;	
}T_RECEIVESESSION;

 typedef struct{
	uint8_t	Header;
	uint8_t	Address;
	uint8_t	Operate1;
	uint8_t	Operate2;
	uint8_t UUID0;
	uint8_t UUID1;
	uint8_t	Datelength;
	uint8_t	dataload[16];
	uint8_t	FCS0;
	uint8_t	FCS1;	
}T_RECEIVECMD;

typedef enum {
    CARINFO_VALID,
    READ_INFO,
    WRITE_AUTH,
    WRITE_SESSION,
    CARINFO_INVALID=0xFF
}MCU_status;

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * API FUNCTIONS
 ********************************************************************/
uint8_t Handle_Character_A( uint8_t *Data, uint32_t frame_len);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* INGEEK_BLE_SERVER_H */
