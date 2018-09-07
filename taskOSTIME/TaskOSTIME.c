#include "TaskOSTIME.h"

#define UART_REC_DATA_LEN 200
uint8_t gUartDatabufferTaskOSTIME[UART_REC_DATA_LEN];

void TaskOSTIME(void)
{

	#if DRIVERUART0
	uint8_t gUart0FrameLen;
	#elif DRIVERUART3
	uint8_t gUart3FrameLen;
	#endif
	
	R_CLOCK_Init();                       /* Clock initialize    */
	
	
	RLIN30_init();
	RLIN33_init();

	OSTM0_INIT();						  /* OSTM0 initialize    */
	__EI();										/*Open Interrupt*/
	OSTM0_START(); 						/* OSTM0 start    */
	
	
	R_TAUD_Init();
  R_TAUD_Start();
	__EI();										/*Open Interrupt*/

	while(1)
	{
	#if DRIVERUART0
	gUart0FrameLen = Uart0GetData(gUartDatabufferTaskOSTIME);
	#elif DRIVERUART3
	gUart3FrameLen = Uart3GetData(gUartDatabufferTaskOSTIME);
	#endif

	#if DRIVERUART0
	if(gUart0FrameLen ){
		Uart0Sent(gUartDatabufferTaskOSTIME,gUart0FrameLen);
	}
	#elif DRIVERUART3
		if(gUart3FrameLen ){
		Uart3Sent(gUartDatabufferTaskOSTIME,gUart3FrameLen);
	}
	#endif
	}
}
