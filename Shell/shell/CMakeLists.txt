project(
	shell
	LANGUAGES CXX
)

set(
	M_this_special_file
)
if(M_system_windows AND M_compiler_msvc)
	set(
		M_this_special_file
		./windows/application.manifest
		./windows/application.rc
	)
endif()

add_executable(
	${PROJECT_NAME}
	./interface.hpp
	./base.hpp
	./loader.hpp
	./callback.hpp
	./main.cpp
	${M_this_special_file}
)

include(${M_root_project_directory}/cmake/condition_definition.cmake)
include(${M_root_project_directory}/cmake/compile_option.cmake)

target_compile_definitions(
	${PROJECT_NAME} PRIVATE
	_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
)

if(M_system_linux)
	target_link_options(
		${PROJECT_NAME} PRIVATE
		-ldl
	)
endif()
