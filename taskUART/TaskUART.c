#include "TaskUART.h"

#define UART_REC_DATA_LEN 200
uint8_t gUartDatabufferTaskUART[UART_REC_DATA_LEN];

void TaskUART(void)
{
	uint8_t tmpdate=0x12;
	#if DRIVERUART0
	uint8_t gUart0FrameLen;
	#elif DRIVERUART3
	uint8_t gUart3FrameLen;
	#endif
	
	R_CLOCK_Init();                       /* Clock initialize    */
	
	
	RLIN30_init();
	RLIN33_init();
	R_TAUD_Init();
  R_TAUD_Start();
	__EI();										/*Open Interrupt*/

	while(1)
	{
	#if DRIVERUART0
	gUart0FrameLen = Uart0GetData(gUartDatabufferTaskUART);
	#elif DRIVERUART3
	gUart3FrameLen = Uart3GetData(gUartDatabufferTaskUART);
	#endif

	#if DRIVERUART0
	if(gUart0FrameLen ){
		Uart0Sent(gUartDatabufferTaskUART,gUart0FrameLen);
	}
	#elif DRIVERUART3
	
		if(gUart3FrameLen ){
		Uart3Sent(&tmpdate,1);
		Uart3Sent(gUartDatabufferTaskUART,gUart3FrameLen);
	}
	#endif
	}
}
