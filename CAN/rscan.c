/******************************************************************************
** Filename:    rscan.c
** Description: This files is the Driver of CAN Bus in communication.
** Author:  andy
** Date: 2018.9.10
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/
//============================================================================
//
// Source file for the RS CAN
//
//============================================================================
/*============================================================================
//  PIN Define:
	CAN1	CAN0_TX 		P0_0  2nd Alternative
	CAN1 	CAN0_RX 		P0_1  2nd Alternative
   
	CAN2 	CAN1_TX     P10_7 4th Alternative
	CAN2	CAN1_RX			P10_6 4th Alternative
	
	CAN3  CAN2_TX     P0_4 
	CAN3  CAN2_RX     P0_5  
	Baud Rate: 500Kbps
	
//============================================================================
//==========================================================================*/
// Includes
//============================================================================
#include  "r_typedefs.h"
#include  "iodefine.h"
#include 	"rscan.h"
#include 	"can_table.h"

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
volatile uint8_t gFlagNewData1 = 0;
volatile uint8_t gFlagNewData2 = 0;
volatile uint8_t gFlagNewData3 = 0;

Can_FrameType CANRecData1;
Can_FrameType CANRecData2;
Can_FrameType CANRecData3;

//============================================================================
// Functions
//============================================================================
/*****************************************************************************
** Function:    CanPinConfig
** Description: Configures the Pin of CAN0,CAN1,CAN2 macros
** Parameter:   None
** Return:      None
******************************************************************************/
void CanPinConfig(void)
{
//#if 1 // Adapt to DemoBoard (if use to Develop,there are some wrong of Sent)
#if DEMOCAN
	// CAN0 ------------------------------------------------------	 
  /* Set CAN0TX as P0_0 and CAN0RX as P0_1 */                                                   
  PFCE0&= ~(0x00003);
  PFC0 |= 0x00003;                                                      
  PM0 &= ~(1<<0);                                                       
  PM0 |= 1<<1;
  PMC0 |= 0x00003;
//#else  // Adapt to DevelopBoard
#elif DEVELOPCAN
	/* P10_0 (CAN0RX) */
  PFCAE10 &= 0xFFFEU;
  PFCE10  &= 0xFFFEU;
  PFC10   |= 0x0001U;
  PM10    |= 0x0001U;
  PMC10   |= 0x0001U;

  /* P10_1 (CAN0TX) */
  PFCAE10 &= 0xFFFDU;
  PFCE10  &= 0xFFFDU;
  PFC10   |= 0x0002U;
  P10     |= 0x0002U;
  PM10    &= 0xFFFDU;
  PMC10   |= 0x0002U;
#endif
	// CAN1 ------------------------------------------------------
	/* Set CAN1TX as P10_7 and CAN1RX as P10_6 */                       
	PFCE10 |= 0x00C0;
	PFC10  |= 0x00C0;                                                      
	PM10 &= ~(1<<7);                                                       
	PM10 |= 1<<6; 
	PMC10  |= 0x00C0; 
	// CAN2 ------------------------------------------------------
	/* Set CAN2TX as P0_4 and CAN2RX as P0_5 */ 
	PFCE0&= ~(0x00003<<4);
	PFC0 &= ~(0x00003<<4);                                                      
	PM0 &= ~(1<<4);                                                       
	PM0 |= 1<<5;
	PMC0 |= 0x00003<<4;
}
void CanPinConfigB(void)
{
	#if  DEMOCAN
	// CAN0 ------------------------------------------------------	 
  /* Set CAN0TX as P0_0 and CAN0RX as P0_1 */                                                   
  PFCE0&= ~(0x00003);
  PFC0 |= 0x00003;                                                      
  PM0 &= ~(1<<0);                                                       
  PM0 |= 1<<1;
  PMC0 |= 0x00003;
	#elif  DEVELOPCAN
	/* P10_0 (CAN0RX) */
  PFCAE10 &= 0xFFFEU;
  PFCE10  &= 0xFFFEU;
  PFC10   |= 0x0001U;
  PM10    |= 0x0001U;
  PMC10   |= 0x0001U;

  /* P10_1 (CAN0TX) */
  PFCAE10 &= 0xFFFDU;
  PFCE10  &= 0xFFFDU;
  PFC10   |= 0x0002U;
  P10     |= 0x0002U;
  PM10    &= 0xFFFDU;
  PMC10   |= 0x0002U;
#endif
	// CAN1 ------------------------------------------------------
	/* Set CAN1TX as P10_7 and CAN1RX as P10_6 */                       
	PFCE10 |= 0x00C0;
	PFC10  |= 0x00C0;                                                      
	PM10 &= ~(1<<7);                                                       
	PM10 |= 1<<6; 
	PMC10  |= 0x00C0; 
	// CAN2 ------------------------------------------------------
	/* Set CAN2TX as P0_4 and CAN2RX as P0_5 */ 
	PFCE0&= ~(0x00003<<4);
	PFC0 &= ~(0x00003<<4);                                                      
	PM0 &= ~(1<<4);                                                       
	PM0 |= 1<<5;
	PMC0 |= 0x00003<<4;
}
/*****************************************************************************
** Function:    RS_CAN_init
** Description: Configures the CAN0/CAN1 macros
** Parameter:   None
** Return:      None
******************************************************************************/
void RS_CAN_init(void)
{
    /* Main Osc -> CAN */
    protected_write(PROTCMD1,CKSC_ICANOSCD_CTL,0x01)
    while (PROTS1 ==0x01);
    
    /* Configure CAN0 GPIO-EnablePin P8_1 */
    PMC8 &= ~(1<<1);
    PM8 &= ~(1<<1);
    P8 &= ~(1<<1);    //First disabled
    
    /* Configure CAN1 GPIO-EnablePin P0_6 */ 
    PMC0 &= ~(1<<6);
    PM0 &= ~(1<<6);
    P0 &= ~(1<<6);    //First disabled
		
		/* Configure CAN0 GPIO-EnablePin P9_4 */
    PMC9 &= ~(1<<4);
    PM9 &= ~(1<<4);
    P9 &= ~(1<<4);    //First disabled
    
    CanPinConfig();
        
    /* Wait while CAN RAM initialization is ongoing */
    while((RSCAN0.GSTS.UINT32 & 0x00000008)) ;

    /* Switch to global/channel reset mode */
    RSCAN0.GCTR.UINT32 &= 0xfffffffb;	//set the 3rd bit to 0 -- global stop mdoe  
    RSCAN0.C0CTR.UINT32 &= 0xfffffffb;
    RSCAN0.C1CTR.UINT32 &= 0xfffffffb;
		RSCAN0.C2CTR.UINT32 &= 0xfffffffb;

    /* Configure clk_xincan as CAN-ClockSource */
    RSCAN0.GCFG.UINT32 = 0x00000010;
    
    /* When fCAN is 16MHz, 
    Bitrate = fCAN/(BRP+1)/(1+TSEG1+TSEG2) = 16/2/16 = 0.5Mbps(500Kbps) */

    RSCAN0.C0CFG.UINT32 = 0x023a0001; //SJW =3TQ, TSEG1=11TQ, TSEG2=4TQ, BRP=1

    RSCAN0.C1CFG.UINT32 = 0x023a0001; //SJW =3TQ, TSEG1=11TQ, TSEG2=4TQ, BRP=1

		RSCAN0.C2CFG.UINT32 = 0x023a0001; //SJW =3TQ, TSEG1=11TQ, TSEG2=4TQ, BRP=1
		
    /* ==== Rx rule setting ==== */
    Can_SetRxRule();

    /* ==== buffer setting ==== */    
    //RSCAN0RMNB = 0x18;  //Can_SetGlobalBuffer--24
    RSCAN0.RMNB.UINT32 = 0x40;  //
    
    /*Receive FIFO buffer setting*/
    //RSCAN0.RFCC0.UINT32=0x0000F200; //8 buffer depth, FIFO 0 interrupt is disabled, RF0 enabled only in operation mode
    RSCAN0.RFCC0.UINT32=0x0000F202;	  //8 buffer depth, FIFO 0 interrupt at each reception is enabled
    RSCAN0.RFCC1.UINT32=0x0000F202;	  //8 buffer depth, FIFO 1 interrupt at each reception is enabled
		RSCAN0.RFCC2.UINT32=0x0000F202;	  //8 buffer depth, FIFO 1 interrupt at each reception is enabled
    
    /*TX RX FIFO buffer 0 setting, assigned to CH0*/
    //RSCAN0.CFCC9.UINT32=0xFF05F200;  //8 buffer depth, Tx Rx FIFO 9 interrupt is disabled, TX mode, link to buffer 0
    RSCAN0.CFCC0.UINT32=0xFF05F205;  //8 buffer depth, Tx Rx FIFO 9 interrupt is enable, TX mode, link to buffer 0
    //RSCAN0.CFCC9.UINT32=0xFF05F205;  //8 buffer depth, Tx Rx FIFO 9 interrupt is enable, TX mode, link to buffer 0

    /*TX RX FIFO buffer 3 setting, assigned to CH1*/
    RSCAN0.CFCC3.UINT32=0xFF06F200; //8 buffer depth, Tx Rx FIFO 12 interrupt is disabled, Gateway mode, link to buffer 0
	
		/*TX RX FIFO buffer 6 setting, assigned to CH2*/
    RSCAN0.CFCC6.UINT32=0xFF06F200; //8 buffer depth, Tx Rx FIFO 12 interrupt is disabled, Gateway mode, link to buffer 0
		
    /* Set THLEIE disabled, MEIE(FIFO Message Lost Interrupt disabled)  */
    RSCAN0.GCTR.UINT32 &= 0xfffff8ff;    

    /* If GlobalChannel in halt or reset mode */
    if (RSCAN0.GSTS.UINT32 & 0x03) 
    {
        RSCAN0.GCTR.UINT32 &= 0xfffffffc; //Switch to communication mode
        while ((RSCAN0.GSTS.UINT32 & 0x02) == 2) {
            /* While halt mode */
        }
        while ((RSCAN0.GSTS.UINT32 & 0x01) == 1) {
            /* While reset mode */
        }
    }

    /* If Channel 0 in halt or reset mode */
    if (RSCAN0.C0STS.UINT32 & 0x03) 
    {
        RSCAN0.C0CTR.UINT32 &= 0xfffffffc;    //Switch to communication mode
        while ((RSCAN0.C0STS.UINT32 & 0x02) == 2) {
            /* While halt mode */
        }
        while ((RSCAN0.C0STS.UINT32 & 0x01) == 1) {
            /* While reset mode */
        }
    }
     
    /* If Channel 1 in halt or reset mode */
    if (RSCAN0.C1STS.UINT32 & 0x03) 
    {
        RSCAN0.C1CTR.UINT32 &= 0xfffffffc;    //Switch to communication mode
        while ((RSCAN0.C1STS.UINT32 & 0x02) == 2) {
            /* While halt mode */
        }
        while ((RSCAN0.C1STS.UINT32 & 0x01) == 1) {
            /* While reset mode */
        }
    }
    
        /* If Channel 1 in halt or reset mode */
    if (RSCAN0.C2STS.UINT32 & 0x03) 
    {
        RSCAN0.C2CTR.UINT32 &= 0xfffffffc;    //Switch to communication mode
        while ((RSCAN0.C2STS.UINT32 & 0x02) == 2) {
            /* While halt mode */
        }
        while ((RSCAN0.C2STS.UINT32 & 0x01) == 1) {
            /* While reset mode */
        }
    }
    
    RSCAN0.RFCC0.UINT32 |=0x01; //set RX FIFO on
    RSCAN0.RFCC1.UINT32 |=0x01; //set RX FIFO on
		RSCAN0.RFCC2.UINT32 |=0x01; //set RX FIFO on
    RSCAN0.CFCC0.UINT32 |=0x01; //set TX RX FIFO 0 on
    RSCAN0.CFCC3.UINT32 |=0x01; //set TX RX FIFO 3 on
		RSCAN0.CFCC6.UINT32 |=0x01; //set TX RX FIFO 6 on
	
    #if 0
		TBRCANGRECC = 1;          //Table interrupt is enable.
    MKRCANGRECC = 0;          //Enable the interrupt processing.
		#else
		TBRCANGRECC = 0;          //Table interrupt is enable.
    MKRCANGRECC = 0;          //Enable the interrupt processing.
		RFRCANGRECC = 0;          //Enable the interrupt processing.
		#endif
    
    TBRCAN0TRX = 1;          //Table interrupt is enable.
    MKRCAN0TRX = 0;          //Enable the interrupt processing.
    
    TBRCAN1TRX = 1;          //Table interrupt is enable.
    MKRCAN1TRX = 0;          //Enable the interrupt processing.
		
		TBRCAN2TRX = 1;          //Table interrupt is enable.
    MKRCAN2TRX = 0;          //Enable the interrupt processing.
    
}
void RS_CAN_initB(void)
{
    /* Main Osc -> CAN */
    protected_write(PROTCMD1,CKSC_ICANOSCD_CTL,0x01)
    while (PROTS1 ==0x01);
    
    /* Configure CAN0 GPIO-EnablePin P8_1 */
    PMC8 &= ~(1<<1);
    PM8 &= ~(1<<1);
    P8 &= ~(1<<1);    //First disabled
    
    /* Configure CAN1 GPIO-EnablePin P0_6 */ 
    PMC0 &= ~(1<<6);
    PM0 &= ~(1<<6);
    P0 &= ~(1<<6);    //First disabled
		
		/* Configure CAN0 GPIO-EnablePin P9_4 */
    PMC9 &= ~(1<<4);
    PM9 &= ~(1<<4);
    P9 &= ~(1<<4);    //First disabled
    
    CanPinConfigB();
        
    /* Wait while CAN RAM initialization is ongoing */
    while((RSCAN0.GSTS.UINT32 & 0x00000008)) ;

    /* Switch to global/channel reset mode */
    RSCAN0.GCTR.UINT32 &= 0xfffffffb;	//set the 3rd bit to 0 -- global stop mdoe  
    RSCAN0.C0CTR.UINT32 &= 0xfffffffb;
    RSCAN0.C1CTR.UINT32 &= 0xfffffffb;
		RSCAN0.C2CTR.UINT32 &= 0xfffffffb;

    /* Configure clk_xincan as CAN-ClockSource */
    RSCAN0.GCFG.UINT32 = 0x00000010;
    
    /* When fCAN is 16MHz, 
    Bitrate = fCAN/(BRP+1)/(1+TSEG1+TSEG2) = 16/2/16 = 0.5Mbps(500Kbps) */

    RSCAN0.C0CFG.UINT32 = 0x023a0001; //SJW =3TQ, TSEG1=11TQ, TSEG2=4TQ, BRP=1

    RSCAN0.C1CFG.UINT32 = 0x023a0001; //SJW =3TQ, TSEG1=11TQ, TSEG2=4TQ, BRP=1

		RSCAN0.C2CFG.UINT32 = 0x023a0001; //SJW =3TQ, TSEG1=11TQ, TSEG2=4TQ, BRP=1
		
    /* ==== Rx rule setting ==== */
    Can_SetRxRule();

    /* ==== buffer setting ==== */    
    //RSCAN0RMNB = 0x18;  //Can_SetGlobalBuffer--24
    RSCAN0.RMNB.UINT32 = 0x40;  //
    
    /*Receive FIFO buffer setting*/
    //RSCAN0.RFCC0.UINT32=0x0000F200; //8 buffer depth, FIFO 0 interrupt is disabled, RF0 enabled only in operation mode
    RSCAN0.RFCC0.UINT32=0x0000F202;	  //8 buffer depth, FIFO 0 interrupt at each reception is enabled
    RSCAN0.RFCC1.UINT32=0x0000F202;	  //8 buffer depth, FIFO 1 interrupt at each reception is enabled
		RSCAN0.RFCC2.UINT32=0x0000F202;	  //8 buffer depth, FIFO 1 interrupt at each reception is enabled
    
    /*TX RX FIFO buffer 0 setting, assigned to CH0*/
    //RSCAN0.CFCC9.UINT32=0xFF05F200;  //8 buffer depth, Tx Rx FIFO 9 interrupt is disabled, TX mode, link to buffer 0
    RSCAN0.CFCC0.UINT32=0xFF05F205;  //8 buffer depth, Tx Rx FIFO 9 interrupt is enable, TX mode, link to buffer 0
    //RSCAN0.CFCC9.UINT32=0xFF05F205;  //8 buffer depth, Tx Rx FIFO 9 interrupt is enable, TX mode, link to buffer 0

    /*TX RX FIFO buffer 3 setting, assigned to CH1*/
    RSCAN0.CFCC3.UINT32=0xFF06F200; //8 buffer depth, Tx Rx FIFO 12 interrupt is disabled, Gateway mode, link to buffer 0
	
		/*TX RX FIFO buffer 6 setting, assigned to CH2*/
    RSCAN0.CFCC6.UINT32=0xFF06F200; //8 buffer depth, Tx Rx FIFO 12 interrupt is disabled, Gateway mode, link to buffer 0
		
    /* Set THLEIE disabled, MEIE(FIFO Message Lost Interrupt disabled)  */
    RSCAN0.GCTR.UINT32 &= 0xfffff8ff;    

    /* If GlobalChannel in halt or reset mode */
    if (RSCAN0.GSTS.UINT32 & 0x03) 
    {
        RSCAN0.GCTR.UINT32 &= 0xfffffffc; //Switch to communication mode
        while ((RSCAN0.GSTS.UINT32 & 0x02) == 2) {
            /* While halt mode */
        }
        while ((RSCAN0.GSTS.UINT32 & 0x01) == 1) {
            /* While reset mode */
        }
    }

    /* If Channel 0 in halt or reset mode */
    if (RSCAN0.C0STS.UINT32 & 0x03) 
    {
        RSCAN0.C0CTR.UINT32 &= 0xfffffffc;    //Switch to communication mode
        while ((RSCAN0.C0STS.UINT32 & 0x02) == 2) {
            /* While halt mode */
        }
        while ((RSCAN0.C0STS.UINT32 & 0x01) == 1) {
            /* While reset mode */
        }
    }
     
    /* If Channel 1 in halt or reset mode */
    if (RSCAN0.C1STS.UINT32 & 0x03) 
    {
        RSCAN0.C1CTR.UINT32 &= 0xfffffffc;    //Switch to communication mode
        while ((RSCAN0.C1STS.UINT32 & 0x02) == 2) {
            /* While halt mode */
        }
        while ((RSCAN0.C1STS.UINT32 & 0x01) == 1) {
            /* While reset mode */
        }
    }
    
        /* If Channel 1 in halt or reset mode */
    if (RSCAN0.C2STS.UINT32 & 0x03) 
    {
        RSCAN0.C2CTR.UINT32 &= 0xfffffffc;    //Switch to communication mode
        while ((RSCAN0.C2STS.UINT32 & 0x02) == 2) {
            /* While halt mode */
        }
        while ((RSCAN0.C2STS.UINT32 & 0x01) == 1) {
            /* While reset mode */
        }
    }
    
    RSCAN0.RFCC0.UINT32 |=0x01; //set RX FIFO on
    RSCAN0.RFCC1.UINT32 |=0x01; //set RX FIFO on
		RSCAN0.RFCC2.UINT32 |=0x01; //set RX FIFO on
    RSCAN0.CFCC0.UINT32 |=0x01; //set TX RX FIFO 0 on
    RSCAN0.CFCC3.UINT32 |=0x01; //set TX RX FIFO 3 on
		RSCAN0.CFCC6.UINT32 |=0x01; //set TX RX FIFO 6 on
	
    #if 0
		TBRCANGRECC = 1;          //Table interrupt is enable.
    MKRCANGRECC = 0;          //Enable the interrupt processing.
		#else
		TBRCANGRECC = 0;          //Table interrupt is enable.
    MKRCANGRECC = 0;          //Enable the interrupt processing.
		RFRCANGRECC = 0;          //Enable the interrupt processing.
		#endif
    
    TBRCAN0TRX = 1;          //Table interrupt is enable.
    MKRCAN0TRX = 0;          //Enable the interrupt processing.
    
    TBRCAN1TRX = 1;          //Table interrupt is enable.
    MKRCAN1TRX = 0;          //Enable the interrupt processing.
		
		TBRCAN2TRX = 1;          //Table interrupt is enable.
    MKRCAN2TRX = 0;          //Enable the interrupt processing.
    
}
/******************************************************************************
** Function:    Can_SetRxRule
** Description: Set all Rx rules
** Parameter:   None
** Return:      None
******************************************************************************/
static void Can_SetRxRule(void)
{
    U16 RxRuleIdx;
    U8 PageRxRuleIdx;
    volatile CAN_RX_RULE_TYPE* pCRE;

		
    /* Set Rx rule number per channel */
    RSCAN0.GAFLCFG0.UINT32 |= 0x10080800;   //Channel 0 rule number is 16 Channel 1 rule number is 8

    /* Get access base address of Rx rule */
    pCRE = (volatile CAN_RX_RULE_TYPE*)&(RSCAN0.GAFLID0.UINT32);

    /* Receive Rule Table Write Enable */
    RSCAN0.GAFLECTR.UINT32 |= 0x00000100;	//set bit8 to 1, Receive rule table write is enabled

    /* Copy Rx rule one by one */
    for (RxRuleIdx = 0U; RxRuleIdx < CAN_RX_RULE_NUM; RxRuleIdx++) //CAN_RX_RULE_NUM=12, refer to cab_table.h
    //for (RxRuleIdx = 0U; RxRuleIdx < 16; RxRuleIdx++)	//if it is more than 16 rules, go to another page
    {
        PageRxRuleIdx = (U8) (RxRuleIdx & CAN_PAGE_RX_RULE_IDX_MASK); //CAN_PAGE_RX_RULE_IDX_MASK= 0xF

        /* Update target Rx rule page if necessary. */
        if (PageRxRuleIdx == 0U) //RxRuleIdx=0, page=0; RxRuleIdx=16, page=1; RxRuleIdx=32,page=2;...
        {
            RSCAN0.GAFLECTR.UINT32 |= RxRuleIdx >> CAN_RX_RULE_PAGE_IDX_BIT_POS; //CAN_RX_RULE_PAGE_IDX_BIT_POS= 4U
        }

        /* Set a single Rx rule.*/
        pCRE[PageRxRuleIdx] = CAN_RX_RULE_TABLE[RxRuleIdx];
    }

    /* Rx rule write disable */
    RSCAN0.GAFLECTR.UINT32 &= 0xfffffeff;
}

