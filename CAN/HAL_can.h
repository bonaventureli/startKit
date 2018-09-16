/******************************************************************************
** Filename:    rscan.h
** Description: This files is the Driver of CAN Bus in communication.
** Author:  lifei
** Date: 2018.8.3
** Copyright (c) by ShangHai  Information Technology Co.Ltd
******************************************************************************/
#ifndef _HAL_CAN_H_
#define _HAL_CAN_H_

#define CAN_NUM       				1 
/* 1~3 ch. Which CAN to be Used Very Wainning
only the channel CAN_NUM can be Used */

#define RX_RULE_NUM_MAX              (192U)       /* Max Rx Rule number */
#define RX_RULE_PAGE_NUM             (1U)         /* Rx Rule Table page number */

#define RX_RULE_NUM                  (32U)         /* Rx Rule number */
#define RX_RULE_NUM_CH0              (16U)         /* Channel 0 Rx rule number */
#define RX_RULE_NUM_CH1              (8U)         /* Channel 1 Rx rule number */
#define RX_RULE_NUM_CH2              (8U)         /* Channel 2 Rx rule number */
#define RX_RULE_NUM_CH3              (0U)         /* Channel 3 Rx rule number */
#define RX_RULE_NUM_CH4              (0U)         /* Channel 4 Rx rule number */
/* structure */
typedef struct {
  unsigned long lword[4];
}can_cre_type;
typedef struct SignalPosition{
	uint8_t 	StartBit;
	uint8_t 	BitLenth;
}T_MASK_MATRIX;  //Used to Matrix of CAN
/*...*/
#if COMPILER == COMP_GHS
  #if 1
  /* ==== basic type ==== */
  typedef unsigned char U8;
  typedef signed char S8;
  typedef unsigned short U16;
  typedef signed short S16;
  typedef unsigned long U32;
  typedef signed long S32;
  typedef unsigned char BOOL;
  typedef volatile unsigned char VU8;
  typedef volatile unsigned short VU16;
  typedef volatile unsigned long VU32;
  #endif
#endif

#if COMPILER == COMP_IAR
  #if 0
  /* ==== basic type ==== */
  typedef unsigned char U8;
  typedef signed char __no_bit_access S8;
  typedef unsigned short __no_bit_access U16;
  typedef signed short __no_bit_access S16;
  typedef unsigned long U32;
  typedef signed long __no_bit_access S32;
  typedef unsigned char __no_bit_access BOOL;
  typedef volatile unsigned char __no_bit_access VU8;
  typedef volatile unsigned short __no_bit_access VU16;
  typedef volatile unsigned long __no_bit_access VU32;
  #endif
#endif


extern const can_cre_type RX_RULE_TABLE_LIST[RX_RULE_NUM];  


/* ---- CAN frame ----- */
typedef struct
{
    U32 ID :29;
    U32 THLEN :1;
    U32 RTR :1;
    U32 IDE :1;
    
    U32 TS :16;
    U32 LBL :12;
    U32 DLC :4;
    U8 DB[8];
} Can_FrameType;


typedef struct CanMatrix{
	uint32_t  StdID;
	uint8_t StartByte;
	uint8_t StartBit;
	uint8_t BitLenth;
	uint8_t SignValue;
}T_CAN_MATRIX;  //Used to Matrix of CAN

typedef struct CANData{
  uint32_t StdID;
  uint8_t	 Data[8];
	uint8_t	 SentBusy;
	uint32_t SendTime;
	uint8_t  ReSentCount;
	
}T_CANDATA;  // The Define of CAN Data Struct
/*---- structure for Tx Frame information --------------------*/
/* ---- CAN function return ---- */
typedef U8 Can_RtnType;


#define CAN_RTN_OK                          0U
#define CAN_RTN_FIFO_FULL                   1U
#define CAN_RTN_BUFFER_EMPTY                2U
#define CAN_RTN_ERR                         255U

#ifndef CAN_ENABLE
#define CAN_ENABLE                          1U
#endif
#ifndef CAN_DISABLE
#define CAN_DISABLE                         0U
#endif

/* ---- bit value ---- */
#define CAN_SET                             1U
#define CAN_CLR                             0U

/* ---- bit mask ---- */
#define CAN_1_BIT_MASK                      1U
#define CAN_2_BIT_MASK                      3U
#define CAN_3_BIT_MASK                      7U
#define CAN_4_BIT_MASK                      0xfU
#define CAN_5_BIT_MASK                      0x1fU

