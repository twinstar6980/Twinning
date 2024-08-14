#pragma once

#include "kernel/utility/string/string.hpp"

#pragma region hash

template <>
struct std::hash<Twinning::Kernel::CStringView> {
	auto operator () (
		Twinning::Kernel::CStringView const & it
	) const noexcept -> std::size_t {
		return std::hash<std::string_view>{}(Twinning::Kernel::make_std_string_view(it));
	}
};

template <>
struct std::hash<Twinning::Kernel::VStringView> {
	auto operator () (
		Twinning::Kernel::VStringView const & it
	) const noexcept -> std::size_t {
		return std::hash<std::string_view>{}(Twinning::Kernel::make_std_string_view(it));
	}
};

template <>
struct std::hash<Twinning::Kernel::String> {
	auto operator () (
		Twinning::Kernel::String const & it
	) const noexcept -> std::size_t {
		return std::hash<std::string_view>{}(Twinning::Kernel::make_std_string_view(it));
	}
};

#pragma endregion