/******************************************************************************
** Function:    Can_C0TrmByTxBuf
** Description: Set a frame of data in channel0
** Parameter:   TxBufIdx the number of send buffer
**              pFrame the point of CAN data struct
** Return:      The result of sent
******************************************************************************/
Can_RtnType Can_C0TrmByTxBuf(U8 TxBufIdx, const Can_FrameType* pFrame)
{
    VU8* pTBSR;
    Can_FrameType* pTxBuffer;
    VU8* pTBCR;

    pTBSR = &(RSCAN0.TMSTS0);
    pTBCR = &(RSCAN0.TMC0);

    /* ---- Return if Tx Buffer is transmitting. ---- */    
    if( (pTBSR[TxBufIdx] & (VU8)0x01) == CAN_TBTST_TRANSMITTING )
    {
        return CAN_RTN_ERR;
    }

    /* Clear Tx buffer status */
    if (pTBSR[TxBufIdx] != CAN_CLR)
		{
			pTBSR[TxBufIdx] = CAN_CLR;
		} 

    /* Store message to tx buffer */
    pTxBuffer = (Can_FrameType*) &(RSCAN0.TMID0);
    pTxBuffer[TxBufIdx] = *pFrame;

    /* Set transmission request */
    pTBCR[TxBufIdx] = CAN_TBCR_TRM;

    return CAN_RTN_OK;
}

