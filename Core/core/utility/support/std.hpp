#pragma once

#include "core/utility/string/string.hpp"

namespace std {

	#pragma region hash

	template <>
	struct hash<TwinStar::Core::CStringView> {
		auto operator () (
			TwinStar::Core::CStringView const & it
		) const noexcept -> std::size_t {
			return std::hash<std::string_view>{}(TwinStar::Core::make_std_string_view(it));
		}
	};

	template <>
	struct hash<TwinStar::Core::VStringView> {
		auto operator () (
			TwinStar::Core::VStringView const & it
		) const noexcept -> std::size_t {
			return std::hash<std::string_view>{}(TwinStar::Core::make_std_string_view(it));
		}
	};

	template <>
	struct hash<TwinStar::Core::String> {
		auto operator () (
			TwinStar::Core::String const & it
		) const noexcept -> std::size_t {
			return std::hash<std::string_view>{}(TwinStar::Core::make_std_string_view(it));
		}
	};

	#pragma endregion

}
