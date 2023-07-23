#pragma once

#include <string_view>
#include <string>

#if !defined M_system_android
#include <source_location>
#define M_current_source_location std::source_location::current()
#else
namespace std {
	struct source_location {
		constexpr source_location (
		) noexcept = default;
		static consteval auto current (
		) -> source_location {
			return source_location{};
		}
		constexpr auto line (
		) const noexcept -> uint_least32_t {
			return 0;
		}
		constexpr auto column (
		) const noexcept -> uint_least32_t {
			return 0;
		}
		constexpr auto file_name (
		) const noexcept -> char const* {
			return "";
		}
		constexpr auto function_name (
		) const noexcept -> char const* {
			return "";
		}
	};
}
#define M_current_source_location std::source_location{}
// TODO : clang bug : https://github.com/llvm/llvm-project/issues/48230 (fixed in 15)
// TODO : clang bug : https://github.com/llvm/llvm-project/issues/56379 (fixed in 16)
#endif

namespace TwinStar::Kernel {

	#pragma region function

	inline auto parse_source_location_file_path (
		std::source_location const & location
	) -> std::string {
		auto string = std::string{location.file_name() + (std::string_view{std::source_location::current().file_name()}.size() - std::string_view{"kernel/utility/exception/source_location.hpp"}.size())};
		for (auto & character : string) {
			if (character == '\\') {
				character = '/';
			}
		}
		return string;
	}

	#pragma endregion

}
