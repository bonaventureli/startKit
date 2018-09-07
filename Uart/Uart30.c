//============================================================================
//
// Source code for the UART Functions
//
//============================================================================

//============================================================================
// Includes
//============================================================================
//#include "device.h"
#include "Uart30.h"
#include "iodefine.h"
#include "r_typedefs.h"


static uint32_t gUart0RecLen = 0;
static uint8_t  gUart0RecData[UART0_BUFF_LEN];

static uint32_t gUart0DataCacheLen = 0;
static uint8_t  gUart0DataCache[UART0_BUFF_LEN];

enum RLIN30_status RLIN30_get_status(void);
static void Uart0DataSave(uint8_t Data);
void RLIN30_port_init(void);

/******************************************************************************
** Function:    UartClockSelect
** Description: Used for UART Clock Config
** Parameter:   None
** Return:      None
******************************************************************************/
void UartClockSelect(void)
{
	uint32_t   reg32_value;

	/* Source Clock Setting for C_ISO_LIN
	CKSC_ILINS_CTL - C_ISO_LIN Source Clock Selection Register
	b31:b 2                    - Reserved set to 0
	b 1:b 0          ILINSCSID - Source Clock Setting for C_ISO_LIN  - CPUCLK2 */
	do
	{
	    reg32_value            = 0x00000001UL;
	    PROTCMD1               = 0x000000A5UL;   /* Protection release the CKSC_ILINS_CTL register   */
	    CKSC_ILINS_CTL         = reg32_value;
	    CKSC_ILINS_CTL         = ~reg32_value;
	    CKSC_ILINS_CTL         = reg32_value;
	} while ( PROTS1 != 0x00000000UL );
	while ( CKSC_ILINS_ACT != reg32_value )
	{
	    /* Wait for CKSC_ILINS_CTL to set. */
	}

	/* Clock Divider Setting for C_ISO_LIN
	CKSC_ILIND_CTL - C_ISO_LIN Divided Clock Selection Register
	b31:b 2                    - Reserved set to 0
	b 1:b 0          ILINDCSID - Source Clock Setting for C_ISO_LIN  - CKSC_ILINS_CTL selection / 1  */
	do
	{
	    reg32_value            = 0x00000001UL;
	    PROTCMD1               = 0x000000A5UL;   /* Protection release the CKSC_ILIND_CTL register   */
	    CKSC_ILIND_CTL         = reg32_value;
	    CKSC_ILIND_CTL         = ~reg32_value;
	    CKSC_ILIND_CTL         = reg32_value;
	} while ( PROTS1 != 0x00000000UL );
	while ( CKSC_ILIND_ACT != reg32_value );
}
/******************************************************************************
** Function:    RLIN30_init
** Description: Initialize the RLIN30 for UART 
** Parameter:   None
** Return:      None
******************************************************************************/
void RLIN30_init(void)
{
  /* RLIN30 is configured in UART mode with 115200 baud*/
  RLIN30_port_init();
  /* Disable RLIN */ 
  RLN30.LUOER=0x00;
  RLN30.LCUC=0x00;
  
	UartClockSelect();
  
  /* IN Mode Register/UART Mode Register (LMD) */
  RLN30.LMD=0x01;      //UART mode
                      //LIN / UART System Clock: fa
                      //Module generates 1 interrupt signal
                      //3-bit majority voting logic for sampling RX data is enabled.
  
											/* LIN Wake-up Baud Rate Selector register  */
  RLN30.LWBR= 0xF0;//0x51;     //6 samples per bit 
	
  /* LIN Baud Rate Prescaler1/UART Baud Rate Prescaler */
  //RLN30.LBRP1=0x02;    //Baudrate = PCLK / (((BRP value) + 1)*(number of samples per bit)).
  //RLN30.LBRP0=0xb5;    //40MHZ/((0x2b5+1)*6)= 9600 baud
  RLN30.LBRP01.UINT16 = 21U; 

  /* LIN / UART Error Detection Enable Register */
  RLN30.LEDE=0x00;     //No error detection

  /* LIN Break Field Configuration Register/UART Configuration Register1 */
  RLN30.LBFC=0x00;     //UART 8-bit communication
                      //LSB first
                      //Stop Bit 1 bit
                      //Parity Disabled
                      //Without RX inversion
                      //Without RX inversion 

  /* LIN / UART Control Register */
  RLN30.LCUC=0x01;     //Set SW Reset request to inactive


  /* UART Operation Enable Register */ 
  RLN30.LUOER=0x03;    //UART Transmission Operation Enable Bit
                      //UART Reception Operation Enable Bit
      
	/* Set the interrupt flags */
  MKRLIN30UR0                = 1U;
  RFRLIN30UR0                = 0U;
  TBRLIN30UR0                = 0U;

  MKRLIN30UR1                = 1U;
  RFRLIN30UR1                = 0U;
  TBRLIN30UR1                = 0U;

  MKRLIN30UR2                = 1U;
  RFRLIN30UR2                = 0U;
  TBRLIN30UR2                = 0U; 
	
	/* Set the interrupt flags */
  MKRLIN30UR1                = 0U;                  /* Enable INTRLIN30UR1 interrupt     */
  MKRLIN30UR2                = 0U;                  /* Enable INTRLIN30UR2 interrupt     */
}

