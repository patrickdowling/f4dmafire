# F4 + DMA + DooM fire
Q: What to do with a new display?

A: Fire!

![Fire Image](/resources/fire.jpg)

For giggles the fire is rendered in the DMA callback since there's not much else going on.
Each scanline is rendered twice.

## Hardware
- I used an Olimex STM32-H405 because that's what was available; the SPI pinout should easily transfer to a Discovery or similar.
- ST7789 135x240px SPI display, e.g.
  [this one](https://www.buydisplay.com/1-14-inch-tft-ips-lcd-display-module-135x240-spi-for-arduino-raspberry-pi)

- 135x240? Gah!

## Building
My preferred method is to set `TOOLCHAIN_PATH` using direnv, e.g. with a `.envrc` something like:
```
export TOOLCHAIN_PATH=~/dev/gcc-arm-none-eabi-10.3-2021.10/
```
