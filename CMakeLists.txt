cmake_minimum_required(VERSION 3.13)

set(BOARD pico_sdk)
include(pico_sdk_import.cmake)

project(pico_embedded_cli C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

set(target_name pico_embedded_cli)
add_executable(${target_name}
    main.c
    ${CMAKE_CURRENT_LIST_DIR}/embedded-cli/lib/src/embedded_cli.c
)

pico_enable_stdio_uart(${target_name} 1)

target_link_options(${target_name} PRIVATE -Xlinker --print-memory-usage)
target_compile_options(${target_name} PRIVATE -Wall -Wextra)
target_compile_definitions(${target_name} PRIVATE
PICO_DEFAULT_UART_TX_PIN=16
PICO_DEFAULT_UART_RX_PIN=17
)

target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/embedded-cli/lib/include)

target_link_libraries(${target_name} pico_stdlib)

pico_add_extra_outputs(${target_name})

