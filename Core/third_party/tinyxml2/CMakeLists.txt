project(
	tinyxml2
	VERSION 9.0.0
	HOMEPAGE_URL https://github.com/leethomason/tinyxml2
	LANGUAGES CXX
)

add_library(
	${PROJECT_NAME} STATIC
	./tinyxml2.h
	./tinyxml2.cpp
)

include(${M_root_project_directory}/cmake/compile_warning_disable.cmake)
