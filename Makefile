###
## Project config
#
PROJECT = f4dmafire
PROJECT_INCLUDE_DIRS = .
PROJECT_SRC_DIRS = . drivers
PROJECT_IOC_FILE = ./stm32/STM32F405RGTx.ioc

PROJECT_DEFINES += SCREEN_WIDTH=135 SCREEN_HEIGHT=240

FLASH_TARGET = flash_openocd

OPTIMIZE=-O3
STM32X_CPPSTD=c++17
MAX_FRAME_SIZE=512

RAM_SIZE     = 128K
CCM_RAM_SIZE = 64K
FLASH_SIZE   = 1024K
MIN_STACK_SIZE = 2K
ENABLE_LIBC_INIT_ARRAY = TRUE
ENABLE_CCM_STACK = TRUE

MODEL = STM32F40_41xxx
F_CPU = 168000000L
HSE_VALUE = 8000000L

include stm32x/makefile

OPENOCD_INTERFACE = ftdi/olimex-arm-usb-tiny-h
OPENOCD = openocd -f interface/$(OPENOCD_INTERFACE).cfg -f target/stm32f4x.cfg

.PHONY: gdb_server
gdb_server: $(ELFFILE)
	$(OPENOCD) \
		-c "init" -c "reset" -c "halt"

.PHONY: gdb_client
gdb_client: $(ELFFILE)
	$(Q)$(GDB) --ex 'target remote localhost:3333' $(ELFFILE)

.PHONY: flash_openocd
flash_openocd: $(BINFILE)
	$(OPENOCD) \
		-c "init" -c "halt" -c "sleep 1000" \
		-c "flash write_image erase $(BINFILE) 0x08000000" \
		-c "sleep 200" -c "reset run" -c "shutdown"
