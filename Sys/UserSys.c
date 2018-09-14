#include "UserSys.h"

uint8_t gKeyStatus = 0;

/******************************************************************************
* Function Name : SetKeyStatus
* Description   : This function is set Key Status.
* Argument      : Key status
* Return Value  : none
******************************************************************************/
void SetKeyStatus(uint8_t Value)
{
	gKeyStatus = Value;
}

/******************************************************************************
* Function Name : GetKeyStatus
* Description   : This function is get Car Status.
* Argument      : Item the classic
* Return Value  : the value of Item
******************************************************************************/
uint8_t GetKeyStatus(void)
{
	return gKeyStatus;
}