/******************************************************************************
** Function:    RLIN30_port_init
** Description: Configures P0_2 to alternative function RLIN30TX
**              Configures P0_3 to alternative function RLIN30RX
** Parameter:   None
** Return:      None
******************************************************************************/
void RLIN30_port_init(void)
{
    
  /* Config RLIN30 Port Pins */ 
  /* RLIN30 TX on PIN 2 of Port 0 */
  PMC0  |= 1 << 2;
  PFCE0 &= ~(1 << 2);
  PFC0  |= (1 << 2);
  PFCAE0&= ~(1 << 2);
  PM0   &= ~(1 << 2);
    
  /* RLIN30 RX on PIN 3 of Port 0 */
  PMC0  |= 1 << 3;
  PFCE0 &= ~(1 << 3);
  PFC0  |=  1 << 3;
  PFCAE0&= ~(1 << 3);
  PM0   |= 1 << 3;
}


/******************************************************************************
** Function:    RLIN30_send_string
** Description: Sends out a complete string via UART by using DirectMemoryAccess
** Parameter:   string to be send
** Return:      None
******************************************************************************/
void RLIN30_send_string( char send_string[] ) {

    unsigned short  us_string_length= 0x00;

    /* Calculate length of string send_string [] */
    while( send_string[ us_string_length ] != '\0' )
        ++us_string_length;
    
    /* Wait until transmission is finished */
    while((RLN30.LST&16)==16);
    
    /* Initialize DMA for sending bytes */
    while(DTS0DTE != 0);                     //Wait until last transmission is finished 
    
                                                    
    DTS0 = 0x00;                             //Disable DMA
    DTC0 = us_string_length-1;               //Number of times the DMA transfers(string length -1 because first trigger is manually given)
    DSA0 = (unsigned int)&send_string[1];    //Source Address = string buffer
    DDA0 = (unsigned int)&RLN30.LUTDR;        //Destination Address = RLIN transmit register
    DTCT0 = 0x0020;                          //8bit TransferSize / No Loop / Fixed Destination / Increment Source
    DTFR0 = 0x800a;                          //Trigger Enabled / Source = RLIN30 transmit interrupt
    DRQCLR = 1;                              //Clear DMA channel 0 request 
    DTS0 |= 0x01;                            //Enable DMA channel 0 transfer
    
    /* Start transmission */
    RLN30.LUTDR.UINT16 = send_string[0];             //Write first char to RLIN transmit register to start transmission
  
}

/******************************************************************************
** Function:    RLIN30_get_status
** Description: Returns if RLIN30 is busy or ready
** Parameter:   None
** Return:      RLIN30_busy
**              RLIN30_ready
******************************************************************************/
enum RLIN30_status RLIN30_get_status()
{
    if((RLN30.LST&16)==16)                   //If transmission bit is set
    {
        return RLIN30_busy;					//Return RLIN30 is busy
    }
    else									//If transmission bit is 0 (transmission ready)
    {
        return RLIN30_ready;                //Return RLIN30 is ready
    }
}

void IntUart0Callback(uint32_t regEIIC_value)
{
	static uint16_t Res3;
	
	if(0x000010DEUL == regEIIC_value )
  {
		Res3   =  RLN30.LURDR.UINT16;        // Echo
		Uart0DataSave(Res3);
  }
}
/******************************************************************************
* Function Name : void ISR_Uart0Callback(void)
* Description   : put it into ISR
* Argument      : none
* Return Value  : none
******************************************************************************/
void ISR_Uart0Callback(void)
{
	static uint16_t Res0;

	Res0   =  RLN30.LURDR.UINT16;        // Echo
	Uart0DataSave(Res0);
}

/******************************************************************************
* Function Name : void Uart0SentOneByte(void)
* Description   : Sent One Byte
* Argument      : the data of Sent
* Return Value  : none
******************************************************************************/
void Uart0SentOneByte(uint8_t Byte)
{
	RLN30.LUTDR.UINT16 = Byte;
	RFRLIN30UR0          = 0U;
	while ( !RFRLIN30UR0 ) {
                /* Wait for RFRLIN30UR0 to set. */
            }
  RFRLIN30UR0          = 0U;
}
/******************************************************************************
* Function Name : Uart0Sent
* Description   : Sent a groupe of data
* Argument      : Data Point ,Data Len;
* Return Value  : none
******************************************************************************/
void Uart0Sent(uint8_t *Data,uint8_t Len)
{
	uint8_t Temp;
	for(Temp=0; Temp<Len; Temp++)
	{
		Uart0SentOneByte(Data[Temp]);
	}
}

/******************************************************************************
** Function:    Uart0DataSave
** Description: Save One Byte
** Parameter:   Input data
** Return:      None
******************************************************************************/
static void Uart0DataSave(uint8_t Data)
{	
	gUart0RecData[gUart0RecLen] = Data;
	gUart0RecLen++;
	
	if(gUart0RecLen > UART0_BUFF_LEN -1)
	{
		gUart0RecLen = 0;
	}
}
/******************************************************************************
* Function Name : void Uart0DataSaveCache(void)
* Description   : Save data to cache
* Argument      : none
* Return Value  : none
******************************************************************************/
void Uart0DataSaveCache(void)
{
  static uint8_t LastCount = 0;
	
	if(gUart0RecLen != 0)
	{
		if(LastCount == gUart0RecLen)
		{
			memcpy(gUart0DataCache,gUart0RecData,gUart0RecLen);
			gUart0DataCacheLen = gUart0RecLen; 
		  gUart0RecLen = 0;
		}
		LastCount = gUart0RecLen;
	}
}
/******************************************************************************
* Function Name : Uart0GetData
* Description   : Capture One Packet
* Argument      : Output Data
* Return Value  : Data Len
******************************************************************************/
uint16_t Uart0GetData(uint8_t* Data)
{
	uint32_t Retval = 0;
	
	if(gUart0DataCacheLen != 0)
	{
		memcpy(Data,gUart0DataCache,gUart0DataCacheLen);
		Retval = gUart0DataCacheLen;
		gUart0DataCacheLen = 0;
	}
	return Retval;
}