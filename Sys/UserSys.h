#ifndef _USER_SYS_H_
#define _USER_SYS_H_

#include "BspTimer.h"
#define 	USE_DATA_FLASH            0

#define 	STATUS_KEY        				1
#define 	STATUS_DOOR 							2
#define 	STATUS_WINDOWS						3

/*
* change auther: lifei 
* change time2018/8/31
* add 
BOARD_NO
BOARD_DEVELOP
BOARD_DEMO
*/

/*step1:  select Board*/
#define 	BOARD_NO 										  0
#define 	BOARD_DEVELOP									0
#define 	BOARD_DEMO 									  1

/*step2:	select task*/
#define 	taskli 												0
#define 	taskhuang 										0
#define	  APPCAN												0
#define	  APPLIN												0
#define	  APPUART												1
#define	  APPTIME												0

/*step3:  select DRIVERUART*/
/*hardware developboard   PIN1_1(TX)   PIN1_0(RX) test ok*/
/*hardware demoboard		  PIN11_5(TX)  PIN11_6(RX) test ok*/
#define   DRIVERUART3											1
/*hardware developboard  TX PIN0_2  RX PIN0_3 test not ok*/
#define   DRIVERUART0											0   

/*step4:  select DRIVERCAN*/
	/* P10_0 (CAN0RX) */
  /* P10_1 (CAN0TX) */
#define   DEVELOPCAN											1
	/* P0_1 (CAN0RX) */
  /* P0_0 (CAN0TX) */
#define   DEMOCAN													0   


typedef struct{
uint8_t IsKeyInside;
uint8_t IsDoorOpen;
uint8_t IsWindowsOpen;
/*...*/

}Car_StatusType;


void SetKeyStatus(uint8_t Value);
uint8_t GetCarStatus(uint8_t Item);


#endif