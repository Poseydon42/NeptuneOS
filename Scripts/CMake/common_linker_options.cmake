if (CMAKE_CROSSCOMPILING)
	add_link_options(-ffreestanding -nostdlib -lgcc -z max-page-size=0x1000 -mno-red-zone)
endif()

