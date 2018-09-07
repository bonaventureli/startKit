//============================================================================
//
// Header file for the UART source code
//
//============================================================================
#ifndef _UART_33_H
#define _UART_33_H
#include "r_typedefs.h"

#define UART3_BUFF_LEN   	 200
#define UART3_IDLE_TIMEOUT 0x01

//============================================================================
// Defines
//============================================================================
enum RLIN33_status
{
    RLIN33_busy,
    RLIN33_ready
};

//============================================================================
// Function defines
//============================================================================
void RLIN33_init(void);
void RLIN33_send_string(char send_string[] );
void Uart3Sent(uint8_t *Data,uint8_t Len);

void IntUart3Callback(uint32_t regEIIC_value);
void ISR_Uart3Callback(void);
void Uart3DataSaveCache(void);

uint16_t Uart3GetData(uint8_t* Data);

#endif
