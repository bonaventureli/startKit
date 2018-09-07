#include "rlin3_api.h"


uint8_t Lin_Tx_Buffer[8],Lin_Rx_Buffer[8];

int Lin_LF_ANT_Rx_Count,Lin_RF_ANT_Rx_Count,Lin_LR_ANT_Rx_Count,Lin_RR_ANT_Rx_Count;

Lin_FramePidType Lin_NAD_Info[4]={LF_ANT_NAD,RF_ANT_NAD,LR_ANT_NAD,RR_ANT_NAD};  

Lin_PduType LF_ANT_Pdu_Info;  //NAD 0x11  PID 0x11
Lin_PduType RF_ANT_Pdu_Info;  //NAD 0x12  PID 0x92
Lin_PduType LR_ANT_Pdu_Info;  //NAD 0x13  PID 0xD3
Lin_PduType RR_ANT_Pdu_Info;  //NAD 0x14  PID 0x14

Lin_ConfigType Lin_config;
Lin_UnitConfigType  LIN_Unit[RLIN3_MAXMACROS];

void RLIN3_Init (uint8_t Channel)
{
	Lin_config.Unit[Channel] 						= &LIN_Unit[Channel];
	Lin_config.Unit[Channel]->UnitNumber_u08 		= Channel;
	Lin_config.Unit[Channel]->LINMode_u08    		= RLIN3_MODE_LINMASTER;
    Lin_config.Unit[Channel]->LINStandard_u08		= RLIN3_LINSTANDARD_V2;
	Lin_config.Unit[Channel]->Baudrate_u32			= 19200;
	Lin_config.Unit[Channel]->SamplesPerBit_u08		= RLIN3_SAMPLES_MASTER; 
	Lin_config.Unit[Channel]->ClockSelection_u08	= RLIN3_CLOCK_FA;		
	Lin_config.Unit[Channel]->WakeupLength_u08      = RLIN3_WUPTL_16BITS;	
	Lin_config.Unit[Channel]->BreakLength_u08		= RLIN3_SBF13BITS;
	Lin_config.Unit[Channel]->BreakDelimiter_u08    = RLIN3_BDT1BITS;
	Lin_config.Unit[Channel]->HeaderSpace_u08       = RLIN3_SPACE_HDRRESP_1BITS;
	Lin_config.Unit[Channel]->InterByteSpace_u08	= RLIN3_SPACE_RESBYTE_1BITS;
	Lin_config.Unit[Channel]->InterruptMode_u08		= RLIN3_TRIPLE_INT;   

	Lin_Init(&Lin_config);
}

void RLIN3_Tx_Frame(uint8_t Channel, Lin_FramePidType LIN_NAD_Info, uint8_t* LIN_Tx_Buffer )
{
		LF_ANT_Pdu_Info.Pid   = LIN_NAD_Info;
		LF_ANT_Pdu_Info.Cs    = LIN_ENHANCED_CS;
		LF_ANT_Pdu_Info.Drc   = LIN_SLAVE_RESPONSE;
		LF_ANT_Pdu_Info.Dl    = 0x08;
		LF_ANT_Pdu_Info.SduPtr= Lin_Tx_Buffer;
		Lin_SendFrame(RLIN3_0,&LF_ANT_Pdu_Info);
}

void RLIN3_Rx_Callback(uint8_t Channel, uint8 *LinSduPtr)
{


	Lin_GetFrame(Channel,0x08,LIN_ENHANCED_CS);
	Lin_GetStatus(Channel,LinSduPtr);
	Lin_LF_ANT_Rx_Count++;

	if (Lin_LF_ANT_Rx_Count>10000)
		Lin_LF_ANT_Rx_Count=0;

}







