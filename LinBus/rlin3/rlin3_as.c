#include <ree_types.h>

#include <map_rlin3.h>
#include <rlin3_as.h>
#include <rlin3_as_cfg.h>
#include "rlin3_api.h"

/* External functions (callbacks) */

extern void LinIf_RxHeaderIndication( Lin_FramePidType );
extern void LinIf_RxFrameIndication( Lin_FramePidType, uint8* );

/*
 * functions
 */

void LinIf_RxHeaderIndication( Lin_FramePidType PID )
{
}

void LinIf_RxFrameIndication( Lin_FramePidType PID, uint8* DataArray )
{
}

/* Local Static Flags */

static u08 WakeupLength[ RLIN3_MAXMACROS ] = { RLIN3_WUPTL_8BITS, RLIN3_WUPTL_8BITS,
                                               RLIN3_WUPTL_8BITS, RLIN3_WUPTL_8BITS,
                                               RLIN3_WUPTL_8BITS, RLIN3_WUPTL_8BITS };

static Lin_FrameDlType CurrentDl[ RLIN3_MAXMACROS ] = { 0, 0, 0, 0, 0, 0 };

//static Lin_StatusType CurrentState[ RLIN3_MAXMACROS ] = { LIN_NOT_OK, LIN_NOT_OK,
//                                                          LIN_NOT_OK, LIN_NOT_OK,
//                                                          LIN_NOT_OK, LIN_NOT_OK };

Lin_StatusType CurrentState[ RLIN3_MAXMACROS ] = { LIN_NOT_OK, LIN_NOT_OK,
                                                          LIN_NOT_OK, LIN_NOT_OK,
                                                          LIN_NOT_OK, LIN_NOT_OK };




/* Interrupt routines */
void RLIN3_LINM_AS_IRQSend( void )
{
  u08 ActiveUnit_u08;
  u08 HeaderReceived_u08;
  u08 DataReceived_u08;
  u08 DataTransmitted_u08;
  u08 PID_u08;
  
  for( ActiveUnit_u08 = 0;
       ActiveUnit_u08 < RLIN3_MAXMACROS;
       ActiveUnit_u08++ )
  {
    if( CurrentState[ ActiveUnit_u08 ] != LIN_NOT_OK )
    {
      RLIN3_LINGetStatus( ActiveUnit_u08,
                          &HeaderReceived_u08,
                          &DataReceived_u08,
                          &DataTransmitted_u08,
                          &PID_u08 );
                        
      switch( CurrentState[ ActiveUnit_u08 ] )
      {
        case LIN_TX_BUSY:
          if( DataTransmitted_u08 )
          {
            CurrentState[ ActiveUnit_u08 ] = LIN_TX_OK;
		  }
          else
          {
            CurrentState[ ActiveUnit_u08 ] = LIN_TX_ERROR;
          }
        break;

      default:
        break;
      }
    }
  }
}

void RLIN3_LINM_AS_IRQReceive( void )
{
  u08 ActiveUnit_u08;
  u08 HeaderReceived_u08;
  u08 DataReceived_u08;
  u08 DataTransmitted_u08;
  u08 PID_u08;
  
  for( ActiveUnit_u08 = 0;
       ActiveUnit_u08 < RLIN3_MAXMACROS;
       ActiveUnit_u08++ )
  {
    if( CurrentState[ ActiveUnit_u08 ] != LIN_NOT_OK )
    {
      RLIN3_LINGetStatus( ActiveUnit_u08,
                          &HeaderReceived_u08,
                          &DataReceived_u08,
                          &DataTransmitted_u08,
                          &PID_u08 );
                        
      switch( CurrentState[ ActiveUnit_u08 ] )
      {
        case LIN_RX_BUSY:
          if( DataReceived_u08 )
          {
            CurrentState[ ActiveUnit_u08 ] = LIN_RX_OK;
						RLIN3_Rx_Callback(ActiveUnit_u08,Lin_Rx_Buffer);
          }
          else
          {
            CurrentState[ ActiveUnit_u08 ] = LIN_RX_ERROR;
          }
        break;

      default:
        break;
      }
    }
  }
}