/******************************************************************************
** Function:    Can_C1TrmByTxBuf
** Description: Set a frame of data in channel1
** Parameter:   TxBufIdx the number of send buffer
**              pFrame the point of CAN data struct
** Return:      The result of sent
******************************************************************************/
Can_RtnType Can_C1TrmByTxBuf(U8 TxBufIdx, const Can_FrameType* pFrame)
{
   VU8* pTBSR;
   Can_FrameType* pTxBuffer;
   VU8* pTBCR;

   pTBSR = &(RSCAN0.TMSTS16);
   pTBCR = &(RSCAN0.TMC16);

   /* ---- Return if Tx Buffer is transmitting. ---- */
   if( (pTBSR[TxBufIdx] & (VU8)0x01) == CAN_TBTST_TRANSMITTING )
   {
       return CAN_RTN_ERR;
   }

   /* Clear Tx buffer status */
   if (pTBSR[TxBufIdx] != CAN_CLR)
   {
			pTBSR[TxBufIdx] = CAN_CLR;
   } 

   /* Store message to Tx buffer */
   pTxBuffer = (Can_FrameType*) &(RSCAN0.TMID16);
   pTxBuffer[TxBufIdx] = *pFrame;

   /* Set transmission request */
   pTBCR[TxBufIdx] = CAN_TBCR_TRM;

   return CAN_RTN_OK;
}

