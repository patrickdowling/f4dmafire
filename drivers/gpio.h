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
// -----------------------------------------------------------------------------
//
// AUTOMATICALLY GENERATED FILE, DO NOT EDIT
// clang-format off
// IOC source: ./stm32/STM32F405RGTx.ioc
// CSV source: ./stm32/STM32F405RGTx.csv
// --------------------------------------------------------------------------------
#ifndef F4DMAFIRE_GPIO_H_
#define F4DMAFIRE_GPIO_H_
#include "stm32x/stm32x_core.h"

namespace f4dmafire {
class GPIO {
  struct GPIO_PORT_INIT {
    GPIO_PORT_INIT() {
      stm32x::GPIOx<stm32x::GPIO_PORT_A>::EnableClock(true);
      stm32x::GPIOx<stm32x::GPIO_PORT_B>::EnableClock(true);
      stm32x::GPIOx<stm32x::GPIO_PORT_C>::EnableClock(true);
    }
  };
  GPIO_PORT_INIT port_initializer_;

public:
  GPIO() = default;
  DISALLOW_COPY_AND_ASSIGN(GPIO);
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_IN<0, stm32x::GPIO_PUPD::NONE> BUT;
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_AF<5, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE, 5/*GPIO_AF5_SPI1*/> SPI1_SCK;
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_AF<7, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE, 5/*GPIO_AF5_SPI1*/> SPI1_MOSI;
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_OUT<0, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> DISP_BL;
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_OUT<7, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> DISP_CS;
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_OUT<8, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> DISP_DC;
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_OUT<9, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> DISP_RST;
  stm32x::GPIOx<stm32x::GPIO_PORT_C>::GPIO_OUT<12, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::PULLUP> LED;
}; // class GPIO

extern GPIO gpio;
} // namespace f4dmafire
#endif // F4DMAFIRE_GPIO_H_
