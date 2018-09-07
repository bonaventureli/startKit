#include    "Kit1.h"
#include    "r_typedefs.h"
#include    "iodefine.h"

uint16_t Strcmp(uint8_t *Data1,uint8_t *Data2,uint16_t Len)
{
	uint16_t temp;
	for(temp = 0;temp < Len; temp++)
	{
		if(Data1[temp] != Data2[temp])
		{
			return 1;
		}
	}
	return 0;
}

/* Used to Caculate the len of "0x0D,0x0A" */
uint16_t CaculateDataLen(uint8_t *Data,uint16_t Len)
{
  uint16_t Shift;
  uint16_t Temp;
  
  for(Temp=0;Temp<Len;Temp++)
  {
      if(Data[Temp] == 0x0D)
      {
        if(Data[Temp+1] == 0x0A)
        {
          Shift = Temp+2;
          break;
        }
      }
      else
      {
        
      }  
  }
	return Shift;
}

void BspPinInit(void)
{
	PMC8  &= 0xFF0FU; /* port mode */
	PM8   &= 0xFF0FU; /* input port */
	P8    &= 0xFF0FU; /* outpu value */
}

void BspPinToggle(void)
{
	PNOT8 = 0x00F0L;                         /* P0_14:CN2C-72pin */
}

static uint32_t gSysTimeMs = 0;
static uint32_t gSysTimeSec = 0;
/* Add to ISR 0x00001000UL:  INTTAUD0I0 */ 
void SysTimeInc(void)
{
	gSysTimeMs++;

  if(gSysTimeMs%1000)
  {
    gSysTimeSec++;
  }
}

uint32_t GetTimeMs(void)
{
	return gSysTimeMs;
}

uint32_t GetTimeSec(void)
{
	return gSysTimeSec;
}