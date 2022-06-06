cmake_minimum_required(VERSION 3.22)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/Scripts/CMake)

if(CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
endif()

project(NeptuneOS C CXX ASM_NASM)

set(NEPTUNE_ARCH "amd64")

if (CMAKE_CROSSCOMPILING)
	add_custom_target(run
		COMMAND "${CMAKE_COMMAND}" -E env "${NeptuneOS_SOURCE_DIR}/Scripts/run.sh"
		DEPENDS image
		USES_TERMINAL
	)

	add_custom_target(run-debug
		COMMAND "${CMAKE_COMMAND}" -E env "${NeptuneOS_SOURCE_DIR}/Scripts/run-debug.sh"
		DEPENDS image
		USES_TERMINAL
	)
	
	add_custom_target(image
		COMMAND "${CMAKE_COMMAND}" -E env "${NeptuneOS_SOURCE_DIR}/Scripts/image.sh"
		USES_TERMINAL
	)
endif()

add_subdirectory(Common)

if (NOT CMAKE_CROSSCOMPILING)
	add_subdirectory(Tests)
else()
	add_subdirectory(Kernel)
endif()

