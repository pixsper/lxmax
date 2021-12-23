cmake_minimum_required(VERSION 3.19)

SET(LXMAX_COMMON_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(liblxmax)
add_subdirectory(lxmax)
add_subdirectory(lx.config)
add_subdirectory(lx.config.ui)
add_subdirectory(lx.dmx.read)
add_subdirectory(lx.dmx.write)