/******************************************************************************
** Function:    Can_C2TrmByTxBuf
** Description: Set a frame of data in channel2
** Parameter:   TxBufIdx the number of send buffer
**              pFrame the point of CAN data struct
** Return:      The result of sent
******************************************************************************/
Can_RtnType Can_C2TrmByTxBuf(U8 TxBufIdx, const Can_FrameType* pFrame)
{
   VU8* pTBSR;
   Can_FrameType* pTxBuffer;
   VU8* pTBCR;

   pTBSR = &(RSCAN0.TMSTS32);
   pTBCR = &(RSCAN0.TMC32);

   /* ---- Return if Tx Buffer is transmitting. ---- */
   if( (pTBSR[TxBufIdx] & (VU8)0x01) == CAN_TBTST_TRANSMITTING )
   {
       return CAN_RTN_ERR;
   }

   /* Clear Tx buffer status */
   if (pTBSR[TxBufIdx] != CAN_CLR)
   {
       pTBSR[TxBufIdx] = CAN_CLR;
   } 

   /* Store message to Tx buffer */
   pTxBuffer = (Can_FrameType*) &(RSCAN0.TMID32);
   pTxBuffer[TxBufIdx] = *pFrame;

   /* Set transmission request */
   pTBCR[TxBufIdx] = CAN_TBCR_TRM;

   return CAN_RTN_OK;
}

