#ifndef RLIN3_API_H
#define RLIN3_API_H

#include <map_rlin3.h>
#include <rlin3_as.h>
#include <rlin3_as_cfg.h>
#include 	"r_typedefs.h"
#include "iodefine.h"
/******************************************************************************
Macro definitions
******************************************************************************/
#define LF_ANT_NAD   0x11
#define RF_ANT_NAD   0x12
#define LR_ANT_NAD   0x13
#define RR_ANT_NAD   0x14

#define LIN1_EN  PM0 &= ~(1<<11);\
				 P0  |=  (1<<11)

#define LIN2_EN  PM8 &= ~(1<<6);\
				 P8  |=  (1<<6)


extern uint8_t Lin_Tx_Buffer[8],Lin_Rx_Buffer[8];

extern Lin_StatusType CurrentState[ RLIN3_MAXMACROS ];

extern Lin_FramePidType Lin_NAD_Info[4];  

extern Lin_PduType LF_ANT_Pdu_Info;  //NAD 0x11  PID 0x11
extern Lin_PduType RF_ANT_Pdu_Info;  //NAD 0x12  PID 0x92
extern Lin_PduType LR_ANT_Pdu_Info;  //NAD 0x13  PID 0xD3
extern Lin_PduType RR_ANT_Pdu_Info;  //NAD 0x14  PID 0x14




void RLIN3_Init (uint8_t Channel);
void RLIN3_Rx_Callback(uint8_t Channel, uint8 *LinSduPtr);
void RLIN3_Tx_Frame(uint8_t Channel, Lin_FramePidType LIN_NAD_Info, uint8_t* LIN_Tx_Buffer );



#endif 


