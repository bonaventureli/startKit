/******************************************************************************

* file Name : HAL_can.c

* version   : 0.3

* Argument  : lifei

* data  : 2018/8/24

* describe: 

*PIN Define:

	CAN1	CAN0_TX 		P0_0  2nd Alternative

	CAN1 	CAN0_RX 		P0_1  2nd Alternative

   

	CAN2 	CAN1_TX     		P10_7 4th Alternative

	CAN2	CAN1_RX			P10_6 4th Alternative

	Baud Rate: 500Kbps



	CAN0

	0010 2th output P0_0   CAN0TX

	0011 2th input  P0_1   CAN0RX

	0011 2th input  P10_0  CAN0RX

	0010 2th output P10_1  CAN0TX



	CAN1

	0110 4th output P10_7  CAN1TX

	0111 4th input  P10_6  CAN1RX



	CAN2

	0000 1st output P0_4   CAN2TX

	0001 1st input  P0_5   CAN2RX







	PFCAEn_m 	PFCEn_m 	PFCn_m 	PMn_m 	Function

	0 		0 		0 	1 	1st alternative function / Input

	0 		0 		0 	0 	1st alternative function / Output

	0 		0 		1 	1 	2nd alternative function / Input

	0 		0 		1 	0 	2nd alternative function / Output

	0 		1 		0 	1 	3rd alternative function / Input

	0 		1 		0 	0 	3rd alternative function / Output

	0 		1 		1 	1 	4th alternative function / Input

	0 		1 		1 	0 	4th alternative function / Output

	1 		0 		0 	1 	5th alternative function / Input

	1 		0 		0 	0 	5th alternative function / Output

	1 		0 		1 	X 	Setting prohibited

	1 		1 		X 	X 	Setting prohibited

	

******************************************************************************/

#include  "r_typedefs.h"
#include  "iodefine.h"
//#include  "rscan.h"
//#include  "can_table.h"
#include "HAL_can.h"
#include "MslATMode.h"
#include "MslGattMode.h"

T_CANDATA gsTxCANData;
T_CANDATA gsRxCANData;

uint8_t CmpStdData[8];   // to be compare the data of Receive

