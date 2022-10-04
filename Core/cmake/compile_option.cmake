# compile option

# basic
if(M_compiler_msvc)
	target_compile_options(
		${PROJECT_NAME} PRIVATE
		# use utf-8 text encoding
		/utf-8
		# enable big object
		/bigobj
		# disable permissive
		/permissive-
		# enable conforming preprocessor
		/Zc:preprocessor
	)
	target_compile_definitions(
		${PROJECT_NAME} PRIVATE
		# disable msvc secure error
		_CRT_SECURE_NO_WARNINGS
	)
endif()
