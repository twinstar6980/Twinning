# import vld

# basic
if(M_system_windows AND M_compiler_msvc AND M_build_debug)
	target_compile_definitions(
		${PROJECT_NAME} PRIVATE
		M_vld
		VLD_FORCE_ENABLE
	)
	target_include_directories(
		${PROJECT_NAME} PRIVATE
		"C:/Program Files (x86)/Visual Leak Detector/include"
	)
	target_link_directories(
		${PROJECT_NAME} PRIVATE
		"C:/Program Files (x86)/Visual Leak Detector/lib/Win64"
	)
	file(
		COPY
		"C:/Program Files (x86)/Visual Leak Detector/bin/Win64/"
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
	)
endif()
