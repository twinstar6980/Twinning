#pragma once

#include "kernel/utility/string/string.hpp"

namespace std {

	#pragma region hash

	template <>
	struct hash<TwinStar::Kernel::CStringView> {
		auto operator () (
			TwinStar::Kernel::CStringView const & it
		) const noexcept -> std::size_t {
			return std::hash<std::string_view>{}(TwinStar::Kernel::make_std_string_view(it));
		}
	};

	template <>
	struct hash<TwinStar::Kernel::VStringView> {
		auto operator () (
			TwinStar::Kernel::VStringView const & it
		) const noexcept -> std::size_t {
			return std::hash<std::string_view>{}(TwinStar::Kernel::make_std_string_view(it));
		}
	};

	template <>
	struct hash<TwinStar::Kernel::String> {
		auto operator () (
			TwinStar::Kernel::String const & it
		) const noexcept -> std::size_t {
			return std::hash<std::string_view>{}(TwinStar::Kernel::make_std_string_view(it));
		}
	};

	#pragma endregion

}
