#ifndef ST7789_H_
#define ST7789_H_

#include <cstdint>

namespace ST7789 {

static constexpr unsigned kScreenWidth = SCREEN_WIDTH;
static constexpr unsigned kScreenHeight = SCREEN_HEIGHT;

enum COMMAND : uint8_t {

  SLPOUT = 0x11,

  INVON = 0x21,
  DISPON = 0x29,
  CASET = 0x2A,
  PASET = 0x2B,
  RAMWR = 0x2C,

  COLMOD = 0x3A,
  MADCTL = 0x36,

  PORCTRL = 0xB2,
  GCTRL = 0xB7,

  VCOMS = 0xBB,
  LCMCTRL = 0xC0,
  VDVVRHEN = 0xC2,
  VRHS = 0xC3,
  VDVS = 0xC4,
  FRCTRL2 = 0xC6,

  PWCTRL1 = 0xD0,

  PVGAMCTRL = 0xE0,
  NVGAMCTRL = 0xEF,
};

}  // namespace ST7789

#endif /* ST7789_H_ */
