cmake_minimum_required(VERSION 3.22)

project(Kernel CXX ASM_NASM)

set(CMAKE_ASM_NASM_COMPILER /usr/bin/nasm)
set(CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS asm)
set(CMAKE_ASM_NASM_FLAGS "-F dwarf -g -f elf64 -w-number-overflow")

set(CMAKE_ASM_NASM_COMPILE_OBJECT
	"<CMAKE_ASM_NASM_COMPILER> ${CMAKE_ASM_NASM_FLAGS} -o <OBJECT> <SOURCE>"
)

include(common_compiler_options)
include(common_linker_options)
add_link_options(-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)
add_compile_definitions(NEPTUNE_KERNEL)

if(NEPTUNE_ARCH STREQUAL "amd64")
	add_compile_definitions(NEPTUNE_AMD64)
endif()

set(ASM_SOURCES
	Arch/amd64/CPU.asm
	Boot/amd64/boot.asm
)

set(CXX_SOURCES
	Arch/amd64/CPU.cpp
	Arch/amd64/IDT.cpp
	Boot/Main.cpp
	CompilerStubs.cpp
	IO/Serial.cpp
	IO/Console.cpp
	Memory/KMalloc.cpp
	Memory/PhysicalMemoryManager.cpp
	Panic.cpp
)

add_executable(Kernel.bin ${ASM_SOURCES} ${CXX_SOURCES})

target_link_libraries(Kernel.bin PRIVATE Common)
target_include_directories(Kernel.bin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

