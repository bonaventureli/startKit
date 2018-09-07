#ifndef _OSTM0_H
#define _OSTM0_H

void OSTM0_INIT(void);
void OSTM0_DEINIT(void);
void OSTM0_START(void);
void OSTM0_enable_int(void);
void OSTM0_STOP(void);
void FEINT_interrupt( unsigned long );

#endif
