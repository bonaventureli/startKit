#ifndef    _KIT_1_H_
#define    _KIT_1_H_

#include "r_typedefs.h"
/******************************************************************************
Typedef definitions
******************************************************************************/
#if 0
typedef signed   char      int8_t;
typedef signed   short     int16_t;
typedef signed   long      int32_t;
typedef signed   long long int64_t;

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned long      uint32_t;
typedef unsigned long long uint64_t;
#endif

uint16_t Strcmp(uint8_t *Data1,uint8_t *Data2,uint16_t Len);
uint16_t CaculateDataLen(uint8_t *Data,uint16_t Len);

void BspPinInit(void);
void BspPinToggle(void);

void SysTimeInc(void);
uint32_t GetTimeMs(void);
uint32_t GetTimeSec(void);

#endif

