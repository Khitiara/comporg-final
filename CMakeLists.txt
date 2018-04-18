cmake_minimum_required(VERSION 3.9)
project(comporg-final C)
set(CMAKE_C_STANDARD 99)


add_subdirectory(impl1)
add_subdirectory(impl2)
add_subdirectory(impl3)

add_custom_target(comporg-final ALL DEPENDS impl1 impl2 impl3)
