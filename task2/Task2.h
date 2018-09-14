#ifndef _MAIN_TASK_H_
#define _MAIN_TASK_H_

#include "r_typedefs.h"

#define STRING_UARTRX_LEN 100

void Task2(void);

void DataUnpackHandle(uint8_t *Data,uint16_t Len);
void DecodeATCmdTask(uint8_t* Data,uint16_t Len);
void ExcuteATCmdTask(uint8_t Cmd);



#endif
