#ifndef __BSP_TIMER_
#define __BSP_TIMER_

#include    "r_typedefs.h"
#include    "iodefine.h"

void    R_TAUD_Init( void );
void    R_TAUD_Start( void );

void IntTimerCallback( uint32_t regEIIC_value );

#endif