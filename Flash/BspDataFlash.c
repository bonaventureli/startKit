#include    "r_typedefs.h"
#include    "iodefine.h"
#include    "BspDataFlash.h"
#include 		"UserSys.h"

#define FLMD0_PROTECTION_OFF    (0x01)
#define FLMD0_PROTECTION_ON     (0x00)

#define FCU_FIRM_START_ADDRESS  (0x00017000)
#define FCU_RAM_START_ADDRESS   (0xFFA12000)
#define FCU_RAM_END_ADDRESS     (0xFFA12FFF)

#define FLASH_ACCESS_AREA	(*(volatile unsigned char *)0xFFA20000)
#define FLASH_ACCESS_AREA_SHORT	(*(volatile unsigned short *)0xFFA20000)

#define R_FPSYS_REGBIT_FSTATR_FRDY      (0x00008000)
#define R_FPSYS_REGBIT_FSTATR_ILGERR    (0x00004000)
#define R_FPSYS_REGBIT_FSTATR_DBFULL    (0x00000400)
#define R_FPSYS_REGBIT_ERROR_STATUS     (0x000774BF)


#define BLOCK_SIZE              (0x40)
#define DATAFLASH_START_ADDRESS (0xFF200000)
#define DATAFLASH_END_ADDRESS   (0xFF207FFF)

#if USE_DATA_FLASH

/******************************************************************************
* Function Name : void DataFlashWriteEnable(void)
* Description   : This function Enable Flash Write.
* Argument      : none
* Return Value  : none
******************************************************************************/
static void DataFlashWriteEnable(void)
{
	FACI.FENTRYR = 0xAA80;
  if(FACI.FENTRYR != 0x0080)
  {
      /* Error in Shift to Data flash P/E mode */
      __nop();
  }
}

/******************************************************************************
* Function Name : void DataFlashWriteDisable(void)
* Description   : This function Disable Flash Write.
* Argument      : none
* Return Value  : none
******************************************************************************/
static void DataFlashWriteDisable(void)
{
	FACI.FENTRYR = 0xAA00;
  if(FACI.FENTRYR != 0x0000)
  {
      /* Error in Shift to Read mode */
      __nop();
  }
}

/******************************************************************************
* Function Name : void EraseOneBlock(uint16_t  NumBlock)
* Description   : To Erase a Block.
* Argument      : NumBlock,which block will be Erased
* Return Value  : none
******************************************************************************/
static void EraseOneBlock(uint16_t  NumBlock)
{
	uint32_t StartAddress;
	uint32_t EndAddress;
	uint32_t  ulRegVal;
	
	StartAddress = 0xFF200000 + (NumBlock*0x40);
	EndAddress   = StartAddress + 0x3F;

	/* Blank check
  FBCCNT - Data Flash Blank Check Control Register
  b7:b1           - Reserved set to 0
  b0       BCDIR  - Blank check is executed from smaller address to larger address */
  FACI.FBCCNT  = 0x00;
  /* FSADDR - FACI Command Start Address Register
  b31:b0      FSADDR  - Start Address of FACI Command Processing ( Blank check ) */
  FACI.FSADDR  = StartAddress;//0xFF200000;
  /* FEADDR - FACI Command End Address Register
  b31:b0      FEADDR  - End Address of FACI Command */
  FACI.FEADDR  = EndAddress;//0xFF200004;
  /* Write to FACI command issue area ( Blank check command ) */
  FLASH_ACCESS_AREA   = 0x71;
  FLASH_ACCESS_AREA   = 0xD0;
  /* Wait for FACI command process */
  do
  {
      ulRegVal    = FACI.FSTATR.UINT32;
  }while(R_FPSYS_REGBIT_FSTATR_FRDY != (R_FPSYS_REGBIT_FSTATR_FRDY & ulRegVal));
  if(FACI.FASTAT & 0x10)
  {
      /* Indicates whether flash sequencer is in Command Lock state */
      __nop();
  }
  /* The target area is filled with 0s and/or 1s */
  if(FACI.FBCSTAT)
  {
      /* Block erase
      FCPSR - Flash Sequencer Process Switch Register
      b15:b1          - Reserved set to 0
      b0      ESUSPMD - Erasure-Suspended Mode - Suspension-priority mode */
      FACI.FCPSR   = 0x0000;
      /* FSADDR - FACI Command Start Address Register
      b31:b0      FSADDR  - Start Address of FACI Command Processing ( Block erase ) */
      FACI.FSADDR  = StartAddress;//0xFF200000;
      /* Write to FACI command issue area ( Block erase command ) */
      FLASH_ACCESS_AREA   = 0x20;
      FLASH_ACCESS_AREA   = 0xD0;
      /* Wait for FACI command process */
      do
      {
          ulRegVal    = FACI.FSTATR.UINT32;
      }while(R_FPSYS_REGBIT_FSTATR_FRDY != (R_FPSYS_REGBIT_FSTATR_FRDY & ulRegVal));
      if(FACI.FASTAT & 0x10)
      {
          /* Indicates whether flash sequencer is in Command Lock state */
          __nop();
      }
  }
}

