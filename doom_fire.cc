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
#include "doom_fire.h"

#include <string.h>

#include "stm32f4xx_rng.h"

namespace gfx {

static constexpr uint8_t kFireMinVal = 0;
static constexpr uint8_t kFireMaxVal = kFireMinVal + 36;

static constexpr uint16_t ARGB(uint8_t, uint8_t r, uint8_t g, uint8_t b)
{
  return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

const uint16_t DoomFire::palette[] = {
    // DOOM FIRE
    ARGB(0xff, 0x07, 0x07, 0x07), ARGB(0xff, 0x1F, 0x07, 0x07), ARGB(0xff, 0x2F, 0x0F, 0x07),
    ARGB(0xff, 0x47, 0x0F, 0x07), ARGB(0xff, 0x57, 0x17, 0x07), ARGB(0xff, 0x67, 0x1F, 0x07),
    ARGB(0xff, 0x77, 0x1F, 0x07), ARGB(0xff, 0x8F, 0x27, 0x07), ARGB(0xff, 0x9F, 0x2F, 0x07),
    ARGB(0xff, 0xAF, 0x3F, 0x07), ARGB(0xff, 0xBF, 0x47, 0x07), ARGB(0xff, 0xC7, 0x47, 0x07),
    ARGB(0xff, 0xDF, 0x4F, 0x07), ARGB(0xff, 0xDF, 0x57, 0x07), ARGB(0xff, 0xDF, 0x57, 0x07),
    ARGB(0xff, 0xD7, 0x5F, 0x07), ARGB(0xff, 0xD7, 0x5F, 0x07), ARGB(0xff, 0xD7, 0x67, 0x0F),
    ARGB(0xff, 0xCF, 0x6F, 0x0F), ARGB(0xff, 0xCF, 0x77, 0x0F), ARGB(0xff, 0xCF, 0x7F, 0x0F),
    ARGB(0xff, 0xCF, 0x87, 0x17), ARGB(0xff, 0xC7, 0x87, 0x17), ARGB(0xff, 0xC7, 0x8F, 0x17),
    ARGB(0xff, 0xC7, 0x97, 0x1F), ARGB(0xff, 0xBF, 0x9F, 0x1F), ARGB(0xff, 0xBF, 0x9F, 0x1F),
    ARGB(0xff, 0xBF, 0xA7, 0x27), ARGB(0xff, 0xBF, 0xA7, 0x27), ARGB(0xff, 0xBF, 0xAF, 0x2F),
    ARGB(0xff, 0xB7, 0xAF, 0x2F), ARGB(0xff, 0xB7, 0xB7, 0x2F), ARGB(0xff, 0xB7, 0xB7, 0x37),
    ARGB(0xff, 0xCF, 0xCF, 0x6F), ARGB(0xff, 0xDF, 0xDF, 0x9F), ARGB(0xff, 0xEF, 0xEF, 0xC7),
    ARGB(0xff, 0xFF, 0xFF, 0xFF)
    // DOOM FIRE
};

void DoomFire::Init()
{
  uint8_t *buffer = buffer_;
  size_t i = 0;
  for (; i < kFireHeight - 1; ++i) {
    memset(buffer, kFireMinVal, kFireWidth);
    buffer += kFireWidth;
  }

  memset(buffer, kFireMaxVal, kFireWidth);

  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
  RNG_Cmd(ENABLE);
}

void DoomFire::Render(uint16_t *scanline)
{
  auto line = line_;
  uint8_t *dst = buffer_ + kFireWidth * line;
  uint8_t *src = dst + kFireWidth;

  if (line < kFireHeight - 1) {
    for (size_t i = 0; i < kFireWidth; ++i) {
      auto val = *src++;
      uint8_t r = RNG_GetRandomNumber() & 0x3;

      auto idx = (i - r + 1);
      if (idx > kFireWidth) idx -= kFireWidth;
      if (val) val -= (r & 0x1);

      dst[idx] = val;
      *scanline++ = palette[val];
    }
  } else {
    for (size_t i = 0; i < kFireWidth; ++i) *scanline++ = palette[0];
  }

  ++line;
  if (line > kFireHeight - 1)
    line_ = 0;
  else
    line_ = line;
}

}  // namespace gfx