uint32_t Mask[] = {0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
volatile uint8_t gFlagNewData1 = 0;
volatile uint8_t gFlagNewData2 = 0;
volatile uint8_t gFlagNewData3 = 0;

Can_FrameType CANRecData1;
Can_FrameType CANRecData2;
Can_FrameType CANRecData3;

const can_cre_type RX_RULE_TABLE_LIST[RX_RULE_NUM] = {
/* CAN 0 reception rules */
    {0x00000010UL, 0xDFFFFFF0UL, 0x00008000UL, 0x00000001UL}, /* NO.000 : std data frm ID = H'10~1F will be accepted, destination Rx Buffer0 */
    {0x00000020UL, 0xDFFFFFF0UL, 0x00008100UL, 0x00000000UL}, /* NO.001 : std data frm ID = H'20~2F will be accepted, destination Rx Buffer1 */
    {0x00000030UL, 0xDFFFFFF0UL, 0x00008200UL, 0x00000000UL}, /* NO.002 : std data frm ID = H'30~3F will be accepted, destination Rx Buffer2 */
    {0x00000040UL, 0xDFFFFFF0UL, 0x00008300UL, 0x00000000UL}, /* NO.003 : std data frm ID = H'40~4F will be accepted, destination Rx Buffer3 */
    {0x00000050UL, 0xDFFFFFFFUL, 0x00008400UL, 0x00000000UL}, /* NO.004 : std data frm ID = H'50 will be accepted, destination Rx Buffer4 */
    {0x00000060UL, 0xDFFFFFFFUL, 0x00008500UL, 0x00000000UL}, /* NO.005 : std data frm ID = H'60 will be accepted, destination Rx Buffer5 */
    {0x00000200UL, 0xDFFFFFFFUL, 0x00000000UL, 0x00000001UL}, /* NO.006 : std data frm ID = H'200 will be accepted, destination RX FIFO buffer0 */
    {0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000001UL}, /* NO.007 : std data frm ID = H'80~8F will be accepted, destination RX FIFO buffer0 */
    
    {0x90000010UL, 0xDFFFFFF0UL, 0x00008800UL, 0x00000000UL}, /* NO.008 : Ext data frm ID = H'10000010-1F will be accepted, destination Rx Buffer8 */
    {0x90000020UL, 0xDFFFFFF0UL, 0x00008900UL, 0x00000000UL},
    {0x90000030UL, 0xDFFFFFF0UL, 0x00008a00UL, 0x00000000UL},
    {0x90000040UL, 0xDFFFFFF0UL, 0x00008b00UL, 0x00000000UL},
    
    {0x40000010UL, 0xDFFFFFF0UL, 0x00008c00UL, 0x00000000UL}, //std remote ID = H'010-1F
    {0x40000020UL, 0xDFFFFFF0UL, 0x00008d00UL, 0x00000000UL}, //std remote ID = H'020-2F 
    {0xc0000030UL, 0xDFFFFFFFUL, 0x00008e00UL, 0x00000000UL}, //ext remote ID = H'00000030
    {0xc0000040UL, 0xDFFFFFFFUL, 0x00008f00UL, 0x00000000UL}, 
	
/* CAN 1 reception rules */
    {0x00000010UL, 0xDFFFFFF0UL, 0x00009000UL, 0x00000000UL}, /* NO.000 : std data frm ID = H'10~1F will be accepted, destination Rx Buffer H10 */
    {0x00000020UL, 0xDFFFFFF0UL, 0x00009100UL, 0x00000000UL}, /* NO.001 : std data frm ID = H'20~2F will be accepted, destination Rx Buffer H11 */
    {0x00000030UL, 0xDFFFFFF0UL, 0x00009200UL, 0x00000000UL}, /* NO.002 : std data frm ID = H'30~3F will be accepted, destination Rx Buffer H12 */
    {0x00000040UL, 0xDFFFFFF0UL, 0x00009300UL, 0x00000000UL}, /* NO.003 : std data frm ID = H'40~4F will be accepted, destination Rx Buffer H13 */
    {0x00000050UL, 0xDFFFFFFFUL, 0x00009400UL, 0x00000000UL}, /* NO.004 : std data frm ID = H'50 will be accepted, destination Rx Buffer H14 */
    {0x00000060UL, 0xDFFFFFFFUL, 0x00009500UL, 0x00000000UL}, /* NO.005 : std data frm ID = H'60 will be accepted, destination Rx Buffer H15 */
    {0x00000200UL, 0xDFFFFFFFUL, 0x00000000UL, 0x00000002UL}, /* NO.006 : std data frm ID = H'200 will be accepted, destination RX FIFO buffer1 */
    {0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000002UL}, /* NO.007 : std data frm ID = H'80~8F will be accepted, destination RX FIFO buffer1 */
		//{0x00000080UL, 0xDFFFFFFFUL, 0x00009700UL, 0x00000000UL}, /* NO.007 : std data frm ID = H'80 will be accepted, destination Rx Buffer H17 */
/* CAN 2 reception rules */
    {0x00000010UL, 0xDFFFFFF0UL, 0x00009000UL, 0x00000000UL}, /* NO.000 : std data frm ID = H'10~1F will be accepted, destination Rx Buffer H10 */
    {0x00000020UL, 0xDFFFFFF0UL, 0x00009100UL, 0x00000000UL}, /* NO.001 : std data frm ID = H'20~2F will be accepted, destination Rx Buffer H11 */
    {0x00000030UL, 0xDFFFFFF0UL, 0x00009200UL, 0x00000000UL}, /* NO.002 : std data frm ID = H'30~3F will be accepted, destination Rx Buffer H12 */
    {0x00000040UL, 0xDFFFFFF0UL, 0x00009300UL, 0x00000000UL}, /* NO.003 : std data frm ID = H'40~4F will be accepted, destination Rx Buffer H13 */
    {0x00000050UL, 0xDFFFFFFFUL, 0x00009400UL, 0x00000000UL}, /* NO.004 : std data frm ID = H'50 will be accepted, destination Rx Buffer H14 */
    {0x0000006UL, 0xDFFFFFFFUL, 0x00009500UL, 0x00000000UL}, /* NO.005 : std data frm ID = H'60 will be accepted, destination Rx Buffer H15 */
    {0x00000200UL, 0xDFFFFFFFUL, 0x00000000UL, 0x00000004UL}, /* NO.006 : std data frm ID = H'200 will be accepted, destination RX FIFO buffer2 */
    {0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000004UL}, /* NO.007 : std data frm ID = H'80~8F will be accepted, destination RX FIFO buffer2 */
		//{0x00000080UL, 0xDFFFFFFFUL, 0x00009700UL, 0x00000000UL}, /* NO.007 : std data frm ID = H'80 will be accepted, destination Rx Buffer H17 */

};

/*******************************************************/
/*list of CAN ID*/
#define ID_KESSY04_STATUS 				0x592 //Is pressed or not the fire buttom
#define ID_RESPONDSE_GW						0x200	//whether receive the respond of GW
#define ID_BCM_01			            0x65A	//engine cover
#define ID_ZV_02									0x583	//door status leftback rightback lock status
#define ID_TSG_FT_01              0x3D0 //main driver lock status & window status
#define ID_TSG_BT_01							0x3D1 //subdriver lock status & window status
#define ID_TSG_HFS_01							0x3CE	//leftback lock status & window status
#define ID_TSG_HBFS_01						0x3CF	//rightback lock status & window status
#define ID_MD1_Position						0x5E0 //skylight status
#define ID_KLIMA_SENSOR_02				0x5E1 //sensor status
#define	ID_KLIMA_03								0x66E	//temperature of inside car
/*******************************************************/
/*list of Term Num*/
#define STATUS_FIRE_BUTTOM 				0
#define STATUS_ENGINE_COVER  			1
#define STATUS_DOOR								2
#define STATUS_TRUNK							3
#define STATUS_PILOT_DOORLOCK					4
#define STATUS_PSSENGER_DOORLOCK			5
#define STATUS_LEFTBACK_DOORLOCK			6
#define STATUS_RIGHTBACK_DOORLOCK			7
#define STATUS_PILOT_WINDOW				8
#define STATUS_PSSENGER_WINDOW		9
#define STATUS_LEFTBACK_WINDOW		10
#define STATUS_RIGHTBACK_WINDOW		11
#define STATUS_SKYLIGHT_WINDOW		12
#define SPEED_VEHICLE  						13
#define SPEED_UNIT								14	
#define INSIDE_TEMPER							15
#define OUTSIDE_TEMPER						16
#define STATUS_FAN 								17

#define STATUS_OIL_VOLUME  			
#define STATUS_TEMPERATURN

/*******************************************************/
/*Mask data of CAN*/
const T_MASK_MATRIX MaskMatrix[] = {
/*NUM   StartBit  VauleLen  */
/*0*/  {16,		    1,	    }, //Fire Buttom status
/*1*/  {31,		    1,	    }, //engine cover 
/*2*/  {24,		    4,	    }, //door status 
/*3*/  {28,		    1,	    }, //Trunk status
/*4*/  {1,		    1,	    }, //main driver site door status
/*5*/  {1,		    1,	    }, //subdriver site door status
/*6*/  {50,		    1,	    }, //leftback door status
/*7*/  {52,		    1,	    }, //rightback door status
/*8*/  {24,		    8,	    }, //main driver site window status
/*9*/  {24,		    8,	    }, //subdriver site window status
/*10*/ {24,		    8,	    }, //leftback window status
/*11*/ {24,		    8,	    }, //rightback window status
/*12*/ {8,		    7,	    }, //skylight window status
/*13*/ {24,		    9,	    }, //vehicle speed
/*14*/ {17,		    1,	    }, //speed unit
/*15*/ {40,		    8,	    }, //car inside temperature
/*16*/ {8,		    8,	    }, //car outside temperature
/*17*/ {26,		    4,	    }, //fan status
/*...*/                                     
}; 

/*****************************************************************************
** Function:    CanPinConfig
** Description: Configures the Pin of CAN0,CAN1,CAN2 macros
** Parameter:   None
** Return:      None
******************************************************************************/
void HAL_CanPinConfig(void)
{
#if DEMOCAN
	// CAN0 ------------------------------------------------------	 
  /* Set CAN0TX as P0_0 and CAN0RX as P0_1 */                                                   
  PFCE0&= ~(0x00003);
  PFC0 |= 0x00003;                                                      
  PM0 &= ~(1<<0);                                                       
  PM0 |= 1<<1;
  PMC0 |= 0x00003;
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
void HAL_CanPinConfigB(void)
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

/******************************************************************************
* Function Name : void CanPinConfigC( void )
* Description   : This function config the CAN Pin.
* Argument      : none
* Return Value  : none
******************************************************************************/

void HAL_CanPinConfigC(void)
{
#if DEMOCAN
	/*CAN0 Set CAN0TX as P0_0 and CAN0RX as P0_1 /
	0010 2th output P0_0   CAN0TX
	0011 2th input  P0_1   CAN0RX
	*/
	/*PFCEn — Port Function Control Expansion Register*/
	PFCE0&= ~(0x03<<0);
	/*PFCn / JPFC0 — Port Function Control Register*/
	PFC0 |= (0x03<<0);
	/*PMn / APMn / JPM0 — Port Mode Register
	 * 0: Output mode (output enabled)
	 * 1: Input mode (output disabled)*/
	PM0 &= ~(1<<0);
	PM0 |= 1<<1;
	/*	PMCn / JPMC0 — Port Mode Control Register
	 *	1: Alternative mode
	 *	0: Port mode*/
	PMC0 |= (0x03<<0);
#elif  DEVELOPCAN
	/*CAN0 Set CAN0TX as P10_1 and CAN0RX as P10_0 /
	0011 2th input  P10_0  CAN0RX
	0010 2th output P10_1  CAN0TX
	*/
	/*PFCEn — Port Function Control Expansion Register*/
	PFCE10&= ~(0x03<<0);
	/*PFCn / JPFC0 — Port Function Control Register*/
	PFC10 |= (0x03<<0);
	/*PMn / APMn / JPM0 — Port Mode Register
	 * 0: Output mode (output enabled)
	 * 1: Input mode (output disabled)*/
	PM10 |= (1<<0);
	PM10 &= ~(1<<1);
	/*	PMCn / JPMC0 — Port Mode Control Register

	 *	1: Alternative mode

	 *	0: Port mode*/

	PMC10 |= (0x03<<0);
#endif

	/* CAN1 Set CAN1TX as P10_7 and CAN1RX as P10_6

	 *0110 4th output P10_7  CAN1TX

	 *0111 4th input  P10_6  CAN1RX */

	/*PFCEn — Port Function Control Expansion Register*/

	PFCE10 |= (0x03<<6);

	/*PFCn / JPFC0 — Port Function Control Register*/

	PFC10  |= (0x03<<6);

	/*PMn / APMn / JPM0 — Port Mode Register

	 * 0: Output mode (output enabled)

	 * 1: Input mode (output disabled)*/

	PM10 &= ~(1<<7);

	PM10 |= 1<<6;

	/*	PMCn / JPMC0 — Port Mode Control Register

	 *	1: Alternative mode

	 *	0: Port mode*/

	PMC10  |= (0x03<<6);

	/* CAN2 Set CAN2TX as P0_4 and CAN2RX as P0_5

	 * 0000 1st output P0_4   CAN2TX

         * 0001 1st input  P0_5   CAN2RX */



	/*PFCEn — Port Function Control Expansion Register*/

	PFCE0&= ~(0x00003<<4);

	/*PFCn / JPFC0 — Port Function Control Register*/

	PFC0 &= ~(0x00003<<4);

	/*PMn / APMn / JPM0 — Port Mode Register

	 * 0: Output mode (output enabled)

	 * 1: Input mode (output disabled)*/

	PM0 &= ~(1<<4);

	PM0 |= 1<<5;

	/*	PMCn / JPMC0 — Port Mode Control Register

	 *	1: Alternative mode

	 *	0: Port mode*/

	PMC0 |= 0x00003<<4;

}
/*****************************************************************************
** Function:    RS_CAN_init
** Description: Configures the CAN0/CAN1 macros
** Parameter:   None
** Return:      None
******************************************************************************/
void HAL_RS_CAN_init(void)
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
    
    HAL_CanPinConfigC();
        
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
    
		HAL_Can_SetRxRule();

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
void HAL_RS_CAN_initB(void)
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
    
    HAL_CanPinConfigB();
        
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
    HAL_Can_SetRxRule();

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
** Function:    HAL_Can_SetRxRule
** Description: Set all Rx rules
** Parameter:   None
** Return:      None
******************************************************************************/
static void HAL_Can_SetRxRule(void)
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
Can_RtnType HAL_Can_C0TxframeByTxBuf(U8 TxBufIdx, const Can_FrameType* pFrame)
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
Can_RtnType HAL_Can_C1TxframeByTxBuf(U8 TxBufIdx, const Can_FrameType* pFrame)
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
Can_RtnType HAL_Can_C2TxframeByTxBuf(U8 TxBufIdx, const Can_FrameType* pFrame)
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
** Function:    HAL_Can_CnTxframeByTxBuf
** Description: Set a frame of data in channel n
** Parameter:   Chn the channel of use
**              TxBufIdx the number of send buffer
**              pFrame the point of CAN data struct
** Return:      The result of sent
******************************************************************************/
Can_RtnType HAL_Can_CnTxframeByTxBuf(U8 Chn,U8 TxBufIdx, const Can_FrameType* pFrame)
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
void HAL_INTRCANGRECC_interruptISR( void )
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

void HAL_IntCANRecvCallback( uint32_t regEIIC_value )
{
  if(0x0000100FUL == regEIIC_value)/* CANGRCC */
		HAL_INTRCANGRECC_interruptISR();
}

void HAL_BspChmCanSent(uint32_t IDStd, uint8_t *Data,uint8_t DataLen)
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

	HAL_Can_CnTxframeByTxBuf(CAN_NUM,TransBuffNum+1,&TxFrame);  // Sent Data
	TransBuffNum++;
	if(TransBuffNum >=15)
	{
		TransBuffNum = 0;
	}
}


/******************************************************************************
* Function Name : CANDataLoad
* Description   : Load Data
* Argument      : Data will to be Load; StartByte
* Return Value  : none
******************************************************************************/
void CANDataLoad(uint8_t* Data,uint8_t StartByte,uint8_t StartBit,uint8_t BitLenth,uint32_t SignValue)
{
	uint8_t TempData;
  
	TempData = Data[StartByte];
	
	TempData &= ~(Mask[BitLenth]<<StartBit);
	TempData |= SignValue<<StartBit;
	
	Data[StartByte] = TempData;
}
/******************************************************************************
* Function Name : CANDataFill
* Description   : Fill Data to Buffer
* Argument      : CANData Buffer Space; Function list of function
* Return Value  : none
******************************************************************************/
void CANDataFill(T_CANDATA *CANData,uint8_t Function)
{
  CANDataLoad(CANData->Data,											// Input Data
							CANCmdMatrix[Function].StartByte,		// StartByte
							CANCmdMatrix[Function].StartBit,		// StartBit
							CANCmdMatrix[Function].BitLenth,		// BitLenth
							CANCmdMatrix[Function].SignValue);	// SignValue
	if(CANCmdMatrix[Function].StdID != 0)
	{
		CANData->StdID = CANCmdMatrix[Function].StdID;  // Frame ID
	}
}
/******************************************************************************
* Function Name : MslCANDataSent
* Description   : CAN Data sent
* Argument      : CANData sent data;
* Return Value  : none
******************************************************************************/
void MslCANDataSent(T_CANDATA *CANData)
{
  Can_FrameType TxFrame;
	uint8_t Num;
	
	if(CAN_TX_RESPOND_EN == 1)
	{
		/*Set Sent Busy*/
		CANData->SentBusy = 1;
		CANData->SendTime = GetTimeMs();
	}
	
	TxFrame.ID = CANData->StdID;		// load ID info
	TxFrame.THLEN = 0;
	TxFrame.RTR = 0;
	TxFrame.IDE = 0;

	TxFrame.TS = 0x0000;
	TxFrame.LBL = 0x000;
	TxFrame.DLC = 0x8;
	memcpy(&TxFrame.DB[0],&CANData->Data[0],8); // load Data info

	for(Num=0;Num < NUM_CANFRAME_ONCE;Num++)
	{
		HAL_Can_CnTxframeByTxBuf(CAN_NUM,Num+1,&TxFrame);
	}
}

/******************************************************************************
* Function Name : MslCANReceive
* Description   : CAN Bus Receive
* Argument      : CANData used to save data;
* Return Value  : which channel was Received
******************************************************************************/
uint8_t MslCANReceive(T_CANDATA *CANData)
{
	uint8_t Retval = 0;
	
	if(CAN_NUM == gFlagNewData1)
	{
		gsRxCANData.StdID = CANRecData1.ID;
		memcpy(&gsRxCANData.Data[0],&CANRecData1.DB[0],8);
		
		gFlagNewData1 = 0;
		Retval = 1;
	}
	else if(CAN_NUM == gFlagNewData2)
	{
		gsRxCANData.StdID = CANRecData2.ID;
		memcpy(&gsRxCANData.Data[0],&CANRecData2.DB[0],8);
	
		gFlagNewData2 = 0;
		Retval = 2;
	}
	else if(CAN_NUM == gFlagNewData3)
	{
		gsRxCANData.StdID = CANRecData3.ID;
		memcpy(&gsRxCANData.Data[0],&CANRecData3.DB[0],8);
	
		gFlagNewData3 = 0;
		Retval = 3;
	}	
	return Retval;
}

/******************************************************************************
* Function Name : CANDataReset
* Description   : Reset the CAN Buffer
* Argument      : CANData used to save data;
* Return Value  : void
******************************************************************************/
void CANDataReset(T_CANDATA *CANData)
{
	memset(CANData->Data,0,8);
	CANData->StdID = 0;
}

/******************************************************************************
* Function Name : MslCANCmdExecute
* Description   : CAN Bus sent
* Argument      : CANData sent data;
* Return Value  : none
******************************************************************************/
void MslCANCmdExecute(uint8_t Function)
{
	/*Reset Data*/
	CANDataReset(&gsTxCANData);
	/*Load Function Opcode into Buff*/
	CANDataFill(&gsTxCANData,Function);
	CANDataFill(&gsTxCANData,FRAME_ASSEMBLY_1);
	CANDataFill(&gsTxCANData,FRAME_ASSEMBLY_2);
	/*...*/
	
	/*Sent CAN Data Twice*/
	MslCANDataSent(&gsTxCANData);
	if(CAN_TX_RESPOND_EN == 1)
	{
		gsTxCANData.ReSentCount = 0;
	}	
}

/******************************************************************************
* Function Name : MslCANRespondTask
* Description   : decode 0x200
* Argument      : RxData the reseived data;
* Return Value  : none
******************************************************************************/
void MslCANRespondTask(T_CANDATA *RxData)
{
	if(CAN_TX_RESPOND_EN == 1)
	{
		MslCANCmdRespond(&gsTxCANData,RxData);
	}
}

/******************************************************************************
* Function Name : MslCANCmdRespond
* Description   : CAN Bus sent
* Argument      : TxData will sent data;RxData reseived data
* Return Value  : none
******************************************************************************/
static void MslCANCmdRespond(T_CANDATA *TxData,T_CANDATA *RxData)
{	
	if(TxData->SentBusy != 1)
	{
		return;
	}
	
	if(RxData->Data[2] == 0x01)
	{
		TxData->SentBusy = 0;
	}
}

/******************************************************************************
* Function Name : MslCANPeriodTask
* Description   : Period Task
* Argument      : handle the Can Task;
* Return Value  : none
******************************************************************************/
void MslCANPeriodTask(void)
{
	if(gsTxCANData.SentBusy == 1)
	{
		if(GetTimeMs() - gsTxCANData.SendTime > RESENT_TIME)
		{
			/*Sent CAN Data Twice*/
			MslCANDataSent(&gsTxCANData);
			gsTxCANData.ReSentCount++;
			if(gsTxCANData.ReSentCount > (RESENT_COUNT - 1))
			{
				gsTxCANData.ReSentCount = 0;
				gsTxCANData.SentBusy = 0;
			}
		}
	}
}

/******************************************************************************
* Function Name : MslCANSentFromSDK
* Description   : Service for Can Bus after SDK decode
* Argument      : Opcodes used to save data;
* Return Value  : which channel was Received
******************************************************************************/
void MslCANSentFromSDK(uint8_t Opcodes)
{
	switch(Opcodes)
	{
		case 0x01:	/*Unlock*/
			MslCANCmdExecute(DOOR_UNLOCK);
		break;
			
		case 0x02:	/*Lock*/
			MslCANCmdExecute(DOOR_LOCK);
		break;
		
		case 0x03:	/*Trunk Open*/
		  MslCANCmdExecute(TRUNK_UNLOCK);
		break;
		
		/*...*/
		
		case 0x07:	/*Find Car*/
			MslCANCmdExecute(WHISTLE);
		break;
		
		case 0x0B:	/*Open Air*/
			MslCANCmdExecute(SKYLIGHT_OPEN);
		break;
			
		case 0x0C:	/*Close Air*/
			MslCANCmdExecute(SKYLIGHT_CLOSE);
		break;
		
		case 0x0D:	/*Up Windows*/
			MslCANCmdExecute(WINDOWNS_UP);
		break;
		
		case 0x0E:	/*Down Windows*/
			MslCANCmdExecute(WINDOWNS_DOWN);
		break;

		
	}
}





/******************************************************************************
* Function Name : MslCANRxTxDemo
* Description   : CAN Bus Receive
* Argument      : CANData used to save data;
* Return Value  : which channel was Received
******************************************************************************/
void MslCANRxTxDemo(void)
{
	if(1 == gFlagNewData1)
	{
		gsRxCANData.StdID = CANRecData1.ID;
		memcpy(&gsRxCANData.Data[0],&CANRecData1.DB[0],8);
		HAL_Can_CnTxframeByTxBuf(1,1,&CANRecData1);
		gFlagNewData1 = 0;
	}
	else if(2 == gFlagNewData2)
	{
		gsRxCANData.StdID = CANRecData2.ID;
		memcpy(&gsRxCANData.Data[0],&CANRecData2.DB[0],8);
		HAL_Can_CnTxframeByTxBuf(2,1,&CANRecData2);
		gFlagNewData2 = 0;
	}
	else if(3 == gFlagNewData3)
	{
		gsRxCANData.StdID = CANRecData3.ID;
		memcpy(&gsRxCANData.Data[0],&CANRecData3.DB[0],8);
	  HAL_Can_CnTxframeByTxBuf(3,1,&CANRecData3);
		gFlagNewData3 = 0;
	}	

}

/******************************************************************************
* Function Name : MslCANSentDemo
* Description   : Used to test CAN Sent Mode
* Argument      : void;
* Return Value  : void;
******************************************************************************/
void MslCANSentDemo(void)
{
	Can_FrameType TxFrame;
	uint8_t Num;
	static uint32_t LastRecordTime;
	uint8_t DataTemp[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};

	while(1)
	{
		if(GetTimeMs()-LastRecordTime<100)
		{
			LastRecordTime = GetTimeMs();
			
			TxFrame.ID = 0x81;		// load ID info
			TxFrame.THLEN = 0;
			TxFrame.RTR = 0;
			TxFrame.IDE = 1;

			TxFrame.TS = 0x0000;
			TxFrame.LBL = 0x000;
			TxFrame.DLC = 0x8;
			memcpy(&TxFrame.DB[0],DataTemp,8); // load Data info
			
			for(Num=0;Num<15;Num++)
			{
				HAL_Can_CnTxframeByTxBuf(1,Num,&TxFrame);
			}
			for(Num=0;Num<15;Num++)
			{
				HAL_Can_CnTxframeByTxBuf(2,Num,&TxFrame);
			}
			for(Num=0;Num<15;Num++)
			{
				HAL_Can_CnTxframeByTxBuf(3,Num,&TxFrame);
			}
		}
	}
}

/*when CANInitFinish ,Sent One Frame to Bus as test*/
void MslCANDemoOneFrame(void)
{
	T_CANDATA CanData={0x81,{0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88}};
	MslCANDataSent(&CanData);
}
/******************************************************************************
** Function:    BllCANPeriodTask
** Description: Periodic call
** Parameter:   None
** Return:      None
******************************************************************************/
void BllCANPeriodTask(void)    		// CAN Period Task
{
	uint8_t FlagReception = 0;

	MslAtPeriodTask();	 /*AT Task Period Task*/
	MslGattPeriodTask(); /*GATT Task Period Task*/
	
	/*judge to received new CAN data*/
	FlagReception = MslCANReceive(&gsRxCANData); 
	if(FlagReception != 0)
	{
		switch(gsRxCANData.StdID)
		{
			case ID_KESSY04_STATUS: //fire buttom pressed
				BllFireJudgeHandle(&gsRxCANData);	//one key fire
			break;
			
			case ID_RESPONDSE_GW:	//CAN resent handle 
				MslCANRespondTask(&gsRxCANData);
			break;
			
			case ID_ZV_02:	//Door status & lock status
				MslGetDoorStatus(&gsRxCANData);	//four door
				MslGetTrunkStatus(&gsRxCANData);	//trunk
				MslGetLeftBackDoorLockStatus(&gsRxCANData);	//leftback door
				MslGetRightBackDoorLockStatus(&gsRxCANData);	//rightback door
			break;
			
			case ID_BCM_01:	//Engine Cover status
				MslGetEngineCoverStatus(&gsRxCANData);	//engine cover
			break;
			
			case ID_TSG_FT_01:	//main driver door Lock status & window status
				MslGetMainDoorLockStatus(&gsRxCANData);	//main door
				MslGetMainDriverWindowStatus(&gsRxCANData);	//main window
			break;
			
			case ID_TSG_BT_01:	//sub driver door Lock status & window status
				MslGetSubDoorLockStatus(&gsRxCANData);	//sub door
				MslGetSubDriverWindowStatus(&gsRxCANData);	//sub window
			break;
			
			case ID_TSG_HFS_01:	//leftback door Lock status & window status
				MslGetLeftBackWindowStatus(&gsRxCANData);	//leftback window
			break;
			
			case ID_TSG_HBFS_01:	//rightback door Lock status & window status
				MslGetRightBackWindowStatus(&gsRxCANData);	//rightback window
			break;
			
			case ID_MD1_Position:	//leftback door Lock status & window status
				MslGetSkyLightStatus(&gsRxCANData);	//skylight window
			break;
			
			case ID_KLIMA_03:	//inside car temperature
				MslGetCarInsideTemperature(&gsRxCANData);	//get inside car temperature
			break;
			
			case ID_KLIMA_SENSOR_02:	//outside car temperature
				MslGetCarOutsideTemperature(&gsRxCANData);	//get outside car temperature
			break;
	
			/*...*/
			
			default:
			
			break;
			
		}
	}
	
}

/******************************************************************************
** Function:    BllFireJudgeHandle
** Description: To determine whether ignition
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void BllFireJudgeHandle(T_CANDATA *RxData)
{	
	/*when the fire button was pressed*/
	if(BllGetValueCan(RxData,STATUS_FIRE_BUTTOM) == 0x01)           /*startbit:16 bitlen:1*/
	{
		SetKeyStatus(0); 	//Clear key status
		MslAtCmdSent(REQ_KEYSITE);	//Sent Request of key status
		SetFireButtom(1);
	}
}

