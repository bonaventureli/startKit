#define DRIVER_LOCAL

#include <ree_types.h>

#include <map_rlin3.h>
#include <map_ports.h>
#include <standardports_p.h>

#include <stddef.h>

#if( RLIN3_MACROS > RLIN3_MAXMACROS )
#error "NUMBER OF CHANNELS OR UNITS NOT SUPPORTED"
#endif

/*
 * CreateInterrupt() Jump Vector Table
 */
 
void RLIN3_IRQDefault( void );

static void ( *RLIN3_CIVector [ RLIN3_MAXMACROS ][ RLIN3_INTERRUPTS ] )( ) =
{
  RLIN3_IRQDefault, RLIN3_IRQDefault, RLIN3_IRQDefault,
  RLIN3_IRQDefault, RLIN3_IRQDefault, RLIN3_IRQDefault,
  RLIN3_IRQDefault, RLIN3_IRQDefault, RLIN3_IRQDefault,
  RLIN3_IRQDefault, RLIN3_IRQDefault, RLIN3_IRQDefault,
  RLIN3_IRQDefault, RLIN3_IRQDefault, RLIN3_IRQDefault,
  RLIN3_IRQDefault, RLIN3_IRQDefault, RLIN3_IRQDefault
};

/*
 * Global Variables 
 */
 
/* Currently handled unit by interrupt */

u08  RLIN3_LIN_ActiveUnit_u08 = RLIN3_NP;

static u08 SendFailure_u08 = 0;

/*
 * Local Functions
 */

static u08 GetUARTSendIRQ( u08 );
static u08 GetUARTReceiveIRQ( u08 );

/* Interrupt Dialogue */

bit DataSentOK_bit[ RLIN3_MAXMACROS ]     = { RLIN3_OK, RLIN3_OK,
                                              RLIN3_OK, RLIN3_OK,
                                              RLIN3_OK, RLIN3_OK };
bit DataReceivedOK_bit[ RLIN3_MAXMACROS ] = { RLIN3_ERROR, RLIN3_ERROR,
                                              RLIN3_ERROR, RLIN3_ERROR,
                                              RLIN3_ERROR, RLIN3_ERROR };
u08 FirstSending_u08[ RLIN3_MAXMACROS ]   = { RLIN3_SET, RLIN3_SET,
                                              RLIN3_SET, RLIN3_SET,
                                              RLIN3_SET, RLIN3_SET };
void RLIN3_IRQDefault( void )
{
}

void RLIN3_IRQ_Send( u08 UnitNumber_u08 )
{
  RLIN3_LIN_ActiveUnit_u08 = UnitNumber_u08;
  DataSentOK_bit[ UnitNumber_u08 ] = RLIN3_TRANSFER_OK;

  if( RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_SEND ] != RLIN3_IRQDefault ) 
    RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_SEND ]( );
}

void RLIN3_IRQ_Receive( u08 UnitNumber_u08 )
{
  RLIN3_LIN_ActiveUnit_u08 = UnitNumber_u08;
  if( DataReceivedOK_bit[ UnitNumber_u08 ] != RLIN3_TRANSFER_OK ) 
  {
    DataReceivedOK_bit[ UnitNumber_u08 ] = RLIN3_TRANSFER_OK;
  }
  else              /* data has been overwritten */
  {                                    
    DataReceivedOK_bit[ UnitNumber_u08 ] = RLIN3_ERROR;
  }

  if( RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_RECEIVE ] != RLIN3_IRQDefault ) 
    RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_RECEIVE ]( );
}

void RLIN3_IRQ_Error( u08 UnitNumber_u08 )
{
  RLIN3_LIN_ActiveUnit_u08 = UnitNumber_u08;
  DataReceivedOK_bit[ UnitNumber_u08 ] = RLIN3_ERROR;

  if( RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_ERROR ] != RLIN3_IRQDefault ) 
    RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_ERROR ]( );
}

void RLIN3_IRQ_Common( u08 UnitNumber_u08 )
{
  RLIN3_LIN_ActiveUnit_u08 = UnitNumber_u08;
  switch( rlin3_p[ UnitNumber_u08 ]->lmd.lmd )
  {
    case RLIN3_MODE_LINMASTER:
      if( ( rlin3_p[ UnitNumber_u08 ]->lst.b.ftc == RLIN3_SET ) ||
          ( rlin3_p[ UnitNumber_u08 ]->lst.b.htrc == RLIN3_SET ) )
      {
        if( RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_SEND ] != RLIN3_IRQDefault ) 
          RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_SEND ]( );
      }
      if( ( rlin3_p[ UnitNumber_u08 ]->lst.b.frc == RLIN3_SET ) ||
          ( rlin3_p[ UnitNumber_u08 ]->lst.b.d1rc == RLIN3_SET ) )
      {
        if( RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_RECEIVE ] != RLIN3_IRQDefault ) 
          RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_RECEIVE ]( );
      }
      if( rlin3_p[ UnitNumber_u08 ]->lst.b.err == RLIN3_SET )
      {
        if( RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_ERROR ] != RLIN3_IRQDefault ) 
          RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_ERROR ]( );
      }
      break;

    case RLIN3_MODE_LINSLAVE_FIX:
    case RLIN3_MODE_LINSLAVE_AUTO:
      if( rlin3_p[ UnitNumber_u08 ]->lst.b.ftc == RLIN3_SET )
      {
        if( RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_SEND ] != RLIN3_IRQDefault ) 
          RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_SEND ]( );
      }
      if( ( rlin3_p[ UnitNumber_u08 ]->lst.b.frc == RLIN3_SET ) ||
          ( rlin3_p[ UnitNumber_u08 ]->lst.b.d1rc == RLIN3_SET ) ||
          ( rlin3_p[ UnitNumber_u08 ]->lst.b.htrc == RLIN3_SET ) )
      {
        if( RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_RECEIVE ] != RLIN3_IRQDefault ) 
          RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_RECEIVE ]( );
      }
      if( rlin3_p[ UnitNumber_u08 ]->lst.b.err == RLIN3_SET )
      {
        if( RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_ERROR ] != RLIN3_IRQDefault ) 
          RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_ERROR ]( );
      }
      break;

    case RLIN3_MODE_UART:
      if( rlin3_p[ UnitNumber_u08 ]->lst.b.ftc == RLIN3_SET )
      {
        DataSentOK_bit[ UnitNumber_u08 ] = RLIN3_TRANSFER_OK;
      
        if( RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_SEND ] != RLIN3_IRQDefault ) 
          RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_SEND ]( );
      }
      if( rlin3_p[ UnitNumber_u08 ]->lst.b.err == RLIN3_SET )
      {
        DataReceivedOK_bit[ UnitNumber_u08 ] = RLIN3_ERROR;
      
        if( RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_ERROR ] != RLIN3_IRQDefault ) 
          RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_ERROR ]( );
      }
      if( ( rlin3_p[ UnitNumber_u08 ]->lst.b.ftc == RLIN3_CLEAR ) &&
          ( rlin3_p[ UnitNumber_u08 ]->lst.b.err == RLIN3_CLEAR ) )
      {
        if( DataReceivedOK_bit[ UnitNumber_u08 ] != RLIN3_TRANSFER_OK ) 
        {
          DataReceivedOK_bit[ UnitNumber_u08 ] = RLIN3_TRANSFER_OK;
        }
        else              /* data has been overwritten */
        {                                    
          DataReceivedOK_bit[ UnitNumber_u08 ] = RLIN3_ERROR;
        }
      
        if( RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_RECEIVE ] != RLIN3_IRQDefault ) 
          RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_RECEIVE ]( );
      }
      break;
  }
}

