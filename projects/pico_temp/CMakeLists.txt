cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH "/home/akbar/sdk/pico-sdk")
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Name of the project
project(blinky C CXX ASM)
set(CXX arm-none-eabi-g++)
set(CC arm-none-eabi-gcc)
# set(PICO_NO_PICOTOOL 1)

# Add your executable
add_executable(blinky main.c)

# Initialize the SDK (required)
pico_sdk_init()

# Link standard Pico libraries
target_link_libraries(blinky pico_stdlib)

# Enable USB/UF2/etc.
pico_add_extra_outputs(blinky)
pico_enable_stdio_usb(blinky 1)
pico_enable_stdio_uart(blinky 0)