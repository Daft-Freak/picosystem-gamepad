cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(picosystem-gamepad C CXX)

# Initialise the Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(picosystem-gamepad
    gamepad.cpp
    usb_descriptors.c
)

pico_set_program_name(picosystem-gamepad "picosystem-gamepad")
pico_set_program_version(picosystem-gamepad "0.1")

pico_enable_stdio_uart(picosystem-gamepad 1)
pico_enable_stdio_usb(picosystem-gamepad 0)

target_link_libraries(picosystem-gamepad pico_stdlib tinyusb_device)
target_include_directories(picosystem-gamepad PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

pico_add_extra_outputs(picosystem-gamepad)

