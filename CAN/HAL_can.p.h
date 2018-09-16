#ifndef _HAL_CAN_P_H_
#define _HAL_CAN_P_H_

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
#endif