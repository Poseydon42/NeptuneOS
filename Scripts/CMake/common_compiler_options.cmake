set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Werror)

add_compile_options(-fno-exceptions -fno-rtti -mcmodel=large -Wno-unknown-pragmas)

if(CMAKE_CROSSCOMPILING)
	add_compile_options(-ffreestanding)
endif()

