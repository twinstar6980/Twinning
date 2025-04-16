#pragma once

#pragma region version

#define M_version "49"

#pragma endregion

#pragma region keyword

#define implicit\
	

#define thiz\
	(*this)

#pragma endregion

#pragma region assertion

#define assert_test(...)\
	if (!(__VA_ARGS__)) {\
		throw std::runtime_error{"assertion failed : " #__VA_ARGS__};\
	}\
	static_assert(true)

#pragma endregion

#pragma region miscellaneous

#if defined M_system_windows
#define M_declare_native_main_function int wmain (int argc, wchar_t * argv[])
#endif
#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
#define M_declare_native_main_function int  main (int argc, char    * argv[])
#endif

#pragma endregion

#pragma region import

import twinning.common.std;

#pragma endregion