/******************************************************************************
* Function Name : void PerWordWrite(uint32_t Address,uint32_t Data)
* Description   : To Flash Write a Word.
* Argument      : Address,Write to Address
									Data,which will be Writen to Flash
* Return Value  : none
******************************************************************************/
static void PerWordWrite(uint32_t Address,uint32_t Data)
{
	uint16_t   Temp[2],i;
	uint32_t    ulRegVal;
	
	Temp[0] = Data&0xFFFF;
	Temp[1] = (Data&0xFFFF0000)>>16;
	/* Program
  FSADDR - FACI Command Start Address Register
  b31:b0      FSADDR  - Start Address of FACI Command Processing ( Progarm ) */
  FACI.FSADDR  = Address;//0xFF200000;
  /* Write to FACI command issue area ( Program command ) */
  FLASH_ACCESS_AREA   = 0xE8;
  FLASH_ACCESS_AREA   = 0x02; /* Target Data flash ( 4Byte ) */
  for(i=0; i<0x02; i++)
  {
      FLASH_ACCESS_AREA_SHORT = Temp[i];
      /* Indicates the data buffer status when issuing Program */
      while(FACI.FSTATR.UINT32 & R_FPSYS_REGBIT_FSTATR_DBFULL)
      {
          __nop();
      }
  }
  FLASH_ACCESS_AREA   = 0xD0;
  do
  {
      ulRegVal    = FACI.FSTATR.UINT32;
  }while(R_FPSYS_REGBIT_FSTATR_FRDY != (R_FPSYS_REGBIT_FSTATR_FRDY & ulRegVal));
  if(FACI.FASTAT & 0x10)
  {
      /* Indicates whether flash sequencer is in Command Lock state */
      __nop();
  }
}

