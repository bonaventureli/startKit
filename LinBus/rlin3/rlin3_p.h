#ifndef _RLIN3_UCIAPRLN_P_H
#define _RLIN3_UCIAPRLN_P_H
                   
#include <ree_types.h>

extern bit
RLIN3_PortEnable( u08 UnitNumber_u08 );

extern bit
RLIN3_PortDisable( u08 UnitNumber_u08 );

extern bit
RLIN3_Reset( u08 UnitNumber_u08 );

extern bit
RLIN3_UARTModeConfig( u08 UnitNumber_u08,
                          flt Oscillator_Frequency_flt,
                          u32 Baudrate_u32,
                          u08 SamplesPerBit_u08,
                          u08 Parity_u08,
                          u08 CharLen_u08,
                          u08 StopBits_u08,
                          u08 Direction_u08,
                          u08 FilterMode_u08,
                          u08 InterruptMode_u08 );

extern bit 
RLIN3_LegacyConfig( u08 UnitNumber_u08,
                        flt Oscillator_Frequency_flt,
                        u32 Baudrate_u32,
                        u08 Parity_u08,
                        u08 CharLen_u08,
                        u08 StopBits_u08,
                        u08 Direction_u08 );

extern bit
RLIN3_GetUARTStatus( u08  UnitNumber_u08,
                         pu08 BitError_pu08,
                         pu08 ParityError_pu08,
                         pu08 FrameError_pu08,
                         pu08 OverrunError_pu08,
                         pu08 SendFlag_pu08 );

extern bit
RLIN3_SendByte( u08 UnitNumber_u08,
                u08 SendByte_u08 );

extern bit
RLIN3_ReceiveByte( u08  UnitNumber_u08,
                   pu08 ReceiveByte_pu08 );

extern bit
RLIN3_CreateInterrupt( u08 UnitNumber_u08,
                       u08 InterruptSource_u08,
                       u08 InterruptLevel_u08,
                       void ( *FunctionVector )( ) );

extern 
bit RLIN3_LINSetConfig( u08 UnitNumber_u08,
                        flt Oscillator_Frequency_flt,
                        u08 OperationMode_u08,
                        u08 LINStandard_u08,
                        u32 Baudrate_u32,
                        u08 SamplesPerBit_u08,
                        u08 ClockSelection_u08,
                        u08 BreakDetectionLength_u08,
                        u08 BreakLength_u08,
                        u08 BreakDelimiter_u08,
                        u08 HeaderSpace_u08,
                        u08 InterByteSpace_u08,
                        u08 FilterMode_u08,
                        u08 InterruptMode_u08 );

extern
bit RLIN3_LINSendMessage( u08  UnitNumber_u08,
                          u08  PID_u08,
                          u08  MessageLength_u08,
                          u08  EnhancedCheckSum_u08,
                          u08  Direction_u08,
                          pu08 Data_pu08,
                          pu08 OperationMode_pu08 );

extern
bit RLIN3_LINEnableInterrupt( u08 UnitNumber_u08,
                              u08 Transmit_u08,
                              u08 Receive_u08,
                              u08 ErrorGlobal_u08,
                              u08 ErrorBit_u08,
                              u08 ErrorPhyBus_u08,
                              u08 ErrorTimeout_u08,
                              u08 ErrorFrame_u08,
                              u08 ErrorSync_u08,
                              u08 ErrorIDPar_u08,
                              u08 TimeoutSel_u08,
                              u08 Header_u08 );

extern
bit RLIN3_LINSReceiveHeader( u08 UnitNumber_u08 );

extern
bit RLIN3_LINGetStatus( u08  UnitNumber_u08,
                        pu08 HeaderReceived_pu08,
                        pu08 DataReceived_pu08,
                        pu08 DataTransmitted_pu08,
                        pu08 PID_pu08 );

extern
bit RLIN3_LINGetError( u08  UnitNumber_u08,
                       pu08 AnyError_pu08,
                       pu08 BitError_pu08,
                       pu08 DataConsistencyError_pu08,
                       pu08 TimeoutError_pu08,
                       pu08 FrameError_pu08,
                       pu08 SyncFieldError_pu08,
                       pu08 CheckSumError_pu08,
                       pu08 IdentParityError_pu08,
                       pu08 ResponsePreparationError_pu08 );

extern
bit RLIN3_LINSReceiveAction( u08  UnitNumber_u08,
                             u08  ActionType_u08,
                             u08  MessageLength_u08,
                             u08  EnhancedCheckSum_u08 );

extern
bit RLIN3_LINReceiveMessage( u08  UnitNumber_u08,
                             u08  MessageLength_u08,
                             pu08 Data_pu08 );

extern
bit RLIN3_LINSendWakeup( u08 UnitNumber_u08,
                         u08 WakeupLength_u08 );
                         
extern
bit RLIN3_LINReceiveWakeup( u08 UnitNumber_u08 );

extern
bit RLIN3_LINRestart( u08 UnitNumber_u08 );

#endif
