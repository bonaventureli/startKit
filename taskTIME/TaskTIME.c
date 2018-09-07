#include "TaskTIME.h"

#define UART_REC_DATA_LEN 200
uint8_t gUartDatabuffer[UART_REC_DATA_LEN];

void TaskTIME(void)
{

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
	gUart0FrameLen = Uart0GetData(gUartDatabuffer);
	#elif DRIVERUART3
	gUart3FrameLen = Uart3GetData(gUartDatabuffer);
	#endif

	#if DRIVERUART0
	if(gUart0FrameLen ){
		Uart0Sent(gUartDatabuffer,gUart0FrameLen);
	}
	#elif DRIVERUART3
		if(gUart3FrameLen ){
		Uart3Sent(gUartDatabuffer,gUart3FrameLen);
	}
	#endif
	}
}
