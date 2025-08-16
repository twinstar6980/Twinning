#pragma once

#pragma region version

#define M_version "57"

#pragma endregion

#pragma region keyword

#define implicit\
	

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

import twinning.common.std;

#pragma endregion
