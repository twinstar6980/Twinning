project(
	bzip2
	VERSION 1.0.8
	HOMEPAGE_URL https://sourceware.org/bzip2/
	LANGUAGES C
)

add_library(
	${PROJECT_NAME} STATIC
	./bzlib.h
	./bzlib_private.h
	./blocksort.c
	./bzip2.c
	./bzlib.c
	./compress.c
	./crctable.c
	./decompress.c
	./huffman.c
	./randtable.c
)

include(${M_root_project_directory}/cmake/compile_warning_disable.cmake)