void RLIN3_LINM_AS_IRQError( void )
{
  u08 ActiveUnit_u08;  
  u08 AnyError_u08;
  u08 BitError_u08;
  u08 DataConsistencyError_u08;
  u08 TimeoutError_u08;
  u08 FrameError_u08;
  u08 SyncFieldError_u08;
  u08 CheckSumError_u08;
  u08 IdentParityError_u08;
  u08 ResponsePreparationError_u08;
  
  for( ActiveUnit_u08 = 0;
       ActiveUnit_u08 < RLIN3_MAXMACROS;
       ActiveUnit_u08++ )
  {
    if( CurrentState[ ActiveUnit_u08 ] != LIN_NOT_OK )
    {
      RLIN3_LINGetError( ActiveUnit_u08,
                         &AnyError_u08,
                         &BitError_u08,
                         &DataConsistencyError_u08,
                         &TimeoutError_u08,
                         &FrameError_u08,
                         &SyncFieldError_u08,
                         &CheckSumError_u08,
                         &IdentParityError_u08,
                         &ResponsePreparationError_u08 );

      if( AnyError_u08 )
      {
        if( DataConsistencyError_u08 ) CurrentState[ ActiveUnit_u08 ] = LIN_TX_HEADER_ERROR;
        if( CheckSumError_u08 )        CurrentState[ ActiveUnit_u08 ] = LIN_RX_ERROR;
        if( TimeoutError_u08 )         CurrentState[ ActiveUnit_u08 ] = LIN_RX_NO_RESPONSE;
        if( BitError_u08 )             CurrentState[ ActiveUnit_u08 ] = LIN_TX_ERROR;
      }
    }
  }
}

/* API Functions */

