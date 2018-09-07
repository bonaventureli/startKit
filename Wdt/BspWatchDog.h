#ifndef _BSP_WATCH_DOG_H_
#define _BSP_WATCH_DOG_H_    
				
void    R_WDT_Init( void );
void    R_WDT_Clear( void );
void    ISRWDTCallBack(uint32_t regEIIC_value);
				
#endif