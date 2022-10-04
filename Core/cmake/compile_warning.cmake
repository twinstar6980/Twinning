# compile warning

# basic
if(M_compiler_msvc)
	target_compile_options(
		${PROJECT_NAME} PRIVATE
		/Wall
		/wd4820 # like -Wpadded in clang, enable this warning will make compiler crashed
		/wd4100 # unused parameter
		/wd4189 # unused local variable
		/wd4458 # shadow the member
		/wd4514 # unused inline function is deleted
		/wd4710 # inline function is not inlined
		/wd4711 # non-inline function is inlined
		/wd4668 # symbol not be defined to preprocessor macro
		/wd4061 # switch case of enum not explicitly given
		/wd4927 # TODO
	)
endif()
if(M_compiler_clang)
	target_compile_options(
		${PROJECT_NAME} PRIVATE
		-Weverything
		-Wno-c++98-compat
		-Wno-c++98-compat-pedantic
		-Wno-pre-c++14-compat
		-Wno-pre-c++17-compat
		-Wno-c++20-compat
		-Wno-c99-extensions
		-Wno-padded
		-Wno-redundant-parens
		-Wno-missing-field-initializers
		-Wno-unused-parameter
		-Wno-unused-variable
		-Wno-missing-noreturn
		-Wno-gnu-zero-variadic-macro-arguments
		-Wno-ctad-maybe-unsupported
		-Wno-global-constructors
		-Wno-exit-time-destructors
		-Wno-weak-vtables
		-Wno-self-assign
		-Wno-switch-enum
		-Wno-shadow-field
		-Wno-shadow-field-in-constructor
		-Wno-shadow-uncaptured-local
		-Wno-float-equal # TODO
	)
endif()