//=============================================================================
// FunctionName: Lin_Init
// IN  : Configuration set
// OUT : -
// Description : 
//=============================================================================
// void Lin_Init( const Lin_ConfigType *Config )
void Lin_Init(  Lin_ConfigType *Config )
{
  u08 CurrentUnit_u08;
  bit UARTStatus_bit   = RLIN3_OK;
  
  for( CurrentUnit_u08 = RLIN3_0;
       CurrentUnit_u08 < RLIN3_MAXMACROS;
       CurrentUnit_u08++ )
  {
    if( Config->Unit[ CurrentUnit_u08 ] != NULL )
    {
      UARTStatus_bit &= RLIN3_Reset( Config->Unit[ CurrentUnit_u08 ]->UnitNumber_u08 );
      UARTStatus_bit &= RLIN3_PortEnable( Config->Unit[ CurrentUnit_u08 ]->UnitNumber_u08 );

      UARTStatus_bit &= RLIN3_LINEnableInterrupt( 
                          Config->Unit[ CurrentUnit_u08 ]->UnitNumber_u08,
                          RLIN3_SET,       /* TX */
                          RLIN3_SET,       /* RX */
                          RLIN3_SET,       /* ERR */
                          Config->Unit[ CurrentUnit_u08 ]->ErrorBit_u08,      /* Bit error */
                          Config->Unit[ CurrentUnit_u08 ]->ErrorPhyBus_u08,   /* Phys. Bus error */
                          Config->Unit[ CurrentUnit_u08 ]->ErrorTimeout_u08,  /* Timeout error */
                          Config->Unit[ CurrentUnit_u08 ]->ErrorFrame_u08,    /* Frame error */
                          Config->Unit[ CurrentUnit_u08 ]->ErrorSync_u08,     /* Sync error */
                          Config->Unit[ CurrentUnit_u08 ]->ErrorIDParity_u08, /* ID Parity error */
                          Config->Unit[ CurrentUnit_u08 ]->TimeoutSel_u08,    /* Frame Timeout */
                          ( Config->Unit[ CurrentUnit_u08 ]->LINMode_u08
                           == RLIN3_MODE_LINMASTER ) ? RLIN3_CLEAR : RLIN3_SET );   /* HEADER */

      UARTStatus_bit &= RLIN3_LINSetConfig( 
                          Config->Unit[ CurrentUnit_u08 ]->UnitNumber_u08,
                          OSCILLATOR_FREQUENCY,
                          Config->Unit[ CurrentUnit_u08 ]->LINMode_u08,
                          Config->Unit[ CurrentUnit_u08 ]->LINStandard_u08,
                          Config->Unit[ CurrentUnit_u08 ]->Baudrate_u32,
                          Config->Unit[ CurrentUnit_u08 ]->SamplesPerBit_u08,
                          Config->Unit[ CurrentUnit_u08 ]->ClockSelection_u08,
                          Config->Unit[ CurrentUnit_u08 ]->BreakDetectionLength_u08,
                          Config->Unit[ CurrentUnit_u08 ]->BreakLength_u08,
                          Config->Unit[ CurrentUnit_u08 ]->BreakDelimiter_u08,
                          Config->Unit[ CurrentUnit_u08 ]->HeaderSpace_u08,
                          Config->Unit[ CurrentUnit_u08 ]->InterByteSpace_u08,
                          Config->Unit[ CurrentUnit_u08 ]->FilterMode_u08,
                          Config->Unit[ CurrentUnit_u08 ]->InterruptMode_u08 );

      switch( Config->Unit[ CurrentUnit_u08 ]->LINMode_u08 )
      {
        case RLIN3_MODE_LINMASTER:

          if( RLIN3_AS_IRQPRIO_COM != RLIN3_INTCLEAR )
            UARTStatus_bit &= RLIN3_CreateInterrupt( 
                                Config->Unit[ CurrentUnit_u08 ]->UnitNumber_u08,
                                RLIN3_IRQ_COMMON,
                                RLIN3_AS_IRQPRIO_COM,
                                RLIN3_LINM_AS_IRQError );
          if( RLIN3_AS_IRQPRIO_TX != RLIN3_INTCLEAR )
            UARTStatus_bit &= RLIN3_CreateInterrupt( 
                                Config->Unit[ CurrentUnit_u08 ]->UnitNumber_u08,
                                RLIN3_IRQ_SEND,
                                RLIN3_AS_IRQPRIO_TX,
                                RLIN3_LINM_AS_IRQSend );
          if( RLIN3_AS_IRQPRIO_RX != RLIN3_INTCLEAR )
            UARTStatus_bit &= RLIN3_CreateInterrupt( 
                                Config->Unit[ CurrentUnit_u08 ]->UnitNumber_u08,
                                RLIN3_IRQ_RECEIVE,
                                RLIN3_AS_IRQPRIO_RX,
                                RLIN3_LINM_AS_IRQReceive );
          if( RLIN3_AS_IRQPRIO_ERR != RLIN3_INTCLEAR )
            UARTStatus_bit &= RLIN3_CreateInterrupt( 
                                Config->Unit[ CurrentUnit_u08 ]->UnitNumber_u08,
                                RLIN3_IRQ_ERROR,
                                RLIN3_AS_IRQPRIO_ERR,
                                RLIN3_LINM_AS_IRQError );
          break;
          
        case RLIN3_MODE_LINSLAVE_FIX:
        case RLIN3_MODE_LINSLAVE_AUTO:  
          break;
        }
    
      if( UARTStatus_bit == RLIN3_OK ) 
      {
        WakeupLength[ Config->Unit[ CurrentUnit_u08 ]
                                    ->UnitNumber_u08 ] = 
                                    Config->Unit[ CurrentUnit_u08 ]
                                    ->WakeupLength_u08;

        switch( Config->Unit[ CurrentUnit_u08 ]->LINMode_u08 )
        {
          case RLIN3_MODE_LINMASTER:
            CurrentState[ Config->Unit[ CurrentUnit_u08 ]
                                        ->UnitNumber_u08 ] = LIN_OPERATIONAL;
            break;
            
          case RLIN3_MODE_LINSLAVE_FIX:
          case RLIN3_MODE_LINSLAVE_AUTO:
            CurrentState[ Config->Unit[ CurrentUnit_u08 ]
                                        ->UnitNumber_u08 ] = LIN_RX_BUSY;
            break;
        }
      }
    }
  }
}

