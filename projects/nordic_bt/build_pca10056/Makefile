TARGETS          := output
OUTPUT_DIRECTORY := _build

SDK_ROOT := /home/akbar/sdk/nRF5_SDK_17.1

LINKER_SCRIPT := direct_test_mode_gcc_nrf52.ld

SRC_FILES := $(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf52840.S
SRC_FILES += $(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c
SRC_FILES += $(SDK_ROOT)/components/libraries/log/src/nrf_log_str_formatter.c
SRC_FILES += $(SDK_ROOT)/components/libraries/util/app_error.c
SRC_FILES += $(SDK_ROOT)/components/libraries/util/app_error_handler_gcc.c
SRC_FILES += $(SDK_ROOT)/components/libraries/util/app_error_weak.c
SRC_FILES += $(SDK_ROOT)/components/libraries/fifo/app_fifo.c
SRC_FILES += $(SDK_ROOT)/components/libraries/util/app_util_platform.c
SRC_FILES += $(SDK_ROOT)/components/libraries/util/nrf_assert.c
SRC_FILES += $(SDK_ROOT)/components/libraries/uart/app_uart_fifo.c
SRC_FILES += $(SDK_ROOT)/components/libraries/uart/retarget.c
SRC_FILES += $(SDK_ROOT)/components/libraries/atomic/nrf_atomic.c
SRC_FILES += $(SDK_ROOT)/components/libraries/balloc/nrf_balloc.c
SRC_FILES += $(SDK_ROOT)/components/libraries/memobj/nrf_memobj.c
SRC_FILES += $(SDK_ROOT)/components/libraries/ringbuf/nrf_ringbuf.c
SRC_FILES += $(SDK_ROOT)/components/libraries/strerror/nrf_strerror.c
SRC_FILES += $(SDK_ROOT)/components/boards/boards.c
SRC_FILES += $(SDK_ROOT)/external/fprintf/nrf_fprintf.c
SRC_FILES += $(SDK_ROOT)/external/fprintf/nrf_fprintf_format.c
SRC_FILES += $(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_uart.c
SRC_FILES += $(SDK_ROOT)/modules/nrfx/drivers/src/prs/nrfx_prs.c
SRC_FILES += $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_uart.c
SRC_FILES += $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_uarte.c
SRC_FILES += $(SDK_ROOT)/modules/nrfx/mdk/system_nrf52840.c
SRC_FILES += ../ble_dtm/ble_dtm_hw_nrf52.c
SRC_FILES += ../ble_dtm/ble_dtm.c
SRC_FILES += ../syscalls.c
SRC_FILES += ../main.c

INC_FOLDERS := $(SDK_ROOT)/components
INC_FOLDERS += $(SDK_ROOT)/components/libraries/fifo
INC_FOLDERS += $(SDK_ROOT)/components/libraries/strerror
INC_FOLDERS += $(SDK_ROOT)/components/libraries/balloc
INC_FOLDERS += $(SDK_ROOT)/components/libraries/ringbuf
INC_FOLDERS += $(SDK_ROOT)/components/libraries/bsp
INC_FOLDERS += $(SDK_ROOT)/components/libraries/uart
INC_FOLDERS += $(SDK_ROOT)/components/libraries/log
INC_FOLDERS += $(SDK_ROOT)/components/libraries/experimental_section_vars
INC_FOLDERS += $(SDK_ROOT)/components/libraries/atomic
INC_FOLDERS += $(SDK_ROOT)/components/libraries/util
INC_FOLDERS += $(SDK_ROOT)/components/libraries/memobj
INC_FOLDERS += $(SDK_ROOT)/components/libraries/log/src
INC_FOLDERS += $(SDK_ROOT)/components/libraries/delay
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/nrf_soc_nosd
# INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_dtm
INC_FOLDERS += $(SDK_ROOT)/components/toolchain/cmsis/include
INC_FOLDERS += $(SDK_ROOT)/components/boards
INC_FOLDERS += $(SDK_ROOT)/modules/nrfx/mdk
INC_FOLDERS += $(SDK_ROOT)/modules/nrfx/hal
INC_FOLDERS += $(SDK_ROOT)/modules/nrfx
INC_FOLDERS += $(SDK_ROOT)/modules/nrfx/drivers/include
INC_FOLDERS += $(SDK_ROOT)/integration/nrfx/legacy
INC_FOLDERS += $(SDK_ROOT)/integration/nrfx
INC_FOLDERS += $(SDK_ROOT)/external/fprintf
INC_FOLDERS += ../ble_dtm
INC_FOLDERS += ../
INC_FOLDERS += .

CFLAGS += -O3 -g3 
CFLAGS += -DBOARD_PCA10056
CFLAGS += -DBSP_DEFINES_ONLY
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -DNRF52840_XXAA
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall -Werror
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums
CFLAGS += $(foreach dir, $(INC_FOLDERS), -I$(dir))
CFLAGS += -D__HEAP_SIZE=8192
CFLAGS += -D__STACK_SIZE=8192

ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += -DBOARD_PCA10056
ASMFLAGS += -DBSP_DEFINES_ONLY
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -DNRF52840_XXAA
ASMFLAGS += -D__HEAP_SIZE=8192
ASMFLAGS += -D__STACK_SIZE=8192

LDFLAGS += -O3 -g3 
LDFLAGS += -mthumb -mabi=aapcs -L$(SDK_ROOT)/modules/nrfx/mdk -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAGS += -Wl,--gc-sections
LDFLAGS += --specs=nano.specs

#SRC_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
#OBJ_FILES := $(addprefix $(OUTPUT_DIRECTORY)/, $(notdir $(SRC_FILES:.c=.o)))
#vpath %.c $(SRC_DIRS)

C_SRC_FILES := $(filter %.c, $(SRC_FILES))
S_SRC_FILES := $(filter %.S, $(SRC_FILES))

OBJ_C_FILES := $(addprefix $(OUTPUT_DIRECTORY)/, $(notdir $(C_SRC_FILES:.c=.o)))
OBJ_S_FILES := $(addprefix $(OUTPUT_DIRECTORY)/, $(notdir $(S_SRC_FILES:.S=.o)))

OBJ_FILES := $(OBJ_S_FILES) $(OBJ_C_FILES)
vpath %.c $(sort $(dir $(C_SRC_FILES)))
vpath %.S $(sort $(dir $(S_SRC_FILES)))

CC      := arm-none-eabi-gcc
CXX     := arm-none-eabi-c++
AS      := arm-none-eabi-as
AR      := arm-none-eabi-ar -r
LD      := arm-none-eabi-ld
NM      := arm-none-eabi-nm
OBJDUMP := arm-none-eabi-objdump
OBJCOPY := arm-none-eabi-objcopy
SIZE    := arm-none-eabi-size


all: $(OUTPUT_DIRECTORY) $(TARGETS).elf

$(OUTPUT_DIRECTORY):
	@if [ ! -d "$@" ]; then mkdir -p "$@"; fi

$(TARGETS).elf: $(OBJ_FILES)
	@echo Building target: $@
	@echo Invoking: ARM v7 Linux gcc linker
	@$(CC) -o $@ $^ $(LDFLAGS)
	@$(OBJCOPY) -O ihex $@ $(TARGETS).hex
	@echo Finished building target: $@
	@$(SIZE) $@

$(OUTPUT_DIRECTORY)/%.o: %.c
	@echo 'Compiling C file: $<'
	@$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT_DIRECTORY)/%.o: %.S
	@echo 'Assembling file: $<'
	@$(CC) $(ASMFLAGS) -c $< -o $@

clean:
	@echo "Removing output folder ..."
	@rm -rf $(OUTPUT_DIRECTORY) $(TARGETS)
