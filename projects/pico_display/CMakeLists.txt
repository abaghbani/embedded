cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH "/home/akbar/sdk/pico-sdk")
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

set(PROJECT_NAME "display")
project(${PROJECT_NAME} C CXX ASM)
set(CXX arm-none-eabi-g++)
set(CC arm-none-eabi-gcc)
# set(PICO_NO_PICOTOOL 1)

add_executable(${PROJECT_NAME} 
	# ../../lib/st7789/st7789.c
	main.c
)

# target_include_directories(${PROJECT_NAME} PRIVATE
	# ../../lib/st7789
# )

add_library(pico_st7789 INTERFACE)
target_sources(pico_st7789 INTERFACE ../../lib/st7789/st7789.c)
target_include_directories(pico_st7789 INTERFACE ../../lib/st7789)

add_library(pico_itap INTERFACE)
target_sources(pico_itap INTERFACE ../../lib/itap/itap.c)
target_include_directories(pico_itap INTERFACE ../../lib/itap)

pico_sdk_init()

target_link_libraries(${PROJECT_NAME}
	pico_stdlib
	hardware_spi
	hardware_pwm
	hardware_adc
	pico_st7789
	pico_itap
)


# Enable USB/UF2/etc.
pico_add_extra_outputs(${PROJECT_NAME})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)




