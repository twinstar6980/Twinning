project(
	Rijndael
	VERSION 0 # none
	HOMEPAGE_URL _ # none
	LANGUAGES CXX
)

add_library(
	${PROJECT_NAME} STATIC
	./Rijndael.h
	./Rijndael.cpp
)
