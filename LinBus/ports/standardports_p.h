#ifndef _STANDARDPORTS_P_H
#define _STANDARDPORTS_P_H

#include <ree_types.h>

extern bit PORT_GetStatus( u16  PortNumber_u16,
                           u16  BitSpecification_u16,
                           pu08 PortDirection_pu08,
                           pu08 PeripheralMode_pu08,
                           pu08 AlternateFunction_pu08 );

extern bit PORT_Enable( u16 PortNumber_u16,
                        u16 BitSpecification_u16,
                        u08 PortDirection_u08,
                        u08 PeripheralMode_u08,
                        u08 AlternateFunction_u08 );

extern bit PORT_Disable( u16 PortNumber_u16,
                         u16 BitSpecification_u16 );

extern bit PORT_Write( u16 PortNumber_u16,
                       u16 PortData_u16 );

extern bit PORT_Read( u16  PortNumber_u16,
                      pu16 PortData_pu16 );

extern bit PORT_WriteBit( u16 PortNumber_u16,
                          u16 BitSpecification_u16,
                          u16 BitValue_u16 );

extern bit PORT_ReadBit( u16  PortNumber_u16,
                         u16  BitSpecification_u16,
                         pu16 BitValue_u16 );

extern bit PORT_Oscillator_Switch_Setup( );

#endif
