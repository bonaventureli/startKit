/**
  ******************************************************************************
  * @file    printf.h
  * @author  bPanda Team
  * @version V1.0.0
  * @date    13-March-2018
  * @brief   
  ******************************************************************************
  * @attention
  *         
  ******************************************************************************
  */


#ifndef __PRINTF_H
#define __PRINTF_H

//#include "queue.h"

#define dbg_printf          DEBUG_PRINTF

#ifdef  DEBUG
#define DEBUG_PRINTF(format)    (DebugPrintf ## format)
#else
#define DEBUG_PRINTF(format)    
#endif

void DebugPrintfInit(void);
//void DebugPrintf(const u8_t *format, ...);
//void assert_failed(const s8_t* file, u32_t line);


#endif
