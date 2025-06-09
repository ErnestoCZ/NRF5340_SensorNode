cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

set(C_GPIO_LIB "C_GPIO_LIB")
set(C_BLUETOOTH_LIB "C_BLUETOOTH_LIB")
project(NRF5340_SensorNode)
add_subdirectory("src")

target_link_libraries(app PUBLIC ${C_GPIO_LIB} ${C_BLUETOOTH_LIB})
target_sources(app PRIVATE src/main.c)
# target_sources_ifdef(CONFIG_GPIO_HELPER app PRIVATE ${C_GPIO_LIB})
