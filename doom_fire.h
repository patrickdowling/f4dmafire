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
#ifndef DOOM_FIRE_H_
#define DOOM_FIRE_H_

#include <stdint.h>
#include <stdlib.h>

namespace gfx {

class DoomFire {
public:
  static constexpr size_t kFireWidth = SCREEN_WIDTH;
  static constexpr size_t kFireHeight = SCREEN_HEIGHT / 2;

  void Init();
  void Render(uint16_t *scanline);

  static const uint16_t palette[];

private:
  uint8_t buffer_[kFireWidth * kFireHeight];
  size_t line_ = 0;
};

}  // namespace gfx

#endif  // DOOM_FIRE_H_
