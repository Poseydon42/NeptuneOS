cmake_minimum_required(VERSION 3.22)

project(Tests CXX)

set(SOURCES
	main.cpp
	Common/Containers/TestVector.cpp
	Common/Templates/TestTypedTransfer.cpp
	Common/Templates/TestTypeTraits.cpp
	Common/Utils/TestMemory.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

add_subdirectory(googletest)

add_executable(Tests ${SOURCES})
target_link_libraries(Tests PRIVATE gtest)
add_test(NAME Tests COMMAND Tests)

add_custom_target(tests
	COMMAND Tests
)

# List all libraries that are being tested
target_link_libraries(Tests PRIVATE Common)