/* ---- bit position ---- */
#define CAN_B0_BIT_POS                      0U
#define CAN_B1_BIT_POS                      1U
#define CAN_B2_BIT_POS                      2U
#define CAN_B3_BIT_POS                      3U
#define CAN_B4_BIT_POS                      4U
#define CAN_B5_BIT_POS                      5U
#define CAN_B6_BIT_POS                      6U
#define CAN_B7_BIT_POS                      7U
#define CAN_B8_BIT_POS                      8U
#define CAN_B9_BIT_POS                      9U
#define CAN_B10_BIT_POS                     10U
#define CAN_B11_BIT_POS                     11U
#define CAN_B12_BIT_POS                     12U
#define CAN_B13_BIT_POS                     13U
#define CAN_B14_BIT_POS                     14U
#define CAN_B15_BIT_POS                     15U

/* **** CAN channel usage **** */
#define RS_CAN_CH3                 CAN_ENABLE  //CAN_DISABLE

#define RS_CAN_CH4                 CAN_ENABLE

/* ---- CAN resource ---- */
/*#define CAN_CHNL_NUM                        3U
#define CAN_RFIFO_NUM                       8U
#define CAN_CHNL_TRFIFO_NUM                 3U
#define CAN_TRFIFO_NUM                      (CAN_CHNL_TRFIFO_NUM * CAN_CHNL_NUM)
#define CAN_FIFO_NUM                        (CAN_RFIFO_NUM + CAN_TRFIFO_NUM)
#define CAN_CHNL_TX_BUF_NUM                 16U
*/
#define CAN_CRBRCF0_RX_BUF_NUM              32U
#define CAN_CRBRCF1_RX_BUF_NUM              32U
#define CAN_CRBRCF2_RX_BUF_NUM              32U
#define CAN_PAGE_RX_RULE_IDX_MASK           0xfU
#define CAN_RX_RULE_PAGE_IDX_BIT_POS        4U
#define CAN_RAM_LW_SIZE                     (0x1C20U >> 2U)
#define CAN_RAM_PAGE_IDX_BIT_POS            6U
#define CAN_RAM_DATA_IDX_MASK               0x3fU

#define RX_RULE_NUM_MAX              (192U)       /* Max Rx Rule number */
#define RX_RULE_PAGE_NUM             (1U)         /* Rx Rule Table page number */


/* ==== Rx rule table (Refer to can_table.h) ==== */
#define CAN_C0RN                       RX_RULE_NUM_CH0
#define CAN_C1RN                       RX_RULE_NUM_CH1
#define CAN_C2RN                       RX_RULE_NUM_CH2
#define CAN_C3RN                       RX_RULE_NUM_CH3
#define CAN_C4RN                       RX_RULE_NUM_CH4
#define CAN_RX_RULE_NUM                (CAN_C0RN + CAN_C1RN + CAN_C2RN + CAN_C3RN + CAN_C4RN)
#define CAN_RX_RULE_TABLE              RX_RULE_TABLE_LIST
#define CAN_RX_RULE_TYPE               can_cre_type

/* ---- TX buffer, TX status flag ---- */
#define CAN_TBTST_NOT_TRANSMITTING          0U
#define CAN_TBTST_TRANSMITTING              1U

/* ---- bit operations ---- */
#define GET_BIT(reg, pos)              (((reg) >> (pos)) & 1U)
#define SET_BIT(reg, pos)              ((reg) |= 1U << (pos))
#define CLR_BIT(reg, pos)              ((reg) &= ~(1UL << (pos)))

/* ---- CiTBCRj ---- */
#define CAN_TBTR_BIT_POS                    0U
#define CAN_TBAR_BIT_POS                    1U
#define CAN_TBOE_BIT_POS                    2U

/* transmission command */
#define CAN_TBCR_TRM                        (CAN_ENABLE << CAN_TBTR_BIT_POS)


#define NUM_CANFRAME_ONCE 		1   // the value: 1~15

#define CAN_TX_RESPOND_EN 		1		// the value: 1/0
#define RESENT_TIME   				1000	// the value: 100~700(ms)
#define RESENT_COUNT  				1		// the value: 1~n

/****************************************************************************/

