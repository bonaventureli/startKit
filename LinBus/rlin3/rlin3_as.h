#ifndef _RLIN3_AS_H
#define _RLIN3_AS_H

#include <stddef.h>
#include <ree_types.h>

#define  DRIVER_LOCAL

#include <map_rlin3.h>


/* Interrupt priorities of CAN driver */

#define RLIN3_AS_IRQPRIO_RX  ( RLIN3_INTENABLEDEFAULT )
#define RLIN3_AS_IRQPRIO_TX  ( RLIN3_INTENABLEDEFAULT )
#define RLIN3_AS_IRQPRIO_ERR ( RLIN3_INTENABLEDEFAULT )
#define RLIN3_AS_IRQPRIO_COM ( RLIN3_INTCLEAR )

/* Common LIN AUTOSAR driver types */

typedef u08 uint8;

typedef struct Lin_UnitConfigType
{
  u08 UnitNumber_u08;
  u08 LINMode_u08;
  u08 LINStandard_u08;
  u32 Baudrate_u32;
  u08 SamplesPerBit_u08;
  u08 ClockSelection_u08;
  u08 WakeupLength_u08;
  u08 BreakDetectionLength_u08;
  u08 BreakLength_u08;
  u08 BreakDelimiter_u08;
  u08 HeaderSpace_u08;
  u08 InterByteSpace_u08;
  u08 FilterMode_u08;
  u08 InterruptMode_u08;
  u08 ErrorBit_u08;
  u08 ErrorPhyBus_u08;
  u08 ErrorTimeout_u08;
  u08 ErrorFrame_u08;
  u08 ErrorSync_u08;
  u08 ErrorIDParity_u08;
  u08 TimeoutSel_u08;
  u08 TableNumber_u08;
  
} Lin_UnitConfigType;

typedef struct Lin_ConfigType
{
  //const struct Lin_UnitConfigType  *Unit[ RLIN3_MAXMACROS ];
  struct Lin_UnitConfigType  *Unit[ RLIN3_MAXMACROS ];
}Lin_ConfigType;

typedef u08 Lin_FramePidType;
typedef u08 Lin_FrameDlType;

typedef enum
{
  LIN_ENHANCED_CS = RLIN3_CHECKSUM_ENHANCED,
  LIN_CLASSIC_CS  = RLIN3_CHECKSUM_CLASSIC
  
} Lin_FrameCsModelType;

typedef enum
{
  LIN_MASTER_RESPONSE = RLIN3_RESPONSE_TRANSMIT,
  LIN_SLAVE_RESPONSE  = RLIN3_RESPONSE_RECEIVE,
  LIN_SLAVE_TO_SLAVE  = RLIN3_INVALID              /* S2S is not yet supported */
  
} Lin_FrameResponseType;

typedef struct Lin_PduType
{
  Lin_FramePidType       Pid;
  Lin_FrameCsModelType   Cs;
  Lin_FrameResponseType  Drc;
  Lin_FrameDlType        Dl;
  uint8                 *SduPtr;
  
} Lin_PduType;

typedef enum
{
  LIN_NOT_OK,
  LIN_TX_OK,
  LIN_TX_BUSY,
  LIN_TX_HEADER_ERROR,
  LIN_TX_ERROR,
  LIN_RX_OK,
  LIN_RX_BUSY,
  LIN_RX_ERROR,
  LIN_RX_NO_RESPONSE,
  LIN_OPERATIONAL,
  LIN_CH_SLEEP
  
} Lin_StatusType;

typedef enum
{
  E_OK     = RLIN3_OK,
  E_NOT_OK = RLIN3_ERROR
  
} Std_ReturnType;

typedef struct LinSlaveTable
{
  uint8        Semaphore;
  Lin_PduType *PDU;

} LinSlaveTable;


/* Function prototypes */

//void           Lin_Init( const Lin_ConfigType *Config );
void           Lin_Init(  Lin_ConfigType *Config );
Std_ReturnType Lin_CheckWakeup( uint8 Channel );
Std_ReturnType Lin_SendFrame( uint8 Channel, Lin_PduType *PduInfoPtr );
Std_ReturnType Lin_GoToSleep( uint8 Channel );
Std_ReturnType Lin_Wakeup( uint8 Channel );
Lin_StatusType Lin_GetStatus( uint8 Channel, uint8 *LinSduPtr );
Std_ReturnType Lin_GetFrame( uint8                Channel, 
                             Lin_FrameDlType      Length, 
                             Lin_FrameCsModelType ChecksumType );
Std_ReturnType Lin_SkipFrame( uint8 Channel );

#endif
