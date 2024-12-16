module;

#include "shell/common.hpp"

export module twinning.shell.utility.string;

export namespace Twinning::Shell {

	#pragma region encoding

	inline auto utf8_to_utf16 (
		std::u8string_view const & source
	) -> std::u16string {
		auto converter = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{};
		auto result = converter.from_bytes(
			reinterpret_cast<char const *>(source.data()),
			reinterpret_cast<char const *>(source.data() + source.size())
		);
		assert_test(converter.converted() == source.size());
		return result;
	}

	inline auto utf16_to_utf8 (
		std::u16string_view const & source
	) -> std::u8string {
		auto converter = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{};
		auto result = converter.to_bytes(
			source.data(),
			source.data() + source.size()
		);
		assert_test(converter.converted() == source.size());
		return reinterpret_cast<std::u8string &>(result);
	}

	#pragma endregion

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
