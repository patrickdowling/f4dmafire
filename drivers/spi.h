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
#ifndef F4DMAFIRE_DRIVERS_SPI_H_
#define F4DMAFIRE_DRIVERS_SPI_H_

#include "gpio.h"
#include "stm32f4xx.h"

namespace f4dmafire {

class Spi {
public:
  Spi() = default;

  void Init();

  enum TxType { COMMAND, DATA };

  template <uint8_t command, typename... Data>
  void Command(Data &&...data)
  {
    {
      ChipSelect<COMMAND> cs;
      Tx(command);
      Flush();
    }
    if constexpr (sizeof...(data)) {
      ChipSelect<DATA> cs;
      (Tx(data), ...);
      Flush();
    }
  }

  template <TxType type>
  void Write(uint8_t b)
  {
    ChipSelect<type> cs;
    SPI1->DR = b;
    Flush();
  }

  void WriteData16(uint16_t data)
  {
    ChipSelect<DATA> cs;
    SPI1->DR = data >> 8;
    while (!(SPI1->SR & SPI_SR_TXE)) {}
    SPI1->DR = data & 0xff;
    Flush();
  }

  void Tx16(const void *data, size_t length)
  {
    SetDataSize(SPI_DataSize_16b);
    ChipSelect<DATA> cs;
    const uint16_t *src = static_cast<const uint16_t *>(data);
    while (length--) {
      while (!(SPI1->SR & SPI_SR_TXE)) {}
      SPI1->DR = *src++;
    }
    Flush();
    SetDataSize(SPI_DataSize_8b);
  }

  void Flush()
  {
    while (!(SPI1->SR & SPI_SR_TXE)) {}
    while (SPI1->SR & SPI_SR_BSY) {}
  }

  void StartDMA(const void *buffer0, const void *buffer1, size_t length);

private:
  void SetDataSize(uint16_t);

  template <TxType type>
  struct ChipSelect {
    ChipSelect()
    {
      gpio.DISP_CS = 0;
      gpio.DISP_DC = COMMAND == type ? false : true;
    }

    ~ChipSelect() { gpio.DISP_CS = 1; }
  };

  static inline void Tx(uint8_t byte)
  {
    while (!(SPI1->SR & SPI_SR_TXE)) {}
    SPI1->DR = byte;
  }

  DISALLOW_COPY_AND_ASSIGN(Spi);
};

}  // namespace f4dmafire

#endif  // F4DMAFIRE_DRIVERS_SPI_H_

