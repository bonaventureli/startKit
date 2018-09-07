#include "UserSys.h"

Car_StatusType gsCarStatus;

/******************************************************************************
* Function Name : SetKeyStatus
* Description   : This function is set Key Status.
* Argument      : Key status
* Return Value  : none
******************************************************************************/
void SetKeyStatus(uint8_t Value)
{
	gsCarStatus.IsKeyInside = Value;
}

/******************************************************************************
* Function Name : GetCarStatus
* Description   : This function is get Car Status.
* Argument      : Item the classic
* Return Value  : the value of Item
******************************************************************************/
uint8_t GetCarStatus(uint8_t Item)
{
	uint8_t Retval;
	
	switch(Item)
	{
		case STATUS_KEY:
			Retval = gsCarStatus.IsKeyInside;
		break;
		case STATUS_DOOR:
			Retval = gsCarStatus.IsDoorOpen;
		break;
		case STATUS_WINDOWS:
			Retval = gsCarStatus.IsWindowsOpen;
		break;
		default:
		break;
	}
	return Retval;
}
