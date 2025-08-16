module;

#include "shell/common.hpp"

export module twinning.shell.utility.string;

export namespace Twinning::Shell {

	#pragma region hash

	inline constexpr auto hash_string (
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

}
