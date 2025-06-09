

set(LIB_SRC gatt.c)
add_library(${C_BLUETOOTH_LIB} STATIC ${LIB_SRC})
target_link_libraries(${C_BLUETOOTH_LIB} PUBLIC zephyr_interface)
target_include_directories(${C_BLUETOOTH_LIB} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})