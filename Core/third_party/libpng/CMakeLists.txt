project(
	libpng
	VERSION 1.6.37
	HOMEPAGE_URL http://www.libpng.org/pub/png/libpng.html
	LANGUAGES C
)

add_library(
	${PROJECT_NAME} STATIC
	./png.h
	./pngconf.h
	./pnglibconf.h
	./pngpriv.h
	./pngdebug.h
	./pnginfo.h
	./pngstruct.h
	./png.c
	./pngerror.c
	./pngget.c
	./pngmem.c
	./pngpread.c
	./pngread.c
	./pngrio.c
	./pngrtran.c
	./pngrutil.c
	./pngset.c
	./pngtrans.c
	./pngwio.c
	./pngwrite.c
	./pngwtran.c
	./pngwutil.c
)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	${M_root_project_directory}/third_party/zlib
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	zlib
)

include(${M_root_project_directory}/cmake/compile_warning_disable.cmake)