//��ָ����ַ��ʼд��ָ�����ȵ����ݣ��޼��飩
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToRead:��(32λ)��
static void DataFlashWriteNoCheck(uint32_t WriteAddress,uint32_t *pBuffer,uint16_t NumToWrite)
{
  uint16_t Temp;
  uint32_t Address = WriteAddress;
  for(Temp=0;Temp<NumToWrite;Temp++)
  {
    PerWordWrite(Address,pBuffer[Temp]);
    Address += 4;
  }
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToRead:��(32λ)��
void BspDataFlashRead(uint32_t FlashAddress,uint32_t *Data,uint32_t NumToRead)  
{
	uint32_t i ,FlashAddr = FlashAddress;
  for(i=0;i<NumToRead;i++)
  {
    Data[i] = *(volatile uint32_t *)FlashAddr; //��ȡ4���ֽ�.
    FlashAddr += 4; //ƫ��4���ֽ�.
  }
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToRead:��(32λ)��
void DataFlashWrite(uint32_t Address,uint32_t*Data,uint16_t Len)
{
	uint32_t StartAddress;
	
	StartAddress = Address;
	
	if((Address < 0xFF200000)||(Address > 0xFF207FFF))
	return;

	DataFlashWriteEnable();
	EraseOneBlock(0);
	EraseOneBlock(1);
	EraseOneBlock(2);
	
  DataFlashWriteNoCheck(StartAddress,Data,Len);

	DataFlashWriteDisable();

}

uint32_t gDataFlashBuff[BLOCK_SIZE/4];
/*******************************************************************
�� �� ���� BspDataFlashWrite(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)	 
��    �ܣ� �ڲ�Flashд��������ָ����ַ��ʼд��ָ�����ȵ�����/
��    ���� WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!) pBuffer:����ָ�� NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
��    �أ� ��
�� �� �ˣ� ��ͥ��
�޶���¼�� 2017/10/20����
********************************************************************/
void BspDataFlashWrite(uint32_t WriteAddr,uint32_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t secpos;	    //������ַ
	uint16_t secoff;	    //������ƫ�Ƶ�ַ(16λ�ּ���)
	uint16_t secremain;   //������ʣ���ַ(16λ�ּ���)	   
 	uint16_t i;    
	uint32_t offaddr;     //ȥ��0X08000000��ĵ�ַ
  
  /* ��ַ���� */
	if((WriteAddr < DATAFLASH_START_ADDRESS)||(WriteAddr >= DATAFLASH_END_ADDRESS))return;//�Ƿ���ַ
	  
	DataFlashWriteEnable();					          //����
	offaddr   = WriteAddr-DATAFLASH_START_ADDRESS;		//ʵ��ƫ�Ƶ�ַ.
	secpos    = offaddr/BLOCK_SIZE;			    //�������  0~512 for RH850
	secoff    = (offaddr%BLOCK_SIZE)/4;		  //�������ڵ�ƫ��(4���ֽ�Ϊ������λ.)
	secremain = BLOCK_SIZE/4 - secoff;		  //����ʣ��ռ��С  
  
	if(NumToWrite <= secremain) secremain = NumToWrite;//�����ڸ�������Χ
  
	while(1) 
	{	
		BspDataFlashRead(secpos*BLOCK_SIZE+DATAFLASH_START_ADDRESS,gDataFlashBuff,BLOCK_SIZE/4);//������������������
		__nop();
		for(i=0;i<secremain;i++)	//У������
		{
			if(gDataFlashBuff[secoff+i]!=0XFFFFFFFF)break;//��Ҫ����  	  
		}
    /* ��Ҫ���� */
		if(i<secremain)				
		{
			EraseOneBlock(secpos);	//�����������
			__nop();           	//�ȴ��ϴβ������
			
			for(i=0;i<secremain;i++)//����
			{
				gDataFlashBuff[i+secoff] = pBuffer[i];	  
			}
			DataFlashWriteNoCheck(secpos*BLOCK_SIZE+DATAFLASH_START_ADDRESS,gDataFlashBuff,BLOCK_SIZE/4);//д����������  
		}else 
		{
			DataFlashWriteNoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 
		}
    __nop();
		if(NumToWrite == secremain) break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff = 0;			//ƫ��λ��Ϊ0 	 
		  pBuffer += secremain;  	//ָ��ƫ��
			WriteAddr  += secremain*4;	//д��ַƫ��(32λ���ݵ�ַ,��Ҫ*4)	   
		  NumToWrite -= secremain;	  //�ֽ�(32λ)���ݼ�
			
      __nop();
			if(NumToWrite > (BLOCK_SIZE/4)) 
			{
				secremain = BLOCK_SIZE/4;//��һ����������д����
				
			}
			else 
			{
				secremain = NumToWrite;//��һ����������д����
				__nop();
			}
		}	 
	};	
	DataFlashWriteDisable();		//����
}

void DataFlashTest(void)
{
	DataFlashWriteEnable();
	
		EraseOneBlock(1);
		EraseOneBlock(2);
		EraseOneBlock(3);
		EraseOneBlock(4);
		EraseOneBlock(5);
		EraseOneBlock(6);
		EraseOneBlock(7);
		EraseOneBlock(8);
		EraseOneBlock(9);
	DataFlashWriteDisable();
}

#define RAMSIZE  256
static uint8_t RAMTempData[RAMSIZE];

int SdkWrite(uint8_t *InData,uint8_t Wlen,uint8_t offset)
{
	BspDataFlashRead(0xFF200000,(uint32_t*)RAMTempData,RAMSIZE/4);
	memcpy(RAMTempData+offset,InData,Wlen);
	BspDataFlashWrite(0xFF200000,(uint32_t*)RAMTempData,RAMSIZE/4);
	return 0;
}

int SdkRead(uint8_t *OutData,uint8_t Rlen,uint8_t offset)
{
	BspDataFlashRead(0xFF200000,(uint32_t*)RAMTempData,RAMSIZE/4);
	memcpy(OutData,&RAMTempData[offset],Rlen);
	return 0;
}

#pragma section text "FACI_RAM_CODE"
/******************************************************************************
* Function Name : void BspFlashInit( void )
* Description   : This function initializes the FACI macro.
* Argument      : none
* Return Value  : none
******************************************************************************/
void BspFlashInit( void )
{
    volatile uint8_t    ucDummyVal;
    volatile uint32_t   ulDummyVal;
    uint32_t            ulRegVal;
    uint32_t            *tAdd, *rAdd;
    
    /* FLMD0 pull-up
    FLMDCNT - This register specifies the internal pull-up or pull-down of the FLMD0 pin
    b31:b1              - Reserved set to 0
    b0        FLMDPUP   - FLMDPUP FLMD0 Pin Software Control - Pull-up selected */
    FLMDPCMD = 0xa5;                        /* Protection release the FLMDCNT register */
    FLMDCNT  = FLMD0_PROTECTION_OFF;
    FLMDCNT  = ~FLMD0_PROTECTION_OFF;
    FLMDCNT  = FLMD0_PROTECTION_OFF;
    /* Wait for transitions to the code flash P/E mode are enabled */
    while(!FACI.FPMON)
    {
        __nop();
    }
    
    /* ID authentication
    SELFIDx ( x = 0 - 3 ) - Self-Programming ID Input Registers
    b31:b0                  SELFIDn - ID for Use in Authentication of Self-Programming */
    SELF.ID0 = 0xFFFFFFFF;
    SELF.ID1 = 0xFFFFFFFF;
    SELF.ID2 = 0xFFFFFFFF;
    SELF.ID3 = 0xFFFFFFFF;
    ulDummyVal = SELF.ID3;
    if(SELF.IDST.UINT32)
    {
        /* Error in the IDs do not match ( ID-based security is locked ) */
        __nop();
    }
    
    /* FCU firmware transfer
    Check to flash mode */
    if(FACI.FENTRYR != 0x0000)
    {
        /* Shift to read mode
        FENTRYR - Program/Erase Mode Entry Register
        b15:b8    KEY       - Writing to this bit is enabled only when AAH is written to the KEY bits
        b7        FENTRYD   - Data Flash Program/Erasure Mode Entry - Set to 0
        b6:b1               - Reserved set to 0
        b0        FENTRYC   - Code Flash Program/Erasure Mode Entry - Set to 0 */
        FACI.FENTRYR = 0xAA00;
    }
    /* FCU firmware transfer
    FCURAME - FCURAME enables or disables access to FCURAM area
    b15:b8    KEY       - Writing to this bit is enabled only when C4H is written to the KEY bits
    b7:b2               - Reserved set to 0
    b1        FRAMTRAN  - Specifies the FCURAM transfer mode - Normal transfer mode
    b0        FCRME     - Enables or disables access to the FCURAM - Enables access to FCURAM */
    FACI.FCURAME = 0xC401;
    /* Select the FCU firmware storage area
    FCUFAREA - FCUFAREA selects the FCU firmware storage area
    b7:b1               - Reserved set to 0
    b0         FCUFSEL  - Firmware Storage Area Select - The firmware storage area is assigned to 00017000H to 00017FFFH */
    CCIB.FCUFAREA    = 0x01;
    /* Update of FCUFAREA register */
    ucDummyVal  = CCIB.FCUFAREA;
    PFSS.BWCBUFEN = 0x00;
    PFSS.BWCBUFEN = 0x01;
    PFSS.BWCBUFEN = 0x00;
    ucDummyVal  = PFSS.BWCBUFEN;
    __synci();
    /* Copy the data from the FCU firmware storage area to FCURAM */
    tAdd = (uint32_t *)FCU_FIRM_START_ADDRESS;
    rAdd = (uint32_t *)FCU_RAM_START_ADDRESS;
    while(rAdd < (uint32_t *)FCU_RAM_END_ADDRESS)
    {
        *rAdd = *tAdd;
        tAdd++;
        rAdd++;
    }
    /* Select user's area
    FCUFAREA - FCUFAREA selects the FCU firmware storage area
    b7:b1               - Reserved set to 0
    b0         FCUFSEL  - Firmware Storage Area Select - The user area is assigned to 00017000H to 00017FFFH */
    CCIB.FCUFAREA    = 0x00;
    /* Update of FCUFAREA register */
    ucDummyVal  = CCIB.FCUFAREA;
    PFSS.BWCBUFEN = 0x00;
    PFSS.BWCBUFEN = 0x01;
    PFSS.BWCBUFEN = 0x00;
    ucDummyVal  = PFSS.BWCBUFEN;
    __synci();
    /* FCURAM access disable
    FCURAME - FCURAME enables or disables access to FCURAM area
    b15:b8    KEY       - Writing to this bit is enabled only when C4H is written to the KEY bits
    b7:b2               - Reserved set to 0
    b1        FRAMTRAN  - Specifies the FCURAM transfer mode - Normal transfer mode
    b0        FCRME     - Enables or disables access to the FCURAM - Disables access to FCURAM */
    FACI.FCURAME = 0xC400;
    /* Shift to P/E mode
    FENTRYR - Program/Erase Mode Entry Register
    b15:b8    KEY       - Writing to this bit is enabled only when AAH is written to the KEY bits
    b7        FENTRYD   - Data flash is in Program/Erasure mode
    b6:b1               - Reserved set to 0
    b0        FENTRYC   - Code Flash Program/Erasure Mode Entry - Set to 0 */
    FACI.FENTRYR = 0xAA80;
    if(FACI.FENTRYR != 0x0080)
    {
        /* Error in Shift to P/E mode */
        __nop();
    }
    /* Issue forced stop command */
    /* Clear the ECC error flag in FCURAM */
    FLASH_ACCESS_AREA   = 0xB3;
    do
    {
        ulRegVal    = FACI.FSTATR.UINT32;
    }while(R_FPSYS_REGBIT_FSTATR_FRDY != (R_FPSYS_REGBIT_FSTATR_FRDY & ulRegVal));
    if(FACI.FASTAT & 0x10)
    {
        /* Indicates whether flash sequencer is in Command Lock state */
        __nop();
    }
    /* Shift to read mode
    FENTRYR - Program/Erase Mode Entry Register
    b15:b8    KEY       - Writing to this bit is enabled only when AAH is written to the KEY bits
    b7        FENTRYD   - Data Flash Program/Erasure Mode Entry - Set to 0
    b6:b1               - Reserved set to 0
    b0        FENTRYC   - Code Flash Program/Erasure Mode Entry - Set to 0 */
    FACI.FENTRYR = 0xAA00;
    if(FACI.FENTRYR != 0x0000)
    {
        /* Error in Shift to read mode */
        __nop();
    }
}



#endif






