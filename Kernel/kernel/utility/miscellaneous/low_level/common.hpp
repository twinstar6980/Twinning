#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-macros"

#pragma region assertion

#undef assert_test
#define assert_test(...)\
	if (!(__VA_ARGS__)) {\
		throw std::runtime_error{"AssertionException: " #__VA_ARGS__};\
	}\
	static_assert(true)

#pragma endregion

#pragma clang diagnostic pop
