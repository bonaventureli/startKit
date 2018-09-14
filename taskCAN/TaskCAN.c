#include "TaskCAN.h"

void TaskCAN(void)
{
	int i;
	R_CLOCK_Init();                       /* Clock initialize    */
	HAL_RS_CAN_initB();                       /* RS-CAN initialize   */
	
	__EI();																/*Open Interrupt*/
	
	while(1)
	{
		//MslCANDemoOneFrame();
		MslCANRxTxDemo(); 					// Tx Rx Demo, 3 channals can be used.
		//MslCANPeriodTask();    		// CAN Period Task
		//MslCANSentFromSDK(1);
	}
}