/*
 * The peripheral support Functions
 */

bit RLIN3_CreateInterrupt( u08 UnitNumber_u08,
                           u08 InterruptSource_u08,
                           u08 InterruptLevel_u08,
                           void ( *FunctionVector )( ) )
{
  switch( UnitNumber_u08 )
    {
    case RLIN3_0:
      switch( InterruptSource_u08 )
        {
#ifdef RLIN3_INTM0TX
        case RLIN3_IRQ_SEND:
          RLIN3_INTM0TX = ( FunctionVector == NULL ? 
                            RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM0RX
        case RLIN3_IRQ_RECEIVE:
          RLIN3_INTM0RX = ( FunctionVector == NULL ? 
                            RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM0ERR
        case RLIN3_IRQ_ERROR:
          RLIN3_INTM0ERR = ( FunctionVector == NULL ? 
                             RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM0COM
        case RLIN3_IRQ_COMMON:
          RLIN3_INTM0COM = ( FunctionVector == NULL ? 
                             RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
        default: return( RLIN3_ERROR );
        } // end switch( InterruptSource )
      break;

    case RLIN3_1:
      switch( InterruptSource_u08 )
        {
#ifdef RLIN3_INTM1TX
        case RLIN3_IRQ_SEND:
          RLIN3_INTM1TX = ( FunctionVector == NULL ? 
                            RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM1RX
        case RLIN3_IRQ_RECEIVE:
          RLIN3_INTM1RX = ( FunctionVector == NULL ? 
                            RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM1ERR
        case RLIN3_IRQ_ERROR:
          RLIN3_INTM1ERR = ( FunctionVector == NULL ? 
                             RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM1COM
        case RLIN3_IRQ_COMMON:
          RLIN3_INTM1COM = ( FunctionVector == NULL ? 
                             RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
        default: return( RLIN3_ERROR );
        } // end switch( InterruptSource )
      break;

    case RLIN3_2:
      switch( InterruptSource_u08 )
        {
#ifdef RLIN3_INTM2TX
        case RLIN3_IRQ_SEND:
          RLIN3_INTM2TX = ( FunctionVector == NULL ? 
                            RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM2RX
        case RLIN3_IRQ_RECEIVE:
          RLIN3_INTM2RX = ( FunctionVector == NULL ? 
                            RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM2ERR
        case RLIN3_IRQ_ERROR:
          RLIN3_INTM2ERR = ( FunctionVector == NULL ? 
                             RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM2COM
        case RLIN3_IRQ_COMMON:
          RLIN3_INTM2COM = ( FunctionVector == NULL ? 
                             RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
        default: return( RLIN3_ERROR );
        } // end switch( InterruptSource )
      break;

    case RLIN3_3:
      switch( InterruptSource_u08 )
        {
#ifdef RLIN3_INTM3TX
        case RLIN3_IRQ_SEND:
          RLIN3_INTM3TX = ( FunctionVector == NULL ? 
                            RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM3RX
        case RLIN3_IRQ_RECEIVE:
          RLIN3_INTM3RX = ( FunctionVector == NULL ? 
                            RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM3ERR
        case RLIN3_IRQ_ERROR:
          RLIN3_INTM3ERR = ( FunctionVector == NULL ? 
                             RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM3COM
        case RLIN3_IRQ_COMMON:
          RLIN3_INTM3COM = ( FunctionVector == NULL ? 
                             RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
        default: return( RLIN3_ERROR );
        } // end switch( InterruptSource )
      /* break; */  // commented to avoid warning

    case RLIN3_4:
      switch( InterruptSource_u08 )
        {
#ifdef RLIN3_INTM4TX
        case RLIN3_IRQ_SEND:
          RLIN3_INTM4TX = ( FunctionVector == NULL ? 
                            RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM4RX
        case RLIN3_IRQ_RECEIVE:
          RLIN3_INTM4RX = ( FunctionVector == NULL ? 
                            RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM4ERR
        case RLIN3_IRQ_ERROR:
          RLIN3_INTM4ERR = ( FunctionVector == NULL ? 
                             RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM4COM
        case RLIN3_IRQ_COMMON:
          RLIN3_INTM4COM = ( FunctionVector == NULL ? 
                             RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
        default: return( RLIN3_ERROR );
        } // end switch( InterruptSource )
      /* break; */   // commented to avoid warning

    case RLIN3_5:
      switch( InterruptSource_u08 )
        {
#ifdef RLIN3_INTM5TX
        case RLIN3_IRQ_SEND:
          RLIN3_INTM5TX = ( FunctionVector == NULL ? 
                            RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM5RX
        case RLIN3_IRQ_RECEIVE:
          RLIN3_INTM5RX = ( FunctionVector == NULL ? 
                            RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM5ERR
        case RLIN3_IRQ_ERROR:
          RLIN3_INTM5ERR = ( FunctionVector == NULL ? 
                             RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
#ifdef RLIN3_INTM5COM
        case RLIN3_IRQ_COMMON:
          RLIN3_INTM5COM = ( FunctionVector == NULL ? 
                             RLIN3_INTCLEAR : InterruptLevel_u08 );
          break;
#endif
        default: return( RLIN3_ERROR );
        } // end switch( InterruptSource )
      /* break; */   // commented to avoid warning
  }
  
  if( FunctionVector == NULL ) FunctionVector = RLIN3_IRQDefault;
  
  if( InterruptSource_u08 == RLIN3_IRQ_COMMON )
  {
    RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_SEND ] = 
      FunctionVector;
    RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_RECEIVE ] = 
      FunctionVector;
    RLIN3_CIVector[ UnitNumber_u08 ][ RLIN3_IRQ_ERROR ] = 
      FunctionVector;
  }
  else
  {
    RLIN3_CIVector[ UnitNumber_u08 ][ InterruptSource_u08 ] = 
      FunctionVector;
  }
  
  return( RLIN3_OK );
}

bit RLIN3_PortEnable( u08 UnitNumber_u08 )
{
  switch( UnitNumber_u08 ) 
  {

#ifdef RLIN3_PORT_RXD0
    case RLIN3_0:
      PORT_Enable( RLIN3_PORT_RXD0, RLIN3_PORT_BIT_RXD0,
                   PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                   RLIN3_PORT_FUNC_RXD0 );
      PORT_Enable( RLIN3_PORT_TXD0, RLIN3_PORT_BIT_TXD0,
                   PORT_DIR_OUTPUT, PORT_MODE_PERIPHERAL, 
                   RLIN3_PORT_FUNC_TXD0 );
      break;
#endif                     

#ifdef RLIN3_PORT_RXD1
    case RLIN3_1:
      PORT_Enable( RLIN3_PORT_RXD1, RLIN3_PORT_BIT_RXD1,
                   PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                   RLIN3_PORT_FUNC_RXD1 );
      PORT_Enable( RLIN3_PORT_TXD1, RLIN3_PORT_BIT_TXD1,
                   PORT_DIR_OUTPUT, PORT_MODE_PERIPHERAL, 
                   RLIN3_PORT_FUNC_TXD1 );
      break;
#endif                     
#ifdef RLIN3_PORT_RXD2
    case RLIN3_2:
      PORT_Enable( RLIN3_PORT_RXD2, RLIN3_PORT_BIT_RXD2,
                   PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                   RLIN3_PORT_FUNC_RXD2 );
      PORT_Enable( RLIN3_PORT_TXD2, RLIN3_PORT_BIT_TXD2,
                   PORT_DIR_OUTPUT, PORT_MODE_PERIPHERAL, 
                   RLIN3_PORT_FUNC_TXD2 );
      break;
#endif                     

#ifdef RLIN3_PORT_RXD3
    case RLIN3_3:
      PORT_Enable( RLIN3_PORT_RXD3, RLIN3_PORT_BIT_RXD3,
                   PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                   RLIN3_PORT_FUNC_RXD3 );
      PORT_Enable( RLIN3_PORT_TXD3, RLIN3_PORT_BIT_TXD3,
                   PORT_DIR_OUTPUT, PORT_MODE_PERIPHERAL, 
                   RLIN3_PORT_FUNC_TXD3 );
      break;
#endif                     

#ifdef RLIN3_PORT_RXD4
    case RLIN3_4:
      PORT_Enable( RLIN3_PORT_RXD4, RLIN3_PORT_BIT_RXD4,
                   PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                   RLIN3_PORT_FUNC_RXD4 );
      PORT_Enable( RLIN3_PORT_TXD4, RLIN3_PORT_BIT_TXD4,
                   PORT_DIR_OUTPUT, PORT_MODE_PERIPHERAL, 
                   RLIN3_PORT_FUNC_TXD4 );
      break;
#endif                     

#ifdef RLIN3_PORT_RXD5
    case RLIN3_5:
      PORT_Enable( RLIN3_PORT_RXD5, RLIN3_PORT_BIT_RXD5,
                   PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                   RLIN3_PORT_FUNC_RXD5 );
      PORT_Enable( RLIN3_PORT_TXD5, RLIN3_PORT_BIT_TXD5,
                   PORT_DIR_OUTPUT, PORT_MODE_PERIPHERAL, 
                   RLIN3_PORT_FUNC_TXD5 );
      break;
#endif      
    default:
      return( RLIN3_ERROR );
    }
    
    return( RLIN3_OK );
}

bit RLIN3_PortDisable( u08 UnitNumber_u08 )
{ 
  switch( UnitNumber_u08 ) 
  {

#ifdef RLIN3_PORT_RXD0
    case RLIN3_0:
      PORT_Disable( RLIN3_PORT_RXD0, RLIN3_PORT_BIT_RXD0 );
      PORT_Disable( RLIN3_PORT_TXD0, RLIN3_PORT_BIT_TXD0 );
      break;
#endif

#ifdef RLIN3_PORT_RXD1
    case RLIN3_1:
      PORT_Disable( RLIN3_PORT_RXD1, RLIN3_PORT_BIT_RXD1 );
      PORT_Disable( RLIN3_PORT_TXD1, RLIN3_PORT_BIT_TXD1 );
      break;
#endif
#ifdef RLIN3_PORT_RXD2
    case RLIN3_2:
      PORT_Disable( RLIN3_PORT_RXD2, RLIN3_PORT_BIT_RXD2 );
      PORT_Disable( RLIN3_PORT_TXD2, RLIN3_PORT_BIT_TXD2 );
      break;
#endif
#ifdef RLIN3_PORT_RXD3
    case RLIN3_3:
      PORT_Disable( RLIN3_PORT_RXD3, RLIN3_PORT_BIT_RXD3 );
      PORT_Disable( RLIN3_PORT_TXD3, RLIN3_PORT_BIT_TXD3 );
      break;
#endif
#ifdef RLIN3_PORT_RXD4
    case RLIN3_4:
      PORT_Disable( RLIN3_PORT_RXD4, RLIN3_PORT_BIT_RXD4 );
      PORT_Disable( RLIN3_PORT_TXD4, RLIN3_PORT_BIT_TXD4 );
      break;
#endif
#ifdef RLIN3_PORT_RXD5
    case RLIN3_5:
      PORT_Disable( RLIN3_PORT_RXD5, RLIN3_PORT_BIT_RXD5 );
      PORT_Disable( RLIN3_PORT_TXD5, RLIN3_PORT_BIT_TXD5 );
      break;
#endif

    default:
      return( RLIN3_ERROR );
  }
  
  return( RLIN3_OK );
}

bit RLIN3_Reset( u08 UnitNumber_u08 )
{
  bit StatusFlag_bit;
  
  if( UnitNumber_u08 >= RLIN3_MACROS ) return( RLIN3_ERROR );
  
  rlin3_p[ UnitNumber_u08 ]->lcuc.om = RLIN3_MMODE_RESET;
  
  while( ( rlin3_p[ UnitNumber_u08 ]->lmst.om & RLIN3_MMODE_RESET_MASK )
          != RLIN3_MMODE_RESET );

  StatusFlag_bit =  RLIN3_CreateInterrupt( UnitNumber_u08,
                                           RLIN3_IRQ_SEND,
                                           0,
                                           NULL );
  StatusFlag_bit &= RLIN3_CreateInterrupt( UnitNumber_u08,
                                           RLIN3_IRQ_RECEIVE,
                                           0,
                                           NULL );
  StatusFlag_bit &= RLIN3_CreateInterrupt( UnitNumber_u08,
                                           RLIN3_IRQ_ERROR,
                                           0,
                                           NULL );
  StatusFlag_bit &= RLIN3_CreateInterrupt( UnitNumber_u08,
                                           RLIN3_IRQ_COMMON,
                                           0,
                                           NULL );
                           

  DataSentOK_bit[ UnitNumber_u08 ]     = RLIN3_OK;
  DataReceivedOK_bit[ UnitNumber_u08 ] = RLIN3_ERROR;

  FirstSending_u08[ UnitNumber_u08 ] = 1;

  return( RLIN3_OK );
}

static u16       /* local subfunction: not visible from outside */
RLIN3_SetPrescaler( u16  PreDivider_u16,
	                  u08  MaxRegValue_u08,
	                  pu08 RegValue_pu08 )
{
  u16 DivisionFactor_u16;

  /* let's just calculate the best prescaler selection */

  if( PreDivider_u16 < 1   )  
    {
      DivisionFactor_u16 = 1;
      *RegValue_pu08 = 0;
    }
  else if( PreDivider_u16 < 2   )  
    {
      DivisionFactor_u16 = 2;
      *RegValue_pu08 = 1;
    }
  else if( ( PreDivider_u16 >= 2  ) && ( PreDivider_u16 < 4   ) ) 
    {
      DivisionFactor_u16 = 4;
      *RegValue_pu08 = 2;
    }
  else if( ( PreDivider_u16 >= 4  ) && ( PreDivider_u16 < 8   ) )
    {
      DivisionFactor_u16 = 8;
      *RegValue_pu08 = 3;
    }
  else if( ( PreDivider_u16 >= 8  ) && ( PreDivider_u16 < 16  ) ) 
    {
      DivisionFactor_u16 = 16;
      *RegValue_pu08 = 4;
    }
  else if( ( PreDivider_u16 >= 16 ) && ( PreDivider_u16 < 32  ) ) 
    {
      DivisionFactor_u16 = 32;
      *RegValue_pu08 = 5;
    }
  else if( ( PreDivider_u16 >= 32 ) && ( PreDivider_u16 < 64  ) ) 
    {
      DivisionFactor_u16 = 64;
      *RegValue_pu08 = 6;
    }
  else if( ( PreDivider_u16 >= 64 ) && ( PreDivider_u16 < 128 ) ) 
    {
      DivisionFactor_u16 = 128;
      *RegValue_pu08 = 7;
    }
  else                                                    
    {
      *RegValue_pu08 = ( MaxRegValue_u08 + 1 );
    }

  return( DivisionFactor_u16 );
}

bit RLIN3_UARTModeConfig( u08 UnitNumber_u08,
                          flt Oscillator_Frequency_flt,
                          u32 Baudrate_u32,
                          u08 SamplesPerBit_u08,
                          u08 Parity_u08,
                          u08 CharLen_u08,
                          u08 StopBits_u08,
                          u08 Direction_u08,
                          u08 FilterMode_u08,
                          u08 InterruptMode_u08 )
{
  u16 TimerPrescaler_u16;      /* the Baudrate Timer Settings */
  u08 TimerPrescalerRegister_u08;
  u16 TimerModulus_u16;
  flt Factor_flt;

  if( UnitNumber_u08 >= RLIN3_MACROS ) return( RLIN3_ERROR );
  if( ( SamplesPerBit_u08 < RLIN3_SAMPLES_MIN ) ||
      ( SamplesPerBit_u08 > RLIN3_SAMPLES_MAX ) ||
      ( SamplesPerBit_u08 == RLIN3_SAMPLES_FORBIDDEN ) )
      return( RLIN3_ERROR );
      
  if( ( rlin3_p[ UnitNumber_u08 ]->lmst.om & RLIN3_MMODE_RESET_MASK ) 
    != RLIN3_MMODE_RESET )
    return( RLIN3_ERROR );

  Factor_flt = ( flt )RLIN3_FREQFACTOR / ( flt )SamplesPerBit_u08;

  TimerPrescaler_u16 = 
    ( ( u16 )( ( Oscillator_Frequency_flt * Factor_flt )/ ( flt )( Baudrate_u32 ) ) )
    / ( RLIN3_MAX_MODULUS_OTHER );
  
  TimerPrescaler_u16 = RLIN3_SetPrescaler( TimerPrescaler_u16, 
                                           RLIN3_MAX_PRESCALER,
                                           &TimerPrescalerRegister_u08 );
  if( TimerPrescalerRegister_u08 > RLIN3_MAX_PRESCALER ) return( RLIN3_ERROR );
  
  TimerModulus_u16 = ( u16 )
    ( ( 
        ( ( Oscillator_Frequency_flt * Factor_flt )
          / ( flt )TimerPrescaler_u16 ) ) / ( ( flt )Baudrate_u32 ) );

  if( TimerModulus_u16 < RLIN3_MIN_MODULUS )
    {
      return( RLIN3_ERROR );
    } 
    
  /* Baudrate setting */

  rlin3_p[ UnitNumber_u08 ]->lwbr.lprs = TimerPrescalerRegister_u08;
  rlin3_p[ UnitNumber_u08 ]->lwbr.nspb = SamplesPerBit_u08 - 1;
  rlin3_p[ UnitNumber_u08 ]->brp0      = ( u08 )( TimerModulus_u16 & 0x00FF );
  rlin3_p[ UnitNumber_u08 ]->brp1      = ( u08 )( TimerModulus_u16 >> 8 );

  /* Other Parameters */

  rlin3_p[ UnitNumber_u08 ]->lbfc.uart.ubls  = CharLen_u08;
  rlin3_p[ UnitNumber_u08 ]->lbfc.uart.ups   = Parity_u08;
  rlin3_p[ UnitNumber_u08 ]->lbfc.uart.usbls = StopBits_u08;
  rlin3_p[ UnitNumber_u08 ]->lbfc.uart.ubos  = Direction_u08;
  rlin3_p[ UnitNumber_u08 ]->lbfc.uart.urps  = 0;     /* No port inversion */
  rlin3_p[ UnitNumber_u08 ]->lbfc.uart.utps  = 0;  
  rlin3_p[ UnitNumber_u08 ]->luor1.uebe      = 0;     /* No 9-bit modes */
  rlin3_p[ UnitNumber_u08 ]->luor1.uebdce    = 0;  
  rlin3_p[ UnitNumber_u08 ]->luor1.uecd      = 0;  
  rlin3_p[ UnitNumber_u08 ]->luor1.utigts    = 1;     /* IRQ on TX completion */

  /* Activate Communication */
  
  rlin3_p[ UnitNumber_u08 ]->lmd.lios        = InterruptMode_u08;
  rlin3_p[ UnitNumber_u08 ]->lmd.lrdnfs      = FilterMode_u08;
  rlin3_p[ UnitNumber_u08 ]->lmd.lcks        = RLIN3_CLOCK_FA;
  rlin3_p[ UnitNumber_u08 ]->lmd.lmd         = RLIN3_MODE_UART;
  
  rlin3_p[ UnitNumber_u08 ]->lcuc.om         = RLIN3_MMODE_OPERATION;
  
  while( rlin3_p[ UnitNumber_u08 ]->lmst.om != RLIN3_MMODE_OPERATION );
  
  rlin3_p[ UnitNumber_u08 ]->luoer.utoe      = 1;     /* Enable RX/TX */
  rlin3_p[ UnitNumber_u08 ]->luoer.uroe      = 1;

  return( RLIN3_OK );
}

bit RLIN3_LegacyConfig( u08 UnitNumber_u08,
                        flt Oscillator_Frequency_flt,
                        u32 Baudrate_u32,
                        u08 Parity_u08,
                        u08 CharLen_u08,
                        u08 StopBits_u08,
                        u08 Direction_u08 )
{
  return( RLIN3_UARTModeConfig( UnitNumber_u08,
                                Oscillator_Frequency_flt,
                                Baudrate_u32,
                                RLIN3_SAMPLES_SLAVE_AUTO,
                                Parity_u08,
                                CharLen_u08,
                                StopBits_u08,
                                Direction_u08,
                                RLIN3_SAMPLE_SINGLE,
                                RLIN3_SINGLE_INT ) );
}

static u08 GetUARTSendIRQ( u08 UnitNumber_u08 )
{
  switch( UnitNumber_u08 )
  {
#ifdef RLIN3_INTM0TX  
      case RLIN3_0:
          if( RLIN3_INTM0TX & RLIN3_INTFLAG )
          {
              RLIN3_INTM0TX &= ~RLIN3_INTFLAG;
              return( 1 );
          }
          else return( 0 );
#endif
#ifdef RLIN3_INTM1TX  
      case RLIN3_1:
          if( RLIN3_INTM1TX & RLIN3_INTFLAG )
          {
              RLIN3_INTM1TX &= ~RLIN3_INTFLAG;
              return( 1 );
          }
          else return( 0 );
#endif
#ifdef RLIN3_INTM2TX  
      case RLIN3_2:
          if( RLIN3_INTM2TX & RLIN3_INTFLAG )
          {
              RLIN3_INTM2TX &= ~RLIN3_INTFLAG;
              return( 1 );
          }
          else return( 0 );
#endif
#ifdef RLIN3_INTM3TX  
      case RLIN3_3:
          if( RLIN3_INTM3TX & RLIN3_INTFLAG )
          {
              RLIN3_INTM3TX &= ~RLIN3_INTFLAG;
              return( 1 );
          }
          else return( 0 );
#endif
#ifdef RLIN3_INTM4TX  
      case RLIN3_4:
          if( RLIN3_INTM4TX & RLIN3_INTFLAG )
          {
              RLIN3_INTM4TX &= ~RLIN3_INTFLAG;
              return( 1 );
          }
          else return( 0 );
#endif
#ifdef RLIN3_INTM5TX  
      case RLIN3_5:
          if( RLIN3_INTM5TX & RLIN3_INTFLAG )
          {
              RLIN3_INTM5TX &= ~RLIN3_INTFLAG;
              return( 1 );
          }
          else return( 0 );
#endif
      default:
          return( 0 );
  }
}    

static u08 GetUARTReceiveIRQ( u08 UnitNumber_u08 )
{
    switch( UnitNumber_u08 )
    {
#ifdef RLIN3_INTM0RX  
        case RLIN3_0:
            if( RLIN3_INTM0RX & RLIN3_INTFLAG )
            {
                RLIN3_INTM0RX &= ~RLIN3_INTFLAG;
                return( 1 );
            }
            else return( 0 );
#endif
#ifdef RLIN3_INTM1RX  
        case RLIN3_1:
            if( RLIN3_INTM1RX & RLIN3_INTFLAG )
            {
                RLIN3_INTM1RX &= ~RLIN3_INTFLAG;
                return( 1 );
            }
            else return( 0 );
#endif
#ifdef RLIN3_INTM2RX  
        case RLIN3_2:
            if( RLIN3_INTM2RX & RLIN3_INTFLAG )
            {
                RLIN3_INTM2RX &= ~RLIN3_INTFLAG;
                return( 1 );
            }
            else return( 0 );
#endif
#ifdef RLIN3_INTM3RX  
        case RLIN3_3:
            if( RLIN3_INTM3RX & RLIN3_INTFLAG )
            {
                RLIN3_INTM3RX &= ~RLIN3_INTFLAG;
                return( 1 );
            }
            else return( 0 );
#endif
#ifdef RLIN3_INTM4RX  
        case RLIN3_4:
            if( RLIN3_INTM4RX & RLIN3_INTFLAG )
            {
                RLIN3_INTM4RX &= ~RLIN3_INTFLAG;
                return( 1 );
            }
            else return( 0 );
#endif
#ifdef RLIN3_INTM5RX  
        case RLIN3_5:
            if( RLIN3_INTM5RX & RLIN3_INTFLAG )
            {
                RLIN3_INTM5RX &= ~RLIN3_INTFLAG;
                return( 1 );
            }
            else return( 0 );
#endif
        default:
            return( 0 );
    }
}    

bit RLIN3_SendByte( u08 UnitNumber_u08,
                    u08 SendByte_u08 )
{
  u08 PhysicalPort_u08 = UnitNumber_u08 & RLIN3_NP;
  u32 SendTimeout_u32  = RLIN3_SENDTIMEOUT;

  if( PhysicalPort_u08 >= RLIN3_MACROS ) return( RLIN3_ERROR );

  if( FirstSending_u08[ PhysicalPort_u08 ] ) {

    DataSentOK_bit[ PhysicalPort_u08 ]   = RLIN3_ERROR;
    rlin3_p[ PhysicalPort_u08 ]->lutdr = ( u16 )SendByte_u08;
    FirstSending_u08[ PhysicalPort_u08 ] = 0;
    return( RLIN3_TRANSFER_OK );
  }
  else {
    if( UnitNumber_u08 & RLIN3_TP )
    {
      if( GetUARTSendIRQ( PhysicalPort_u08 ) == 0 )
        return( RLIN3_ERROR );
    }
    else
    {
      if( !SendFailure_u08 )
        {
          while( ( DataSentOK_bit[ PhysicalPort_u08 ] != RLIN3_TRANSFER_OK ) &&
                 ( --SendTimeout_u32 > 0L ) );
        }
        else
        {
          while( ( rlin3_p[ PhysicalPort_u08 ]->lst.b.uts == 1 ) && 
                 ( --SendTimeout_u32 > 0L ) );
        }
    }
    if( SendTimeout_u32 == 0L )     
    {
      GetUARTSendIRQ( PhysicalPort_u08 );
      SendFailure_u08 = 1;
      SendByte_u08 = '#';
    }
    DataSentOK_bit[ PhysicalPort_u08 ]  = RLIN3_ERROR;
    rlin3_p[ PhysicalPort_u08 ]->lutdr = ( u16 )SendByte_u08;
    return( RLIN3_TRANSFER_OK );
  }
}

bit RLIN3_ReceiveByte( u08  UnitNumber_u08,
                       pu08 ReceiveByte_pu08 )
{
  u08 PhysicalPort_u08 = UnitNumber_u08 & RLIN3_NP;

  if( PhysicalPort_u08 >= RLIN3_MACROS ) return( RLIN3_ERROR );

  if( SendFailure_u08 )
  {
    if( ( GetUARTReceiveIRQ( PhysicalPort_u08 ) ) ||      /* emergency entry: */
        ( rlin3_p[ PhysicalPort_u08 ]->lest.uart.uper != 0 ) ||
        ( DataReceivedOK_bit[ PhysicalPort_u08 ] == RLIN3_TRANSFER_OK ) )
    {                                              /* receive with parity error */
        *ReceiveByte_pu08 = ( u08 )rlin3_p[ PhysicalPort_u08 ]->lurdr;
        DataReceivedOK_bit[ PhysicalPort_u08 ] = RLIN3_ERROR;
        return( RLIN3_TRANSFER_OK );           
    }
  }
  else
  {
    if( DataReceivedOK_bit[ PhysicalPort_u08 ] == RLIN3_TRANSFER_OK ) 
    {
        *ReceiveByte_pu08 = ( u08 )rlin3_p[ PhysicalPort_u08 ]->lurdr;
        DataReceivedOK_bit[ PhysicalPort_u08 ] = RLIN3_ERROR;
        return( RLIN3_TRANSFER_OK );    
    }
  }

  return( RLIN3_ERROR );
}

bit RLIN3_GetUARTStatus( u08  UnitNumber_u08,
                         pu08 BitError_pu08,
                         pu08 ParityError_pu08,
                         pu08 FrameError_pu08,
                         pu08 OverrunError_pu08,
                         pu08 SendFlag_pu08 )
{
    volatile u08 DummyRead_u08;

    if( UnitNumber_u08 >= RLIN3_MACROS ) return( RLIN3_ERROR );

    *BitError_pu08             = rlin3_p[ UnitNumber_u08 ]->lest.uart.ber;
    *ParityError_pu08          = rlin3_p[ UnitNumber_u08 ]->lest.uart.uper; 
    *FrameError_pu08           = rlin3_p[ UnitNumber_u08 ]->lest.uart.fer;
    *OverrunError_pu08         = rlin3_p[ UnitNumber_u08 ]->lest.uart.oer;

    DummyRead_u08 = ( u08 )rlin3_p[ UnitNumber_u08 ]->lurdr;

    *SendFlag_pu08 = rlin3_p[ UnitNumber_u08 ]->lst.b.uts;

    /* Clear all error status flags */

    rlin3_p[ UnitNumber_u08 ]->lest.all = RLIN3_CLEAR;

    return( RLIN3_OK );
}

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
                        u08 InterruptMode_u08 )
{
  u16 TimerPrescaler_u16;      /* the Baudrate Timer Settings */
  u08 TimerPrescalerRegister_u08;
  u16 TimerModulus_u16;
  u08 ClockSelectionFactor_u08;
  flt Factor_flt;

  if( UnitNumber_u08 >= RLIN3_MACROS ) return( RLIN3_ERROR );

  if( ( OperationMode_u08 == RLIN3_MODE_LINMASTER ) ||
      ( OperationMode_u08 == RLIN3_MODE_LINSLAVE_FIX ) )
  {
    if( SamplesPerBit_u08 != RLIN3_SAMPLES_SLAVE_FIX )
    {
      return( RLIN3_ERROR );
    }
    if( OperationMode_u08 == RLIN3_MODE_LINSLAVE_FIX )
    {
      if( ClockSelection_u08 != RLIN3_CLOCK_FA )
      {
        return( RLIN3_ERROR );
      }
    }
  }
  else if( OperationMode_u08 == RLIN3_MODE_LINSLAVE_AUTO )
  {
    if( SamplesPerBit_u08 != RLIN3_SAMPLES_SLAVE_AUTO )
    {
      return( RLIN3_ERROR );
    }
    if( ClockSelection_u08 != RLIN3_CLOCK_FA )
    {
      return( RLIN3_ERROR );
    }
  }
  else
  {
    return( RLIN3_ERROR );   /* only LIN modes allowed */
  }
  
  if( ( rlin3_p[ UnitNumber_u08 ]->lmst.om & RLIN3_MMODE_RESET_MASK ) 
    != RLIN3_MMODE_RESET )
    return( RLIN3_ERROR );

  switch( ClockSelection_u08 )
  {
    case RLIN3_CLOCK_FA:
      ClockSelectionFactor_u08 = RLIN3_CLOCKDIVIDER_FA;
      break;
    case RLIN3_CLOCK_FB:
      ClockSelectionFactor_u08 = RLIN3_CLOCKDIVIDER_FB;
      break;
    case RLIN3_CLOCK_FC:
      ClockSelectionFactor_u08 = RLIN3_CLOCKDIVIDER_FC;
      break;
    case RLIN3_CLOCK_FD:
      ClockSelectionFactor_u08 = RLIN3_CLOCKDIVIDER_FD;
      break;
    default:
      return( RLIN3_ERROR );
  }
  // 1/32 = 1/2 * 1/16 16 samplebit for master
  Factor_flt = ( flt )RLIN3_FREQFACTOR / 
               ( ( flt )SamplesPerBit_u08 * ( flt )ClockSelectionFactor_u08 );

  TimerPrescaler_u16 = 
    ( ( u16 )( ( Oscillator_Frequency_flt * Factor_flt )/ ( flt )( Baudrate_u32 ) ) )
    / ( ( OperationMode_u08 == RLIN3_MODE_LINMASTER ) ? 
          RLIN3_MAX_MODULUS_MASTER : RLIN3_MAX_MODULUS_OTHER );
  
  TimerPrescaler_u16 = RLIN3_SetPrescaler( TimerPrescaler_u16, 
                                           RLIN3_MAX_PRESCALER,
                                           &TimerPrescalerRegister_u08 );
  if( TimerPrescalerRegister_u08 > RLIN3_MAX_PRESCALER ) return( RLIN3_ERROR );
  
  TimerModulus_u16 = ( u16 )
    ( ( ( ( Oscillator_Frequency_flt * Factor_flt )
          / ( flt )TimerPrescaler_u16 ) ) / ( ( flt )Baudrate_u32 ) );

  if( TimerModulus_u16 < RLIN3_MIN_MODULUS )
  {
    return( RLIN3_ERROR );
  } 
    
  /* Baudrate setting */

  rlin3_p[ UnitNumber_u08 ]->lwbr.lprs  = TimerPrescalerRegister_u08;
  rlin3_p[ UnitNumber_u08 ]->lwbr.nspb  = SamplesPerBit_u08 - 1;  				//0xFF when master
  rlin3_p[ UnitNumber_u08 ]->lwbr.lwbr0 = LINStandard_u08;  					//Wake-up Baud Rate Select version LIN2.x
  
  if( OperationMode_u08 == RLIN3_MODE_LINMASTER )
  {
    if( ClockSelection_u08 == RLIN3_CLOCK_FD )
    {
      rlin3_p[ UnitNumber_u08 ]->brp1 = ( u08 )( TimerModulus_u16 );
    }
    else
    {
      rlin3_p[ UnitNumber_u08 ]->brp0 = ( u08 )( TimerModulus_u16 );
    }

    rlin3_p[ UnitNumber_u08 ]->lbfc.master.blt = BreakLength_u08;
    rlin3_p[ UnitNumber_u08 ]->lbfc.master.bdt = BreakDelimiter_u08;
  }
  else// LIN_SLAVE_MODE
  {
    rlin3_p[ UnitNumber_u08 ]->brp0 = ( u08 )( TimerModulus_u16 & 0x00FF );
    rlin3_p[ UnitNumber_u08 ]->brp1 = ( u08 )( TimerModulus_u16 >> 8 );

    rlin3_p[ UnitNumber_u08 ]->lbfc.slave.lblt = BreakDetectionLength_u08;
  }

  /* Other Parameters */

  rlin3_p[ UnitNumber_u08 ]->lmd.lios   = InterruptMode_u08;
  rlin3_p[ UnitNumber_u08 ]->lmd.lrdnfs = FilterMode_u08;
  rlin3_p[ UnitNumber_u08 ]->lmd.lcks   = ClockSelection_u08;
  rlin3_p[ UnitNumber_u08 ]->lsc.ibhs   = HeaderSpace_u08;
  rlin3_p[ UnitNumber_u08 ]->lsc.ibs    = InterByteSpace_u08;

  /* Activate Communication */
  
  rlin3_p[ UnitNumber_u08 ]->lmd.lmd         = OperationMode_u08;
  
  rlin3_p[ UnitNumber_u08 ]->lcuc.om         = RLIN3_MMODE_OPERATION;
  while( rlin3_p[ UnitNumber_u08 ]->lmst.om != RLIN3_MMODE_OPERATION );
  
  return( RLIN3_OK );
}

bit RLIN3_LINSendMessage( u08  UnitNumber_u08,
                          u08  PID_u08,
                          u08  MessageLength_u08,
                          u08  EnhancedCheckSum_u08,
                          u08  Direction_u08,
                          pu08 Data_pu08,
                          pu08 OperationMode_pu08 )
{
  u08  DataCounter_u08;
  u08  OperationMode_u08;
  u08  PIDCalc_u08;
  pu08 DataBufferPtr_pu08 = Data_pu08;
  u08  LDirection_u08     = Direction_u08;
  
  if( UnitNumber_u08 >= RLIN3_MACROS ) return( RLIN3_ERROR );
    
  OperationMode_u08 = rlin3_p[ UnitNumber_u08 ]->lmd.lmd;
  *OperationMode_pu08 = OperationMode_u08;
  
  if( OperationMode_u08 == RLIN3_MODE_LINMASTER ) 
  {
    if( rlin3_p[ UnitNumber_u08 ]->ltrc.fts == RLIN3_SET )
    {
      return( RLIN3_ERROR );
    }
  }
  else
  {
    if( rlin3_p[ UnitNumber_u08 ]->ltrc.rts == RLIN3_SET )
    {
      return( RLIN3_ERROR );
    }   
    
    LDirection_u08 = RLIN3_RESPONSE_TRANSMIT; 
  }

  if( LDirection_u08 == RLIN3_RESPONSE_TRANSMIT )
  {
    for( DataCounter_u08 = 0;
         DataCounter_u08 < MessageLength_u08;
         DataCounter_u08++ )
    {
      rlin3_p[ UnitNumber_u08 ]->ldb.buf8.buf[ DataCounter_u08 ] = 
        *DataBufferPtr_pu08;
        DataBufferPtr_pu08++;
    }
  }
  
  PID_u08 &= 0x3F;

  PIDCalc_u08 =    PID_u08 + ( PID_u08 >> 1 ) + 
                             ( PID_u08 >> 2 ) + 
                             ( PID_u08 >> 4 );
  PID_u08 |= ( ( PIDCalc_u08 & 0x01 ) << 6 );


  PIDCalc_u08 = ~( ( PID_u08 >> 1 ) + 
                   ( PID_u08 >> 3 ) + 
                   ( PID_u08 >> 4 ) + 
                   ( PID_u08 >> 5 ) );
  PID_u08 |= ( ( PIDCalc_u08 & 0x01 ) << 7 );

  rlin3_p[ UnitNumber_u08 ]->ldfc.lin.rfdl = MessageLength_u08;
  rlin3_p[ UnitNumber_u08 ]->ldfc.lin.rcds = LDirection_u08;
  rlin3_p[ UnitNumber_u08 ]->ldfc.lin.lcs  = EnhancedCheckSum_u08;
  rlin3_p[ UnitNumber_u08 ]->ldfc.lin.fsm  = RLIN3_MFRAME_COMBINED;
  rlin3_p[ UnitNumber_u08 ]->ldfc.lin.lss  = RLIN3_CLEAR;  /* only  max. 8 byte supported */
  
  rlin3_p[ UnitNumber_u08 ]->lidb.lin.id   = ( PID_u08 & RLIN3_PIDMASK );
  rlin3_p[ UnitNumber_u08 ]->lidb.lin.idp0 = ( PID_u08 >> RLIN3_PID_P0 );
  rlin3_p[ UnitNumber_u08 ]->lidb.lin.idp1 = ( PID_u08 >> RLIN3_PID_P1 );
  
  if( OperationMode_u08 == RLIN3_MODE_LINMASTER )  /* trigger transmission */
  {
    rlin3_p[ UnitNumber_u08 ]->ltrc.fts = RLIN3_SET;
  }
  else
  {
    rlin3_p[ UnitNumber_u08 ]->ltrc.rts = RLIN3_SET;
  }

  return( RLIN3_OK );
}

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
                              u08 Header_u08 )
{
  if( UnitNumber_u08 >= RLIN3_MACROS ) return( RLIN3_ERROR );

  rlin3_p[ UnitNumber_u08 ]->lie.ftcie = Transmit_u08;
  rlin3_p[ UnitNumber_u08 ]->lie.frcie = Receive_u08;
  rlin3_p[ UnitNumber_u08 ]->lie.errie = ErrorGlobal_u08;
  rlin3_p[ UnitNumber_u08 ]->lie.shie  = Header_u08;
  
  rlin3_p[ UnitNumber_u08 ]->lede.lin.bere  = ErrorBit_u08;
  rlin3_p[ UnitNumber_u08 ]->lede.lin.pbere = ErrorPhyBus_u08;
  rlin3_p[ UnitNumber_u08 ]->lede.lin.ftere = ErrorTimeout_u08;
  rlin3_p[ UnitNumber_u08 ]->lede.lin.fere  = ErrorFrame_u08;
  rlin3_p[ UnitNumber_u08 ]->lede.lin.sfere = ErrorSync_u08;
  rlin3_p[ UnitNumber_u08 ]->lede.lin.ipere = ErrorIDPar_u08;
  rlin3_p[ UnitNumber_u08 ]->lede.lin.ltes  = TimeoutSel_u08;

  return( RLIN3_OK );  
}

bit RLIN3_LINSReceiveHeader( u08 UnitNumber_u08 )
{
  if( UnitNumber_u08 > RLIN3_MACROS ) return( RLIN3_ERROR );

  if( ( rlin3_p[ UnitNumber_u08 ]->lmst.om & RLIN3_MMODE_RESET_MASK ) 
        != RLIN3_MMODE_RESET_MASK )
  {
    return( RLIN3_ERROR );
  }

  rlin3_p[ UnitNumber_u08 ]->ltrc.fts = RLIN3_SET;
  
  return( RLIN3_OK );    
}    

bit RLIN3_LINGetStatus( u08  UnitNumber_u08,
                        pu08 HeaderReceived_pu08,
                        pu08 DataReceived_pu08,
                        pu08 DataTransmitted_pu08,
                        pu08 PID_pu08 )
{
  if( UnitNumber_u08 >= RLIN3_MACROS ) return( RLIN3_ERROR );

  *HeaderReceived_pu08  = RLIN3_CLEAR;
  *DataReceived_pu08    = RLIN3_CLEAR;
  *DataTransmitted_pu08 = RLIN3_CLEAR;
  
  *PID_pu08 = ( rlin3_p[ UnitNumber_u08 ]->lidb.lin.id );

  if( rlin3_p[ UnitNumber_u08 ]->lst.b.htrc )
  {
    *HeaderReceived_pu08 = RLIN3_SET;
    rlin3_p[ UnitNumber_u08 ]->lst.b.htrc = RLIN3_CLEAR;
  }

  if( rlin3_p[ UnitNumber_u08 ]->lst.b.ftc )
  {
    *DataTransmitted_pu08 = RLIN3_SET;
    rlin3_p[ UnitNumber_u08 ]->lst.b.ftc = RLIN3_CLEAR;
  }
  
  if( rlin3_p[ UnitNumber_u08 ]->lst.b.frc )
  {
    *DataReceived_pu08 = RLIN3_SET;
    rlin3_p[ UnitNumber_u08 ]->lst.b.frc = RLIN3_CLEAR;
  }

  return( RLIN3_OK );
}

bit RLIN3_LINGetError( u08  UnitNumber_u08,
                       pu08 AnyError_pu08,
                       pu08 BitError_pu08,
                       pu08 DataConsistencyError_pu08,
                       pu08 TimeoutError_pu08,
                       pu08 FrameError_pu08,
                       pu08 SyncFieldError_pu08,
                       pu08 CheckSumError_pu08,
                       pu08 IdentParityError_pu08,
                       pu08 ResponsePreparationError_pu08 )
{
  if( UnitNumber_u08 >= RLIN3_MACROS ) return( RLIN3_ERROR );

  *BitError_pu08                 = rlin3_p[ UnitNumber_u08 ]->lest.lin.ber;
  *DataConsistencyError_pu08     = rlin3_p[ UnitNumber_u08 ]->lest.lin.pber;
  *TimeoutError_pu08             = rlin3_p[ UnitNumber_u08 ]->lest.lin.ter;
  *FrameError_pu08               = rlin3_p[ UnitNumber_u08 ]->lest.lin.fer;
  *SyncFieldError_pu08           = rlin3_p[ UnitNumber_u08 ]->lest.lin.sfer;
  *CheckSumError_pu08            = rlin3_p[ UnitNumber_u08 ]->lest.lin.cser;
  *IdentParityError_pu08         = rlin3_p[ UnitNumber_u08 ]->lest.lin.iper;
  *ResponsePreparationError_pu08 = rlin3_p[ UnitNumber_u08 ]->lest.lin.rper;
  
  if( ( *BitError_pu08                 == RLIN3_SET ) ||
      ( *DataConsistencyError_pu08     == RLIN3_SET ) ||
      ( *TimeoutError_pu08             == RLIN3_SET ) ||
      ( *FrameError_pu08               == RLIN3_SET ) ||
      ( *SyncFieldError_pu08           == RLIN3_SET ) ||
      ( *CheckSumError_pu08            == RLIN3_SET ) ||
      ( *IdentParityError_pu08         == RLIN3_SET ) ||
      ( *ResponsePreparationError_pu08 == RLIN3_SET ) )
  {
    *AnyError_pu08 = RLIN3_SET;
  }
  else
  {
    *AnyError_pu08 = RLIN3_CLEAR;
  }
  
  rlin3_p[ UnitNumber_u08 ]->lest.all = RLIN3_CLEAR;
  
  return( RLIN3_OK );
}

bit RLIN3_LINSReceiveAction( u08  UnitNumber_u08,
                             u08  ActionType_u08,
                             u08  MessageLength_u08,
                             u08  EnhancedCheckSum_u08 )
{
  if( UnitNumber_u08 >= RLIN3_MACROS ) return( RLIN3_ERROR );

  if( ( rlin3_p[ UnitNumber_u08 ]->lmst.om & RLIN3_MMODE_RESET_MASK ) 
        != RLIN3_MMODE_RESET_MASK )
  {
    return( RLIN3_ERROR );
  }

  switch( ActionType_u08 )
  {
    case RLIN3_A_SKIPMESSAGE:
      rlin3_p[ UnitNumber_u08 ]->ltrc.lnrr = RLIN3_SET;
      break;

    case RLIN3_A_GETMESSAGE:
      rlin3_p[ UnitNumber_u08 ]->ldfc.lin.rfdl = MessageLength_u08;
      rlin3_p[ UnitNumber_u08 ]->ldfc.lin.lcs  = EnhancedCheckSum_u08;
      rlin3_p[ UnitNumber_u08 ]->ldfc.lin.rcds = RLIN3_RESPONSE_RECEIVE;
      rlin3_p[ UnitNumber_u08 ]->ldfc.lin.lss  = RLIN3_CLEAR;
      rlin3_p[ UnitNumber_u08 ]->ltrc.rts = RLIN3_SET;
      break;
      
    case RLIN3_A_GETHEADER:
      rlin3_p[ UnitNumber_u08 ]->ldfc.lin.lcs = EnhancedCheckSum_u08;
      rlin3_p[ UnitNumber_u08 ]->ltrc.fts     = RLIN3_SET;
      break;
      
    default:
      return( RLIN3_ERROR );
  }
  
  return( RLIN3_OK );
}

bit RLIN3_LINReceiveMessage( u08  UnitNumber_u08,
                             u08  MessageLength_u08,
                             pu08 Data_pu08 )
{
  u08  DataCounter_u08;
  pu08 DataBufferPtr_pu08 = Data_pu08;
  
  if( UnitNumber_u08 >= RLIN3_MACROS ) return( RLIN3_ERROR );

  for( DataCounter_u08 = 0;
       DataCounter_u08 < MessageLength_u08;
       DataCounter_u08++ )
  {
    *DataBufferPtr_pu08 = rlin3_p[ UnitNumber_u08 ]->ldb.buf8.buf
                                 [ DataCounter_u08 ];
    DataBufferPtr_pu08++;
  }
   
  return( RLIN3_OK );
}

bit RLIN3_LINSendWakeup( u08 UnitNumber_u08,
                         u08 WakeupLength_u08 )
{
  if( UnitNumber_u08 >= RLIN3_MACROS ) return( RLIN3_ERROR );

  rlin3_p[ UnitNumber_u08 ]->lcuc.om = RLIN3_MMODE_RESET;
  
  while( ( rlin3_p[ UnitNumber_u08 ]->lmst.om & RLIN3_MMODE_RESET_MASK )
          != RLIN3_MMODE_RESET );

  rlin3_p[ UnitNumber_u08 ]->lwup.wutl = WakeupLength_u08;

  rlin3_p[ UnitNumber_u08 ]->lcuc.om = RLIN3_MMODE_WAKEUP;
  while( rlin3_p[ UnitNumber_u08 ]->lmst.om != RLIN3_MMODE_WAKEUP );
  
  rlin3_p[ UnitNumber_u08 ]->ldfc.lin.rcds = RLIN3_RESPONSE_TRANSMIT;
  rlin3_p[ UnitNumber_u08 ]->ltrc.fts      = RLIN3_SET;
  while( rlin3_p[ UnitNumber_u08 ]->ltrc.fts == RLIN3_SET );
  
  rlin3_p[ UnitNumber_u08 ]->lcuc.om = RLIN3_MMODE_RESET;
  
  while( ( rlin3_p[ UnitNumber_u08 ]->lmst.om & RLIN3_MMODE_RESET_MASK )
          != RLIN3_MMODE_RESET );

  rlin3_p[ UnitNumber_u08 ]->lcuc.om         = RLIN3_MMODE_OPERATION;
  while( rlin3_p[ UnitNumber_u08 ]->lmst.om != RLIN3_MMODE_OPERATION );

  return( RLIN3_OK );  
}

bit RLIN3_LINReceiveWakeup( u08 UnitNumber_u08 )
{
  if( UnitNumber_u08 >= RLIN3_MACROS ) return( RLIN3_ERROR );

  rlin3_p[ UnitNumber_u08 ]->lcuc.om = RLIN3_MMODE_RESET;
  
  while( ( rlin3_p[ UnitNumber_u08 ]->lmst.om & RLIN3_MMODE_RESET_MASK )
          != RLIN3_MMODE_RESET );

  rlin3_p[ UnitNumber_u08 ]->ldfc.lin.rcds = RLIN3_RESPONSE_RECEIVE;
  rlin3_p[ UnitNumber_u08 ]->ltrc.fts      = RLIN3_SET;

  return( RLIN3_OK );  
}

bit RLIN3_LINRestart( u08 UnitNumber_u08 )
{
  if( UnitNumber_u08 >= RLIN3_MACROS ) return( RLIN3_ERROR );

  rlin3_p[ UnitNumber_u08 ]->lcuc.om = RLIN3_MMODE_RESET;
  
  while( ( rlin3_p[ UnitNumber_u08 ]->lmst.om & RLIN3_MMODE_RESET_MASK )
          != RLIN3_MMODE_RESET );

  rlin3_p[ UnitNumber_u08 ]->lcuc.om         = RLIN3_MMODE_OPERATION;
  while( rlin3_p[ UnitNumber_u08 ]->lmst.om != RLIN3_MMODE_OPERATION );

  return( RLIN3_OK );  
}
