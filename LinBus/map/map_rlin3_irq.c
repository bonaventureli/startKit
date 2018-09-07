#include <rlin3.h>

extern void RLIN3_IRQ_Common( u08 );
extern void RLIN3_IRQ_Send( u08 );
extern void RLIN3_IRQ_Receive( u08 );
extern void RLIN3_IRQ_Error( u08 );
#ifdef __ghs__
__interrupt void INTRLIN30UR0( void );
__interrupt void INTRLIN30UR1( void );
__interrupt void INTRLIN30UR2( void );

__interrupt void INTRLIN31( void );
__interrupt void INTRLIN31UR0( void );
__interrupt void INTRLIN31UR1( void );
__interrupt void INTRLIN31UR2( void );

__interrupt void INTRLIN30UR0( void )
{
  RLIN3_IRQ_Send( RLIN3_0 );
}

__interrupt void INTRLIN30UR1( void )
{
  RLIN3_IRQ_Receive( RLIN3_0 );
}

__interrupt void INTRLIN30UR2( void )
{
  RLIN3_IRQ_Error( RLIN3_0 );
}

__interrupt void INTRLIN31( void )
{
  RLIN3_IRQ_Common( RLIN3_1 );
}

__interrupt void INTRLIN31UR0( void )
{
  RLIN3_IRQ_Send( RLIN3_1 );
}

__interrupt void INTRLIN31UR1( void )
{
  RLIN3_IRQ_Receive( RLIN3_1 );
}

__interrupt void INTRLIN31UR2( void )
{
  RLIN3_IRQ_Error( RLIN3_1 );
}

#else

/******************************************************************************
* Function Name : void INTRLIN30UR0 (void);
* Description   : This function is INTRLIN30UR0 interrupt processing. (table refer.)
*               : INTRLIN30UR0
* Argument      : none
* Return Value  : none
******************************************************************************/
#pragma interrupt INTRLIN30UR0 ( enable = false, channel = 26, callt = false, fpu = false )
void INTRLIN30UR0 (void);
#pragma interrupt INTRLIN30UR1 ( enable = false, channel = 27, callt = false, fpu = false )
void INTRLIN30UR1 (void);
#pragma interrupt INTRLIN30UR2 ( enable = false, channel = 28, callt = false, fpu = false )
void INTRLIN30UR2 (void);
#pragma interrupt INTRLIN31    ( enable = false, channel = 112, callt = false, fpu = false )
void INTRLIN31    (void);
#pragma interrupt INTRLIN31UR0 ( enable = false, channel = 113, callt = false, fpu = false )
void INTRLIN31UR0 (void);
#pragma interrupt INTRLIN31UR1 ( enable = false, channel = 114, callt = false, fpu = false )
void INTRLIN31UR1 (void);
#pragma interrupt INTRLIN31UR2 ( enable = false, channel = 115, callt = false, fpu = false )
void INTRLIN31UR2 (void);


void INTRLIN30UR0( void )
{
  RLIN3_IRQ_Send( RLIN3_0 );
}

void INTRLIN30UR1( void )
{
  RLIN3_IRQ_Receive( RLIN3_0 );
}

void INTRLIN30UR2( void )
{
  RLIN3_IRQ_Error( RLIN3_0 );
}

void INTRLIN31( void )
{
  RLIN3_IRQ_Common( RLIN3_1 );
}

void INTRLIN31UR0( void )
{
  RLIN3_IRQ_Send( RLIN3_1 );
}

void INTRLIN31UR1( void )
{
  RLIN3_IRQ_Receive( RLIN3_1 );
}

void INTRLIN31UR2( void )
{
  RLIN3_IRQ_Error( RLIN3_1 );
}

#endif









