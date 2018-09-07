#ifndef _BSP_DATAFLASH_H_
#define _BSP_DATAFLASH_H_

void BspFlashInit( void );
void BspDataFlashWrite(uint32_t WriteAddr,uint32_t *pBuffer,uint16_t NumToWrite);
void BspDataFlashRead(uint32_t FlashAddress,uint32_t *Data,uint32_t NumToRead);
int SdkWrite(uint8_t *InData,uint8_t Wlen,uint8_t offset);
int SdkWrite17(uint8_t *InData,uint8_t Wlen,uint8_t offset);
int SdkWrite48(uint8_t *InData,uint8_t Wlen,uint8_t offset);
int SdkRead(uint8_t *OutData,uint8_t Rlen,uint8_t offset);
int SdkRead17(uint8_t *OutData,uint8_t Rlen,uint8_t offset);
int SdkRead48(uint8_t *OutData,uint8_t Rlen,uint8_t offset);

#endif
