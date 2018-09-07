/******************************************************************************
** Filename:    rscan.h
** Description: This files is the Driver of CAN Bus in communication.
** Author:  Huangtingxuan
** Date: 2018.8.3
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/
//============================================================================
//
// Header file for the RS CAN
//
//============================================================================
#ifndef _RSCAN_H_
#define _RSCAN_H_

#define CAN_NUM       				1 /* 1~3 ch. Which CAN to be Used Very Wainning
only the channel CAN_NUM can be Used */

  
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


/*
*******************************************************************************
**  Register bit define
*******************************************************************************
*/

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

/*---- structure for Tx Frame information --------------------*/
/* ---- CAN function return ---- */
typedef U8 Can_RtnType;

/* ---- function return value ---- */
#define CAN_RTN_OK                          0U
#define CAN_RTN_FIFO_FULL                   1U
#define CAN_RTN_BUFFER_EMPTY                2U
#define CAN_RTN_ERR                         255U


/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/
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

/*
*******************************************************************************
**  Function define
******************************************************************************/

void RS_CAN_init(void);
void RS_CAN_initB(void);
void CanPinConfig(void);
void CanPinConfigB(void);

Can_RtnType Can_C0TrmByTxBuf(U8 TxBufIdx, const Can_FrameType* pFrame);
Can_RtnType Can_C1TrmByTxBuf(U8 TxBufIdx, const Can_FrameType* pFrame);
Can_RtnType Can_C2TrmByTxBuf(U8 TxBufIdx, const Can_FrameType* pFrame);
Can_RtnType Can_ChTrmByTxBuf(U8 Chn,U8 TxBufIdx, const Can_FrameType* pFrame);
static void Can_SetRxRule(void);

void INTRCANGRECC_interruptISR( void );
void IntCANRecvCallback( uint32_t regEIIC_value );

extern volatile uint8_t gFlagNewData1;
extern volatile uint8_t gFlagNewData2;
extern volatile uint8_t gFlagNewData3;
extern Can_FrameType CANRecData1;
extern Can_FrameType CANRecData2;
extern Can_FrameType CANRecData3;
void BspChmCanSent(uint32_t IDStd, uint8_t *Data,uint8_t DataLen);

#endif

