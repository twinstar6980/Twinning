# compile warning disable

# basic
if(M_compiler_msvc)
	target_compile_options(
		${PROJECT_NAME} PRIVATE
		/w
	)
endif()
if(M_compiler_clang)
	target_compile_options(
		${PROJECT_NAME} PRIVATE
		-w
		-Wno-c++11-narrowing
	)
endif()