/******************************************************************************
** Function:    Can_ChTrmByTxBuf
** Description: Set a frame of data in channel n
** Parameter:   Chn the channel of use
**              TxBufIdx the number of send buffer
**              pFrame the point of CAN data struct
** Return:      The result of sent
******************************************************************************/
Can_RtnType Can_ChTrmByTxBuf(U8 Chn,U8 TxBufIdx, const Can_FrameType* pFrame)
{
   VU8* pTBSR;
   Can_FrameType* pTxBuffer;
   VU8* pTBCR;
   if(Chn == 1)
	 {
	 		pTBSR = &(RSCAN0.TMSTS0);
   		pTBCR = &(RSCAN0.TMC0);
	 }
	 else  if(Chn == 2)
	 {
	 		pTBSR = &(RSCAN0.TMSTS16);
   		pTBCR = &(RSCAN0.TMC16);
	 }
	 else  if(Chn == 3)
	 {
	 		pTBSR = &(RSCAN0.TMSTS32);
   		pTBCR = &(RSCAN0.TMC32);
	 }
   /* ---- Return if Tx Buffer is transmitting. ---- */
   if( (pTBSR[TxBufIdx] & (VU8)0x01) == CAN_TBTST_TRANSMITTING )
   {
       return CAN_RTN_ERR;
   }

   /* Clear Tx buffer status */
   if (pTBSR[TxBufIdx] != CAN_CLR)
   {
       pTBSR[TxBufIdx] = CAN_CLR;
   } 

   /* Store message to Tx buffer */
	 if(Chn == 1)
	 {
			pTxBuffer = (Can_FrameType*) &(RSCAN0.TMID0);
	 }
	 else  if(Chn == 2)
	 {
	 		pTxBuffer = (Can_FrameType*) &(RSCAN0.TMID16);
	 }
	 else  if(Chn == 3)
	 {
	 		pTxBuffer = (Can_FrameType*) &(RSCAN0.TMID32);
	 }

   pTxBuffer[TxBufIdx] = *pFrame;

   /* Set transmission request */
   pTBCR[TxBufIdx] = CAN_TBCR_TRM;

   return CAN_RTN_OK;
}