/******************************************************************************
** Function:    MslGetDoorStatus
** Description: To get the status of four door
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetDoorStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusFourDoor = BllGetValueCan(RxData,STATUS_DOOR);
}

/******************************************************************************
** Function:    MslGetMainDoorLockHandle
** Description: To get the status of main door lock
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetMainDoorLockStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusMainDoorLock = BllGetValueCan(RxData,STATUS_PILOT_DOORLOCK);
}

/******************************************************************************
** Function:    MslGetSubDoorLockHandle
** Description: To get the status of sub door lock
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetSubDoorLockStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusSubDoorLock = BllGetValueCan(RxData,STATUS_PSSENGER_DOORLOCK);
}

/******************************************************************************
** Function:    MslGetLeftBackDoorLockHandle
** Description: To get the status of leftback door lock
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetLeftBackDoorLockStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusLeftBackDoorLock = BllGetValueCan(RxData,STATUS_LEFTBACK_DOORLOCK);
}

/******************************************************************************
** Function:    MslGetRightBackDoorLockHandle
** Description: To get the status of leftback door lock
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetRightBackDoorLockStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusRightBackDoorLock = BllGetValueCan(RxData,STATUS_RIGHTBACK_DOORLOCK);
}

/******************************************************************************
** Function:    MslGetTrunkStatusHandle
** Description: To get the status of car trunk
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetTrunkStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusTrunk = BllGetValueCan(RxData,STATUS_TRUNK);
}

/******************************************************************************
** Function:    MslGetEngineCoverStatusHandle
** Description: To get the status of engine cover
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetEngineCoverStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusEngineCover = BllGetValueCan(RxData,STATUS_ENGINE_COVER);
}

/******************************************************************************
** Function:    MslGetEngineCoverStatusHandle
** Description: To get the status of car trunk
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetMainDriverWindowStatus(T_CANDATA *RxData)
{
	gsCarStatus.MainCarWindow = BllGetValueCan(RxData,STATUS_PILOT_WINDOW)/20;
}

/******************************************************************************
** Function:    MslGetEngineCoverStatusHandle
** Description: To get the status of car trunk
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetSubDriverWindowStatus(T_CANDATA *RxData)
{
	gsCarStatus.SubCarWindow = BllGetValueCan(RxData,STATUS_PSSENGER_WINDOW)/20;
}

/******************************************************************************
** Function:    MslGetLeftBackWindowStatus
** Description: To get the status of car trunk
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetLeftBackWindowStatus(T_CANDATA *RxData)
{
	gsCarStatus.LeftBackWindow = BllGetValueCan(RxData,STATUS_LEFTBACK_WINDOW)/20;
}

/******************************************************************************
** Function:    MslGetRightBackWindowStatus
** Description: To get the status of car trunk
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetRightBackWindowStatus(T_CANDATA *RxData)
{
	gsCarStatus.RightBackWindow = BllGetValueCan(RxData,STATUS_RIGHTBACK_WINDOW)/20;
}

/******************************************************************************
** Function:    MslGetSkyLightStatus
** Description: To get the status of skylight
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetSkyLightStatus(T_CANDATA *RxData)
{
	gsCarStatus.StatusSkyLight = BllGetValueCan(RxData,STATUS_SKYLIGHT_WINDOW)/10;
}

/******************************************************************************
** Function:    MslGetCarInsideTemperature
** Description: To get the status of skylight
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetCarInsideTemperature(T_CANDATA *RxData)
{
	gsCarStatus.CarInsideTemperature = BllGetValueCan(RxData,INSIDE_TEMPER);
}

/******************************************************************************
** Function:    MslGetCarOutsideTemperature
** Description: To get the status of skylight
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetCarOutsideTemperature(T_CANDATA *RxData)
{
	gsCarStatus.CarOutsideTemperature = BllGetValueCan(RxData,OUTSIDE_TEMPER);
}

/******************************************************************************
** Function:    MslGetCarSpeed
** Description: To get the speed of car
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
void MslGetCarSpeed(T_CANDATA *RxData)
{
	if(BllGetValueCan(RxData,SPEED_UNIT) == 1)
		gsCarStatus.CarSpeed = BllGetValueCan(RxData,SPEED_VEHICLE)/1000;
	else
		gsCarStatus.CarSpeed = BllGetValueCan(RxData,SPEED_VEHICLE);
}

/*******************************************************/
/*basic function list*/
uint8_t DataMask[] = {0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};
/******************************************************************************
** Function:    BllGetValueCan
** Description: Get value of Can data
** Parameter:   Received data struct point
** Return:      None
******************************************************************************/
uint8_t BllGetValueCan(T_CANDATA *RxData,uint8_t Select)
{
	uint8_t Retval,Bytestart,Bitstart,Valuelen;
	
	Bytestart = MaskMatrix[Select].StartBit/8;
	Bitstart  = MaskMatrix[Select].StartBit%8;
	Valuelen  = MaskMatrix[Select].BitLenth;
	
	Retval = (RxData->Data[Bytestart]>>Bitstart)&DataMask[Valuelen];

	return Retval;
}
//line 918
