#ifndef _STANDARDPORTS_H
#define _STANDARDPORTS_H
            
#include <ree_types.h>
             
#define PORT_OK      true
#define PORT_ERROR   false

#define PORT_NUMERIC            /* PORT DEFINITIONS */

#define PORT_0   0x0000         
#define PORT_1   0x0001
#define PORT_2   0x0002
#define PORT_3   0x0003
#define PORT_4   0x0004
#define PORT_5   0x0005
#define PORT_6   0x0006
#define PORT_7   0x0007
#define PORT_8   0x0008
#define PORT_9   0x0009
#define PORT_10   0x000A
#define PORT_11   0x000B
#define PORT_12   0x000C
#define PORT_13   0x000D
#define PORT_14   0x000E
#define PORT_15   0x000F
#define PORT_16   0x0010
#define PORT_17   0x0011
#define PORT_20   0x0014
#define PORT_24   0x0018
#define PORT_25   0x0019
#define PORT_29   0x001D

#define PORT_ALPHA

#define PORT_DL  0x0100
#define PORT_DH  0x0101

#define PORT_AL  0x0200
#define PORT_AH  0x0201

#define PORT_CS  0x0300
#define PORT_CT  0x0301
#define PORT_CM  0x0302
#define PORT_CD  0x0303

#define PORT_NOPORT 0xFFFF

#define BIT_0    0x01         /* BIT DEFINITIONS */
#define BIT_1    0x02
#define BIT_2    0x04
#define BIT_3    0x08
#define BIT_4    0x10
#define BIT_5    0x20
#define BIT_6    0x40
#define BIT_7    0x80

#define BIT_8    0x0100
#define BIT_9    0x0200
#define BIT_10   0x0400
#define BIT_11   0x0800
#define BIT_12   0x1000
#define BIT_13   0x2000
#define BIT_14   0x4000
#define BIT_15   0x8000  

#define BIT_ALL16 0xFFFF
#define BIT_ALL8  0xFF
#define BIT_NONE  0x0000

#define BIT_0_P  0
#define BIT_1_P  1
#define BIT_2_P  2
#define BIT_3_P  3
#define BIT_4_P  4
#define BIT_5_P  5
#define BIT_6_P  6
#define BIT_7_P  7

#define BIT_8_P  8
#define BIT_9_P  9
#define BIT_10_P 10
#define BIT_11_P 11
#define BIT_12_P 12
#define BIT_13_P 13
#define BIT_14_P 14
#define BIT_15_P 15


#define PORT_DIR_INPUT  1       /* Port Direction */
#define PORT_DIR_OUTPUT 0
#define PORT_DIR_KEEP   2

#define PORT_MODE_IO         0  /* Port Mode Control */
#define PORT_MODE_PERIPHERAL 1
#define PORT_MODE_KEEP       2

#define PORT_FUNCTION_NORMAL  0x00 /* Port Function Control */
#define PORT_FUNCTION_ALTERN  0x01
#define PORT_FUNCTION_ALTERN2 0x10
#define PORT_FUNCTION_ALTERN4 0x20
#define PORT_FUNCTION_ALTLV0  ( PORT_FUNCTION_NORMAL )
#define PORT_FUNCTION_ALTLV1  ( PORT_FUNCTION_ALTERN )
#define PORT_FUNCTION_ALTLV2  ( PORT_FUNCTION_ALTERN2 )
#define PORT_FUNCTION_ALTLV3  ( PORT_FUNCTION_ALTERN | PORT_FUNCTION_ALTERN2 )
#define PORT_FUNCTION_ALTLV4  ( PORT_FUNCTION_ALTERN4 )
#define PORT_FUNCTION_KEEP    0xFF

#endif