//=============================================================================
// FunctionName: Lin_SendFrame
// IN  : Channel, PDU
// OUT : -
// Description : 
// Send PDU on channel
//=============================================================================

Std_ReturnType Lin_SendFrame( uint8 Channel, Lin_PduType *PduInfoPtr )
{
  bit UARTStatus_bit;
  u08 OperationMode_u08;
  
  if( ( CurrentState[ Channel ] == LIN_TX_BUSY ) ||
      ( CurrentState[ Channel ] == LIN_RX_BUSY ) ||
      ( CurrentState[ Channel ] == LIN_NOT_OK )  ||
      ( CurrentState[ Channel ] == LIN_CH_SLEEP ) )
  {
    return( E_NOT_OK );
  }
  
  UARTStatus_bit = RLIN3_LINSendMessage( ( u08 )Channel,
                                         ( u08 )PduInfoPtr->Pid,
                                         ( u08 )PduInfoPtr->Dl,
                                         ( u08 )PduInfoPtr->Cs,
                                         ( u08 )PduInfoPtr->Drc,
                                         ( pu08 )PduInfoPtr->SduPtr,
                                         &OperationMode_u08 );

  CurrentDl[ Channel ] = PduInfoPtr->Dl;

  switch( PduInfoPtr->Drc )
  {
     case LIN_MASTER_RESPONSE:
      if( OperationMode_u08 == RLIN3_MODE_LINMASTER )
      {
        CurrentState[ Channel ] = LIN_TX_BUSY;
      }
      else
      {
        CurrentState[ Channel ] = LIN_NOT_OK;
      }
      break;
      
     case LIN_SLAVE_RESPONSE:
      if( OperationMode_u08 == RLIN3_MODE_LINMASTER )
      {
        CurrentState[ Channel ] = LIN_RX_BUSY;
      }
      else
      {
        CurrentState[ Channel ] = LIN_TX_BUSY;
      }
      break;
	  case LIN_SLAVE_TO_SLAVE:
      if( OperationMode_u08 == RLIN3_MODE_LINMASTER )
      {
        CurrentState[ Channel ] = LIN_RX_BUSY;
      }
      else
      {
        CurrentState[ Channel ] = LIN_TX_BUSY;
      }
      break;
  }

  if( UARTStatus_bit == RLIN3_OK ) return( E_OK );
  else return( E_NOT_OK );
}

//=============================================================================
// FunctionName: Lin_GetFrame
// IN  : Channel
// OUT : -
// Description : 
// Receive next response on channel
//=============================================================================

Std_ReturnType Lin_GetFrame( uint8                Channel, 
                             Lin_FrameDlType      Length, 
                             Lin_FrameCsModelType ChecksumType )
{
  bit UARTStatus_bit;

  UARTStatus_bit = RLIN3_LINSReceiveAction( Channel,
                                            RLIN3_A_GETMESSAGE,
                                            ( u08 )Length,
                                            ( u08 )( ( ChecksumType == LIN_ENHANCED_CS ) ? 
                                            RLIN3_CHECKSUM_ENHANCED : 
                                            RLIN3_CHECKSUM_CLASSIC ) );
  if( UARTStatus_bit == RLIN3_OK )
  {
    CurrentState[ Channel ] = LIN_RX_BUSY;
    return( E_OK );
  }
  else
  {
    CurrentState[ Channel ] = LIN_NOT_OK;
    return( E_NOT_OK );
  }
}

//=============================================================================
// FunctionName: Lin_SkipFrame
// IN  : Channel
// OUT : -
// Description : 
// Skip next response on channel
//=============================================================================

