project(
	debugger
	LANGUAGES CXX
)

set(
	M_this_special_file
)
if(M_system_windows AND M_compiler_msvc)
	set(
		M_this_special_file
		${M_root_project_directory}/../Shell/shell/windows/application.manifest
	)
endif()

add_executable(
	${PROJECT_NAME}
	./main.cpp
	${M_this_special_file}
)

include(${M_root_project_directory}/cmake/condition_definition.cmake)
include(${M_root_project_directory}/cmake/compile_option.cmake)
include(${M_root_project_directory}/cmake/compile_warning.cmake)
include(${M_root_project_directory}/cmake/import_root.cmake)
include(${M_root_project_directory}/cmake/import_vld.cmake)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${M_root_project_directory}/../Shell
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	core
)

target_compile_definitions(
	${PROJECT_NAME} PRIVATE
	_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
)

if(M_system_windows)
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND cmd /C mklink "\"${CMAKE_CURRENT_BINARY_DIR}/core.dll\"" "\"${CMAKE_CURRENT_BINARY_DIR}/../core/core.dll\"" | exit
	)
endif()
if(M_system_linux)
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ln -s "\"${CMAKE_CURRENT_BINARY_DIR}/../core/libcore.so\"" "\"${CMAKE_CURRENT_BINARY_DIR}/libcore.so\""
	)
endif()
if(M_system_macos)
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ln -s "\"${CMAKE_CURRENT_BINARY_DIR}/../core/libcore.dylib\"" "\"${CMAKE_CURRENT_BINARY_DIR}/libcore.dylib\""
	)
endif()

if(M_system_linux)
	set_target_properties(
		${PROJECT_NAME} PROPERTIES
		COMPILE_FLAGS "-pthread"
		LINK_FLAGS "-pthread"
	)
endif()
