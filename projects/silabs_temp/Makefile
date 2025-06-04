TARGETS          := output
OUTPUT_DIRECTORY := _build

SDK_ROOT := /home/akbar/sdk/gecko_sdk

SRC_FILES := $(SDK_ROOT)/platform/Device/SiliconLabs/EFM32LG/Source/GCC/startup_efm32lg.S
SRC_FILES += $(SDK_ROOT)/platform/Device/SiliconLabs/EFM32LG/Source/startup_efm32lg.c
SRC_FILES += $(SDK_ROOT)/platform/Device/SiliconLabs/EFM32LG/Source/system_efm32lg.c
SRC_FILES += $(SDK_ROOT)/platform/emlib/src/em_system.c
SRC_FILES += $(SDK_ROOT)/platform/emlib/src/em_cmu.c
SRC_FILES += $(SDK_ROOT)/platform/emlib/src/em_core.c
SRC_FILES += $(SDK_ROOT)/platform/emlib/src/em_gpio.c
SRC_FILES += ../mystartup.c
SRC_FILES += ../main.c

INC_FOLDERS := $(SDK_ROOT)/platform/Device/SiliconLabs/EFM32LG/Include
INC_FOLDERS += $(SDK_ROOT)/platform/CMSIS/Core/Include
INC_FOLDERS += $(SDK_ROOT)/platform/emlib/inc
INC_FOLDERS += $(SDK_ROOT)/platform/common/inc
INC_FOLDERS += ../

FLAGS := -O3 -g3 
FLAGS += -mcpu=cortex-m3
FLAGS += -mthumb -mabi=aapcs
FLAGS += -Wall -Werror
# FLAGS += -DEFM32LG330F256
FLAGS += -DPART_EFM32LG330F256 -DEFM32LG330F256

CFLAGS := $(FLAGS)
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums
CFLAGS += $(foreach dir, $(INC_FOLDERS), -I$(dir))

LDFLAGS := $(FLAGS) 
LDFLAGS += -L$(SDK_ROOT)/platform/Device/SiliconLabs/EFM32LG/Source/GCC -Tefm32lg.ld
LDFLAGS += -Wl,--gc-sections
# LDFLAGS += --specs=nano.specs
# LDFLAGS += -nostartfiles -Wl,--gc-sections


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
	@$(OBJCOPY) -O binary $@ $(TARGETS).bin
	@$(SIZE) $@

$(OUTPUT_DIRECTORY)/%.o: %.c
	@echo 'Compiling C file: $<'
	@$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT_DIRECTORY)/%.o: %.S
	@echo 'Assembling file: $<'
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Removing output folder ..."
	@rm -rf $(OUTPUT_DIRECTORY) $(TARGETS).*