/******************************************************************************
* Function Name : INTRCANGRECC_interruptISR
* Description   : This function is INTRCANGRECC interrupt processing. (Table Reference)
* Argument      : none
* Return Value  : none
******************************************************************************/
void INTRCANGRECC_interruptISR( void )
{
	Can_FrameType* pRxBuffer;
	RSCAN0.RFSTS0.UINT8[LL] &= 0x07;	//clear RFIF Receive FIFO Interrupt Request Flag
	while(RSCAN0.RFSTS0.UINT8[LH]  != 0)  // The receive FIFO buffer contains unread message.
	{
		gFlagNewData1 = 1;	
		/* Read out message from Rx buffer */
		pRxBuffer = (Can_FrameType*) &(RSCAN0.RFID0);
		CANRecData1 = pRxBuffer[0];	//RF0
		RSCAN0.RFPCTR0.UINT8[LL] = 0xff;
	}
	
	RSCAN0.RFSTS1.UINT8[LL] &= 0x07;	//clear RFIF Receive FIFO Interrupt Request Flag
	while(RSCAN0.RFSTS1.UINT8[LH]  != 0)  // The receive FIFO buffer contains unread message.
	{
		gFlagNewData2 = 2;	
		/* Read out message from Rx buffer */
		pRxBuffer = (Can_FrameType*) &(RSCAN0.RFID1);
		CANRecData2 = pRxBuffer[0];	//RF0
		RSCAN0.RFPCTR1.UINT8[LL] = 0xff;
	}
	
	RSCAN0.RFSTS2.UINT8[LL] &= 0x07;	//clear RFIF Receive FIFO Interrupt Request Flag
	while(RSCAN0.RFSTS2.UINT8[LH]  != 0)  // The receive FIFO buffer contains unread message.
	{
		gFlagNewData3 = 3;	
		/* Read out message from Rx buffer */
		pRxBuffer = (Can_FrameType*) &(RSCAN0.RFID2);
		CANRecData3 = pRxBuffer[0];	//RF0
		RSCAN0.RFPCTR2.UINT8[LL] = 0xff;
	}
}

void IntCANRecvCallback( uint32_t regEIIC_value )
{
  if(0x0000100FUL == regEIIC_value)/* CANGRCC */
		INTRCANGRECC_interruptISR();
}

void BspChmCanSent(uint32_t IDStd, uint8_t *Data,uint8_t DataLen)
{
  Can_FrameType TxFrame;
	static uint8_t TransBuffNum = 0;
	
	/*Fill Data*/
	TxFrame.ID    = IDStd;		// load ID info
	TxFrame.THLEN = 0;
	TxFrame.RTR   = 0;
	TxFrame.IDE   = 0;  // Standard Frame

	TxFrame.TS    = 0x0000;
	TxFrame.LBL   = 0x000;
	TxFrame.DLC   = DataLen;
	memcpy(&TxFrame.DB[0],Data,DataLen); // load Data info

	Can_ChTrmByTxBuf(CAN_NUM,TransBuffNum+1,&TxFrame);  // Sent Data
	TransBuffNum++;
	if(TransBuffNum >=15)
	{
		TransBuffNum = 0;
	}
}
