// Copyright 2022 Patrick Dowling
//
// Author: Patrick Dowling (pld@gurkenkiste.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// See http://creativecommons.org/licenses/MIT/ for more information.
//
#include "drivers/spi.h"

#include <string.h>

// SPI1_SCK  = PA5 = EXT1-18
// SPI1_MOSI = PA7 = EXT1-22
// CS        = PB7 = EXT1-15
// DC        = PB8 = EXT1-16
// RST       = PB9 = EXT1-17
// BL        = PB0 = EXT1-21

namespace f4dmafire {

void Spi::Init()
{
  gpio.DISP_CS = 1;
  gpio.DISP_DC = 1;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

  // Initialize SPI
  SPI_InitTypeDef spi_init;
  spi_init.SPI_Direction = SPI_Direction_1Line_Tx;
  spi_init.SPI_Mode = SPI_Mode_Master;
  spi_init.SPI_DataSize = SPI_DataSize_8b;
  spi_init.SPI_CPOL = SPI_CPOL_High;
  spi_init.SPI_CPHA = SPI_CPHA_1Edge;
  spi_init.SPI_NSS = SPI_NSS_Soft;
  spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  // 8=10.5MHz
  spi_init.SPI_FirstBit = SPI_FirstBit_MSB;
  spi_init.SPI_CRCPolynomial = 7;

  SPI_Init(SPI1, &spi_init);
  SPI_Cmd(SPI1, ENABLE);
}

void Spi::StartDMA(const void *buffer0, const void *buffer1, size_t length)
{
  SPI_Cmd(SPI1, DISABLE);
  SPI_DataSizeConfig(SPI1, SPI_DataSize_16b);

  DMA_DeInit(DMA2_Stream3);
  DMA_InitTypeDef dma_init;
  dma_init.DMA_Channel = DMA_Channel_3;
  dma_init.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
  dma_init.DMA_Memory0BaseAddr = 0;
  dma_init.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  dma_init.DMA_BufferSize = 0;
  dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  dma_init.DMA_Mode = DMA_Mode_Circular;
  dma_init.DMA_Priority = DMA_Priority_Medium;
  dma_init.DMA_FIFOMode = DMA_FIFOMode_Disable;
  dma_init.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  dma_init.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  dma_init.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream3, &dma_init);

  DMA2_Stream3->M0AR = (uint32_t)buffer0;
  DMA2_Stream3->M1AR = (uint32_t)buffer1;
  DMA2_Stream3->NDTR = length;
  DMA2_Stream3->CR &= ~DMA_SxCR_CT;
  DMA2_Stream3->CR |= DMA_SxCR_DBM;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 2:2 bits for pre-emption:subpriority
  NVIC_InitTypeDef nvic_init;
  nvic_init.NVIC_IRQChannel = DMA2_Stream3_IRQn;
  nvic_init.NVIC_IRQChannelPreemptionPriority = 2;
  nvic_init.NVIC_IRQChannelSubPriority = 2;
  nvic_init.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic_init);

  DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3);
  DMA_ITConfig(DMA2_Stream3, DMA_IT_TC, ENABLE);

  DMA2_Stream3->CR |= DMA_SxCR_EN;  // DMA_Cmd(DMA2_Stream3, ENABLE);
  SPI1->CR2 |= SPI_I2S_DMAReq_Tx;   // SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);

  gpio.DISP_CS = false;
  gpio.DISP_DC = true;
  SPI_Cmd(SPI1, ENABLE);
}

void Spi::SetDataSize(uint16_t datasize)
{
  SPI_Cmd(SPI1, DISABLE);
  SPI_DataSizeConfig(SPI1, datasize);
  SPI_Cmd(SPI1, ENABLE);
}

}  // namespace f4dmafire
