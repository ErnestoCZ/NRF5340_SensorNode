set(LIB_SRC gpio_helper.c)
add_library(${C_GPIO_LIB} STATIC ${LIB_SRC})
target_link_libraries(${C_GPIO_LIB} PUBLIC zephyr_interface)
target_include_directories(${C_GPIO_LIB} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})