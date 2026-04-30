cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_PROJECT_NAME ece5780massagegun)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(toolchain-arm-none-eabi)

project(${CMAKE_PROJECT_NAME} C ASM)
enable_language(C ASM)

set(CMSIS_CORE_INC "${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include")
set(CMSIS_DEVICE_INC "${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F0xx/Include")
set(SYSTEM_SRC "${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/system_stm32f0xx.c")
set(STARTUP_SRC "${CMAKE_SOURCE_DIR}/startup_stm32f072xb.s")

foreach(req
        "${CMSIS_CORE_INC}/core_cm0.h"
        "${CMSIS_DEVICE_INC}/stm32f0xx.h"
        "${SYSTEM_SRC}"
        "${STARTUP_SRC}"
        "${CMAKE_SOURCE_DIR}/STM32F072XX_FLASH.ld")
    if(NOT EXISTS "${req}")
        message(FATAL_ERROR "Required file missing: ${req}")
    endif()
endforeach()

add_executable(${CMAKE_PROJECT_NAME}
    Src/main.c
    Src/app_modes.c
    Src/mode_leds.c
    Src/motor_pwm.c
    Src/pot_adc.c
    ${SYSTEM_SRC}
    ${STARTUP_SRC}
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    STM32F072xB
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    Inc
    ${CMSIS_CORE_INC}
    ${CMSIS_DEVICE_INC}
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
    COMMENT "Generating HEX and BIN files")
