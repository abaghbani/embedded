TARGET = stm32f411_project
BUILD_DIR = _build
LINKER_SCRIPT  := STM32F411RETX_FLASH.ld
STARTUP_FILE := ../startup_stm32f411retx.s

TOOLCHAIN := arm-none-eabi
SDK_ROOT := /home/akbar/sdk/STM32CubeF4

SRC_DIRS := ..
SRC_DIRS += $(SDK_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src
INC_DIRS := ..
INC_DIRS += $(SDK_ROOT)/Drivers/STM32F4xx_HAL_Driver/Inc
INC_DIRS += $(SDK_ROOT)/Drivers/CMSIS/Device/ST/STM32F4xx/Include
INC_DIRS += $(SDK_ROOT)/Drivers/CMSIS/Include


SRC_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJ_FILES := $(addprefix $(BUILD_DIR)/, $(notdir $(SRC_FILES:.c=.o)))
# OBJ_FILES += $(BUILD_DIR)/$(notdir $(STARTUP_FILE:.S=.o))
OBJ_FILES += _build/startup_stm32f411retx.o

vpath %.c $(SRC_DIRS)


CC = $(TOOLCHAIN)-gcc
AS = $(TOOLCHAIN)-as
LD = $(TOOLCHAIN)-ld
OBJCOPY = $(TOOLCHAIN)-objcopy
OBJDUMP = $(TOOLCHAIN)-objdump
SIZE = $(TOOLCHAIN)-size


MCU := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS := $(MCU) -c -DSTM32F411xE -Wall -O2 -std=gnu11
CFLAGS += $(foreach dir, $(INC_DIRS), -I$(dir)/)
CFLAGS += -DHSE_VALUE=25000000U


all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).elf


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET).elf: $(OBJ_FILES)
	@echo Building target: $@
	@echo Invoking: ARM v7 Linux gcc linker
	@$(CC) $(MCU) -T $(LINKER_SCRIPT) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--gc-sections -o $@ $^
	@echo Finished building target: $@
	@$(OBJCOPY) -O binary $@ $(BUILD_DIR)/$(TARGET).bin
	@$(SIZE) $@

$(BUILD_DIR)/%.o: %.c
	@echo 'Compiling file: $<'
	@$(CC) $(CFLAGS) -o $@ $<
	
$(BUILD_DIR)/startup_stm32f411retx.o: ../startup_stm32f411retx.s
	@echo 'Compiling assembly file: $<'
	@$(CC) $(CFLAGS) -o $@ $<

clean:
	@echo "Removing output folder ..."
	@rm -rf $(BUILD_DIR)
