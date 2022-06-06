cmake_minimum_required(VERSION 3.22)

project(Tests CXX)

set(SOURCES
	main.cpp
	Common/Templates/TestTypeTraits.cpp
	Common/Utils/TestMemory.cpp
)

add_subdirectory(googletest)

add_executable(Tests ${SOURCES})
target_link_libraries(Tests PRIVATE gtest)
add_test(NAME Tests COMMAND Tests)

add_custom_target(tests
	COMMAND Tests
)

# List all libraries that are being tested
target_link_libraries(Tests PRIVATE Common)