#define FRAME_PROPERTY  	0  // Config the can Data property
#define LAMP_BLINK    		1
#define DOOR_LOCK     		2
#define DOOR_UNLOCK     	3
#define WHISTLE       		4 // Beep
#define TRUNK_UNLOCK  		5
#define WINDOWNS_UP  	 		6
#define WINDOWNS_DOWN	 		7
#define FIND_CAR					4
#define SKYLIGHT_OPEN			8
#define SKYLIGHT_CLOSE 		9
#define WELCOME       		10
#define ENGINE_START  		11
/*Add Frame Option*/
#define FRAME_ASSEMBLY_1	12
#define FRAME_ASSEMBLY_2	13
/*CAN Matrix*/
static T_CAN_MATRIX CANCmdMatrix[] = {
/*NUM    StdID   StartByte StartBit VauleLen Value */
/*0*/  { 0x000,  0,        0,		    0,	     0x00}, // IDLE/FRAME_PROPERTY
/*1*/  { 0x100,  2,        0,		    8,	     0x01}, // LAMP_BLINK
/*2*/  { 0x100,  2,        0,		    8,	     0x02}, // DOOR_UNLOCK
/*3*/  { 0x100,  2,        0,		    8,	     0x03}, // DOOR_LOCK
/*4*/  { 0x100,  2,        0,		    8,	     0x0E}, // Hupe/Beep
/*5*/  { 0x100,  2,        0,		    8,	     0x0A}, // Tail openning
/*6*/  { 0x100,  2,        0,		    8,	     0x08}, // WINDOWNS_Raise 
/*7*/  { 0x100,  2,        0,		    8,	     0x09}, // WINDOWNS_Drop
                                             
/*8*/  { 0x100,  2,        0,		    8,	     0x0C}, // Skylight Openning
/*9*/  { 0x100,  2,        0,		    8,	     0x0D}, // Skylight Closing
/*10*/ { 0x100,  2,        0,		    8,	     0x12}, // Welcome... 
/*11*/ { 0x100,  2,        0,		    8,	     0x11}, // Engine Start...
                                             
/*12*/ { 0x000,  0,        0,		    1,	     0x01}, // FRAME_ASSEMBLY_1...
/*13*/ { 0x000,  1,        0,		    8,	     0x60}, // FRAME_ASSEMBLY_2...	
};   // The define of CAN Matrix    

extern T_CANDATA gsTxCANData;
extern T_CANDATA gsRxCANData;

/*
*******************************************************************************
**  Function define
******************************************************************************/

void HAL_RS_CAN_init(void);
void HAL_RS_CAN_initB(void);
void HAL_CanPinConfig(void);
void HAL_CanPinConfigB(void);

Can_RtnType HAL_Can_C0TxframeByTxBuf(U8 TxBufIdx, const Can_FrameType* pFrame);
Can_RtnType HAL_Can_C1TxframeByTxBuf(U8 TxBufIdx, const Can_FrameType* pFrame);
Can_RtnType HAL_Can_C2TxframeByTxBuf(U8 TxBufIdx, const Can_FrameType* pFrame);
Can_RtnType HAL_Can_CnTxframeByTxBuf(U8 Chn,U8 TxBufIdx, const Can_FrameType* pFrame);
static void HAL_Can_SetRxRule(void);
void HAL_INTRCANGRECC_interruptISR( void );
void HAL_IntCANRecvCallback( uint32_t regEIIC_value );

extern volatile uint8_t gFlagNewData1;
extern volatile uint8_t gFlagNewData2;
extern volatile uint8_t gFlagNewData3;
extern Can_FrameType CANRecData1;
extern Can_FrameType CANRecData2;
extern Can_FrameType CANRecData3;
void HAL_BspChmCanSent(uint32_t IDStd, uint8_t *Data,uint8_t DataLen);

/*CAN Period Task*/
void MslCANRespondTask(T_CANDATA *RxData);
void MslCANPeriodTask(void);

/*Handle SDK Decode CMD ,will call MslCANCmdExecute   */
void MslCANSentFromSDK(uint8_t Opcodes);
/*CAN CMD Execute*/
void MslCANCmdExecute(uint8_t Function);

static void MslCANCmdRespond(T_CANDATA *TxData,T_CANDATA *RxData);

void CANDataFill(T_CANDATA *CANData,uint8_t Function);
void MslCANDataSent(T_CANDATA *CANData);
uint8_t MslCANReceive(T_CANDATA *CANData);

void MslCANRxTxDemo(void);
void MslCANDemoOneFrame(void);

/*******************************************************/
/*list of function*/
void BllCANPeriodTask(void);
void BllFireJudgeHandle(T_CANDATA *RxData);
void MslGetDoorStatus(T_CANDATA *RxData);
void MslGetMainDoorLockStatus(T_CANDATA *RxData);
void MslGetSubDoorLockStatus(T_CANDATA *RxData);
void MslGetLeftBackDoorLockStatus(T_CANDATA *RxData);
void MslGetRightBackDoorLockStatus(T_CANDATA *RxData);
void MslGetTrunkStatus(T_CANDATA *RxData);
void MslGetEngineCoverStatus(T_CANDATA *RxData);
void MslGetMainDriverWindowStatus(T_CANDATA *RxData);
void MslGetSubDriverWindowStatus(T_CANDATA *RxData);
void MslGetLeftBackWindowStatus(T_CANDATA *RxData);
void MslGetRightBackWindowStatus(T_CANDATA *RxData);
void MslGetCarInsideTemperature(T_CANDATA *RxData);
void MslGetCarOutsideTemperature(T_CANDATA *RxData);
void MslGetSkyLightStatus(T_CANDATA *RxData);
void MslGetCarSpeed(T_CANDATA *RxData);
uint8_t BllGetValueCan(T_CANDATA *RxData,uint8_t Select);

#endif

