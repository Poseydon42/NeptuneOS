cmake_minimum_required(VERSION 3.22)

project(Common C CXX)

set(SOURCES
	Utils/Memory.cpp
)

include(common_compiler_options)
include(common_linker_options)

add_library(Common STATIC ${SOURCES})
target_include_directories(Common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

