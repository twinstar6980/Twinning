#pragma once

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

#pragma region import

import std;

#pragma endregion
