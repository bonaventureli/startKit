#ifndef _MAP_RLIN3_H
#define _MAP_RLIN3_H

#include <standardports.h>

/* MAPPED RLIN3: RLIN3_UCIAPRLN */

#include <map_device.h>
#include <rlin3.h>

#ifndef DRIVER_LOCAL
#include <rlin3_p.h>
#include <rlin3_as.h>
#endif

/* FURTHER RLIN3 OPERATION PROPERTIES */

#define RLIN3_BASE              ( 0xFFCF0000 )
#define RLIN3_OFFSET_CHANNEL0   ( 0x00000000 )
#define RLIN3_OFFSET_CHANNEL1   ( 0x00000040 )
#define RLIN3_OFFSET_CHANNEL2   ( 0x00000080 )
#define RLIN3_OFFSET_CHANNEL3   ( 0x000000C0 )
#define RLIN3_OFFSET_CHANNEL4   ( 0x00000100 )
#define RLIN3_OFFSET_CHANNEL5   ( 0x00000140 )


#define RLIN3_FREQFACTOR        ( ( UnitNumber_u08 == 0 ) ? 0.5 : 0.25 )

/* Baudrate limits */

#define RLIN3_MINBAUDRATE       ( 100L )
#define RLIN3_MAXBAUDRATE       ( 2000000L )

/* Maximum Waiting Loop Counts */

#define RLIN3_SENDTIMEOUT       ( 1000000L )


/* PORT DEFINITIONS */

#define RLIN3_PORT_RXD0         PORT_0
#define RLIN3_PORT_BIT_RXD0     BIT_3 
#define RLIN3_PORT_TXD0         PORT_0
#define RLIN3_PORT_BIT_TXD0     BIT_2
#define RLIN3_PORT_FUNC_RXD0    PORT_FUNCTION_ALTLV1
#define RLIN3_PORT_FUNC_TXD0    PORT_FUNCTION_ALTLV1

/*
#define RLIN3_PORT_RXD0         PORT_10
#define RLIN3_PORT_BIT_RXD0     BIT_9 
#define RLIN3_PORT_TXD0         PORT_10
#define RLIN3_PORT_BIT_TXD0     BIT_10
#define RLIN3_PORT_FUNC_RXD0    PORT_FUNCTION_ALTLV1
#define RLIN3_PORT_FUNC_TXD0    PORT_FUNCTION_ALTLV1
*/
/*
#define RLIN3_PORT_RXD1         PORT_0
#define RLIN3_PORT_BIT_RXD1     BIT_4 
#define RLIN3_PORT_TXD1         PORT_0
#define RLIN3_PORT_BIT_TXD1     BIT_5
#define RLIN3_PORT_FUNC_RXD1    PORT_FUNCTION_ALTLV0
#define RLIN3_PORT_FUNC_TXD1    PORT_FUNCTION_ALTLV0
*/

/*
#define RLIN3_PORT_RXD2         
#define RLIN3_PORT_BIT_RXD2      
#define RLIN3_PORT_TXD2         
#define RLIN3_PORT_BIT_TXD2     
#define RLIN3_PORT_FUNC_RXD2    
#define RLIN3_PORT_FUNC_TXD2    
*/

/* INTERRUPT REGISTER MAPPING */

#define RLIN3_INTM0TX           ICRLIN30UR0
#define RLIN3_INTM0RX           ICRLIN30UR1
#define RLIN3_INTM0ERR          ICRLIN30UR2
#define RLIN3_INTM0COM          ICRLIN30
#define RLIN3_INTM1TX           ICRLIN31UR0
#define RLIN3_INTM1RX           ICRLIN31UR1
#define RLIN3_INTM1ERR          ICRLIN31UR2
#define RLIN3_INTM1COM          ICRLIN31
#define RLIN3_INTM2TX           ICRLIN32UR0
#define RLIN3_INTM2RX           ICRLIN32UR1
#define RLIN3_INTM2ERR          ICRLIN32UR2
#define RLIN3_INTM2COM          ICRLIN32



#define RLIN3_INTCLEAR         ( 0x008F )       // Interrupt mask bit
#define RLIN3_INTENABLEDEFAULT ( 0x0040 )
#define RLIN3_INTFLAG          ( 0x1000 )


/* The RLIN3 Memory Mapping */  

static struct rlin3 *rlin3_p[ RLIN3_MACROS ] =
{
    ( rlin3 * )( RLIN3_BASE + RLIN3_OFFSET_CHANNEL0 ),              /* unit 0 */
    ( rlin3 * )( RLIN3_BASE + RLIN3_OFFSET_CHANNEL1 ),              /* unit 1 */
    ( rlin3 * )( RLIN3_BASE + RLIN3_OFFSET_CHANNEL2 )               /* unit 2 */
};   


#endif
