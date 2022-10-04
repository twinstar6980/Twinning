project(
	md5
	VERSION 0 # none
	HOMEPAGE_URL https://github.com/JieweiWei/md5
	LANGUAGES CXX
)

add_library(
	${PROJECT_NAME} STATIC
	./md5.h
	./md5.cpp
)

include(${M_root_project_directory}/cmake/compile_warning_disable.cmake)
