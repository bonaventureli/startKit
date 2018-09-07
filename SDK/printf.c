#include "printf.h"
#include "Uart33.h"

#include <stdarg.h> 
#include "stdarg.h"
//typedef struct __va_list { void *__ap; } va_list;
//static char sprint_buf[300];
//void DebugPrintf(unsigned char *Data,unsigned char Len);
void DebugPrintf(const char *fmt, int len)
{
	//Uart3Sent(fmt,len);
	
}
