#ifndef __SBSP_SPI_H__
#define __SBSP_SPI_H__
//控制是否启用这个模块
#define __SBSP_SPI_EN__
#ifdef  __SBSP_SPI_EN__


#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C"{
#endif

#define SBSP_SPI


void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi);
void sBSP_SPI1_Init(uint8_t SPI_BAUDRATE);
void sBSP_SPI1_SetCS(uint8_t cs_en);

void sBSP_SPI1_SendBytes(uint8_t *pData,uint16_t Size);
void sBSP_SPI1_ReciBytes(uint8_t *pData,uint16_t Size);

void sBSP_SPI1_SendByte(uint8_t byte);
uint8_t sBSP_SPI1_ReciByte();


void sBSP_SPI1_SetEN(uint8_t en);


#ifdef __cplusplus
}
#endif

#endif
#endif

