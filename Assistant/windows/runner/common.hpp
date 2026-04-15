#pragma once

#pragma region keyword

#define implicit\
	

#define abstract\
	0

#define thiz\
	(*this)

#pragma endregion

#pragma region assertion

#define assert_test(...)\
	if (!(__VA_ARGS__)) {\
		throw std::runtime_error{"AssertionException: " #__VA_ARGS__};\
	}\
	static_assert(true)

#pragma endregion

#pragma region import

#include <memory>
#include <string>

#pragma endregion
