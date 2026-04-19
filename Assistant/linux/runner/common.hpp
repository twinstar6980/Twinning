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

#include <cstdint>
#include <memory>
#include <string>
#include <format>
#include <optional>

// ----------------

inline constexpr auto hash_string(
	std::string_view const & string
) -> std::uint64_t {
	auto offset = std::uint64_t{14695981039346656037ull};
	auto prime = std::uint64_t{1099511628211ull};
	auto result = offset;
	for (auto & element : string) {
		result ^= static_cast<std::uint8_t>(element);
		result *= prime;
	}
	return result;
}

#pragma endregion