Std_ReturnType Lin_SkipFrame( uint8 Channel )
{
  bit UARTStatus_bit;

  UARTStatus_bit = RLIN3_LINSReceiveAction( Channel,
                                            RLIN3_A_SKIPMESSAGE,
                                            0,
                                            RLIN3_AS_CSMODEL_DEFAULT );
  if( UARTStatus_bit == RLIN3_OK )
  {
    CurrentState[ Channel ] = LIN_RX_BUSY;
    return( E_OK );
  }
  else
  {
    CurrentState[ Channel ] = LIN_NOT_OK;
    return( E_NOT_OK );
  }
}

//=============================================================================
// FunctionName: Lin_GetStatus
// IN  : Channel, SDU pointer
// OUT : -
// Description : 
// Get LIN status and receive SDU from channel
//=============================================================================

Lin_StatusType Lin_GetStatus( uint8 Channel, uint8 *LinSduPtr )
{
  bit UARTStatus_bit;
  u08 ByteCounter_u08;
  u08 LINBuffer[ RLIN3_LINDATALEN_MAX ];
  
  Lin_StatusType CurrentStateFlag;
  
  CurrentStateFlag = CurrentState[ Channel ];
  
  if( ( CurrentStateFlag == LIN_TX_HEADER_ERROR ) ||
      ( CurrentStateFlag == LIN_TX_ERROR )        ||
      ( CurrentStateFlag == LIN_RX_ERROR )        ||
      ( CurrentStateFlag == LIN_RX_NO_RESPONSE ) )
  {
    CurrentState[ Channel ] = LIN_OPERATIONAL;
  }
  
  if( CurrentStateFlag == LIN_RX_BUSY)//lin_rx_ok,LIN_RX_BUSY
  {
    UARTStatus_bit = RLIN3_LINReceiveMessage( Channel,
                                              CurrentDl[ Channel ],
                                              &LINBuffer[ 0 ] );
    if( UARTStatus_bit == RLIN3_OK )
    {
      for( ByteCounter_u08 = 0;
           ByteCounter_u08 < CurrentDl[ Channel ];
           ByteCounter_u08++ )
      {
        *LinSduPtr = LINBuffer[ ByteCounter_u08 ];
        LinSduPtr++;
      }
	  CurrentState[ Channel ] = LIN_RX_OK;
      return( LIN_RX_OK );//LIN_RX_BUSY
    }
    else
    {
      return( LIN_NOT_OK );
    }
  }
  else
  {
    return( CurrentStateFlag );
  }
}

//=============================================================================
// FunctionName: Lin_CheckWakeup
// IN  : Channel
// OUT : -
// Description : 
// Get LIN wakeup status
//=============================================================================

Std_ReturnType Lin_CheckWakeup( uint8 Channel )
{
  if( CurrentState[ Channel ] == LIN_CH_SLEEP ) return( E_NOT_OK );
  else return( E_OK );
}

//=============================================================================
// FunctionName: Lin_GoToSleep
// IN  : Channel
// OUT : -
// Description : 
// Set LIN channel in sleep mode
//=============================================================================

Std_ReturnType Lin_GoToSleep( uint8 Channel )
{
  bit UARTStatus_bit;  
  
  UARTStatus_bit = RLIN3_LINReceiveWakeup( ( u08 )Channel );
  
  if( UARTStatus_bit == RLIN3_OK )
  {
    CurrentState[ Channel ] = LIN_CH_SLEEP;
    return( E_OK );
  }
  else
  {
    CurrentState[ Channel ] = LIN_NOT_OK;
    return( E_NOT_OK );
  }
}

//=============================================================================
// FunctionName: Lin_Wakeup
// IN  : Channel
// OUT : -
// Description : 
// Set LIN channel in active mode, send wake up frame
//=============================================================================

Std_ReturnType Lin_Wakeup( uint8 Channel )
{
  bit UARTStatus_bit;  
  
  UARTStatus_bit = RLIN3_LINSendWakeup( ( u08 )Channel,
                                        WakeupLength[ Channel ] );
                                        
  if( UARTStatus_bit == RLIN3_OK )
  {
    CurrentState[ Channel ] = LIN_OPERATIONAL;
    return( E_OK );
  }
  else
  {
    CurrentState[ Channel ] = LIN_NOT_OK;
    return( E_NOT_OK );
  }
}
