#pragma once

#include "shell/common.hpp"
#include "shell/utility/macro.hpp"

namespace TwinStar::Shell {

	#pragma region function

	inline auto parse_current_exception (
	) -> std::string {
		auto destination = std::string{};
		try {
			std::rethrow_exception(std::current_exception());
		}
		catch (std::string & source) {
			destination = std::string{source};
		}
		catch (std::exception & source) {
			destination = std::string{typeid(source).name()} + std::string{" : "s} + std::string{source.what()};
		}
		catch (...) {
			destination = std::string{"?"s};
		}
		return destination;
	}

	#pragma endregion

}

#define assert_test(...)\
	if (!(__VA_ARGS__)) {\
		throw std::runtime_error{"assertion failed : " #__VA_ARGS__};\
	}\
	static_assert(true)
