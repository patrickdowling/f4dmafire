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
#include <algorithm>

#include "ST7789.h"
#include "doom_fire.h"
#include "drivers/gpio.h"
#include "drivers/spi.h"
#include "stm32x/stm32x_core.h"

static constexpr uint32_t kSysTickUpdate = 1000UL;
STM32X_CORE_DEFINE()

namespace f4dmafire {
GPIO gpio;
Spi spi;
}  // namespace f4dmafire

using namespace f4dmafire;

static gfx::DoomFire doom_fire;
uint16_t scanline_buffers[2][gfx::DoomFire::kFireWidth * 2];

namespace ST7789 {

static void SetWritePos(uint16_t Xstart, uint16_t Xend, uint16_t Ystart, uint16_t Yend)
{
  Xstart += 52;
  Xend += 52;
  spi.Command<ST7789::CASET>(Xstart >> 8, Xstart & 0xff, Xend >> 8, Xend & 0xff);

  Ystart += 40;
  Yend += 40;
  spi.Command<ST7789::PASET>(Ystart >> 8, Ystart & 0xff, Yend >> 8, Yend & 0xff);

  spi.Command<ST7789::RAMWR>();
}

void DrawRect(unsigned int x, unsigned int y, unsigned w, unsigned h, unsigned color)
{
  SetWritePos(x, x + w - 1, y, y + h - 1);
  for (unsigned i = 0; i < w * h; ++i) spi.WriteData16(color);
}
}  // namespace ST7789

extern "C" void SysTick_Handler()
{
  STM32X_CORE_TICK();
}

extern "C" void DMA2_Stream3_IRQHandler()
{
  gpio.LED = 1;
  if (DMA2->LISR & DMA_IT_TCIF3) {
    DMA2->LIFCR = DMA_IT_TCIF3;
    auto scanline = scanline_buffers[DMA2_Stream3->CR & DMA_SxCR_CT ? 1 : 0];
    doom_fire.Render(scanline);
    memcpy(scanline + gfx::DoomFire::kFireWidth * 2, scanline, gfx::DoomFire::kFireWidth);
  }
  gpio.LED = 0;
}

int main()
{
  spi.Init();
  STM32X_CORE_INIT(F_CPU / kSysTickUpdate);

  gpio.DISP_CS = 1;
  gpio.DISP_RST = 0;

  gpio.DISP_RST = 1;
  stm32x::core.Delay(5);
  gpio.DISP_RST = 0;
  stm32x::core.Delay(20);
  gpio.DISP_RST = 1;
  stm32x::core.Delay(150);

  // Initial setup
  spi.Command<ST7789::MADCTL>(0x00);
  // spi.Write<Spi::DATA>(0x00);  // vertical screen  0
  // spi.Write<Spi::DATA>(0xC0);//vertical screen  180
  // spi.Write<Spi::DATA>(0x70);//horizontal screen 90
  // spi.Write<Spi::DATA>(0xA0);//horizontal screen 270

  spi.Command<ST7789::COLMOD>(0x05);

  spi.Command<ST7789::PORCTRL>();
  spi.Write<Spi::DATA>(0x0C);
  spi.Write<Spi::DATA>(0x0C);
  spi.Write<Spi::DATA>(0x00);
  spi.Write<Spi::DATA>(0x33);
  spi.Write<Spi::DATA>(0x33);

  spi.Command<ST7789::GCTRL>(0x35);
  spi.Command<ST7789::VCOMS>(0x19);
  spi.Command<ST7789::LCMCTRL>(0x2C);
  spi.Command<ST7789::VDVVRHEN>(0x01);
  spi.Command<ST7789::VRHS>(0x12);
  spi.Command<ST7789::VDVS>(0x20);
  spi.Command<ST7789::FRCTRL2>(0x0F);
  spi.Command<ST7789::PWCTRL1>(0xA4, 0xA1);

  spi.Command<ST7789::PVGAMCTRL>();
  spi.Write<Spi::DATA>(0xD0);
  spi.Write<Spi::DATA>(0x04);
  spi.Write<Spi::DATA>(0x0D);
  spi.Write<Spi::DATA>(0x11);
  spi.Write<Spi::DATA>(0x13);
  spi.Write<Spi::DATA>(0x2B);
  spi.Write<Spi::DATA>(0x3F);
  spi.Write<Spi::DATA>(0x54);
  spi.Write<Spi::DATA>(0x4C);
  spi.Write<Spi::DATA>(0x18);
  spi.Write<Spi::DATA>(0x0D);
  spi.Write<Spi::DATA>(0x0B);
  spi.Write<Spi::DATA>(0x1F);
  spi.Write<Spi::DATA>(0x23);

  spi.Command<ST7789::NVGAMCTRL>();
  spi.Write<Spi::DATA>(0xD0);
  spi.Write<Spi::DATA>(0x04);
  spi.Write<Spi::DATA>(0x0C);
  spi.Write<Spi::DATA>(0x11);
  spi.Write<Spi::DATA>(0x13);
  spi.Write<Spi::DATA>(0x2C);
  spi.Write<Spi::DATA>(0x3F);
  spi.Write<Spi::DATA>(0x44);
  spi.Write<Spi::DATA>(0x51);
  spi.Write<Spi::DATA>(0x2F);
  spi.Write<Spi::DATA>(0x1F);
  spi.Write<Spi::DATA>(0x1F);
  spi.Write<Spi::DATA>(0x20);
  spi.Write<Spi::DATA>(0x23);

  spi.Command<ST7789::INVON>();  // interesting...
  spi.Command<ST7789::SLPOUT>();
  stm32x::core.Delay(120);
  spi.Command<ST7789::DISPON>();

  ST7789::DrawRect(0, 0, ST7789::kScreenWidth, ST7789::kScreenHeight, gfx::DoomFire::palette[0]);
  gpio.DISP_BL.Set();

  doom_fire.Init();

  auto w = gfx::DoomFire::kFireWidth;
  auto h = 2 * gfx::DoomFire::kFireHeight;
  auto x = 0;  //( - w) / 2;
  auto y = 0;  //(240 - h) / 2;
  ST7789::SetWritePos(x, x + w - 1, y, y + h - 1);
  doom_fire.Render(scanline_buffers[0]);
  doom_fire.Render(scanline_buffers[1]);
  spi.StartDMA(scanline_buffers[0], scanline_buffers[1], w * 2);

  while (true) {}
}

