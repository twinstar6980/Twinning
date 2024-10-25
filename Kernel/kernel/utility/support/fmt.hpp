#pragma once

#include "kernel/utility/string/string.hpp"
#include "kernel/utility/base_wrapper/base.hpp"
#include "kernel/utility/file_system/path.hpp"

#pragma region formatter

template <>
struct fmt::formatter<Twinning::Kernel::CStringView> :
	fmt::formatter<std::string_view> {
	template <typename Context> requires
		Twinning::Kernel::NoneConstraint
	auto format (
		Twinning::Kernel::CStringView const & value,
		Context &                             context
	) const -> typename Context::iterator {
		return fmt::formatter<std::string_view>::format(Twinning::Kernel::make_std_string_view(value), context);
	}
};

template <>
struct fmt::formatter<Twinning::Kernel::VStringView> :
	fmt::formatter<std::string_view> {
	template <typename Context> requires
		Twinning::Kernel::NoneConstraint
	auto format (
		Twinning::Kernel::VStringView const & value,
		Context &                             context
	) const -> typename Context::iterator {
		return fmt::formatter<std::string_view>::format(Twinning::Kernel::make_std_string_view(value), context);
	}
};

template <>
struct fmt::formatter<Twinning::Kernel::String> :
	fmt::formatter<std::string_view> {
	template <typename Context> requires
		Twinning::Kernel::NoneConstraint
	auto format (
		Twinning::Kernel::String const & value,
		Context &                        context
	) const -> typename Context::iterator {
		return fmt::formatter<std::string_view>::format(Twinning::Kernel::make_std_string_view(value), context);
	}
};

// ----------------

template <>
struct fmt::formatter<Twinning::Kernel::Path> :
	fmt::formatter<Twinning::Kernel::String> {
	template <typename Context> requires
		Twinning::Kernel::NoneConstraint
	auto format (
		Twinning::Kernel::Path const & value,
		Context &                      context
	) const -> typename Context::iterator {
		return fmt::formatter<Twinning::Kernel::String>::format(value.to_string(), context);
	}
};

// ----------------

// TODO : specialization format for floater wrapper ?
template <typename TType> requires
	Twinning::Kernel::AutoConstraint
	&& (Twinning::Kernel::IsBaseWrapper<TType>)
	&& (Twinning::Kernel::IsDerivedFrom<TType, Twinning::Kernel::BaseWrapper<typename TType::Value>>) // NOTE : avoid clang bug
struct fmt::formatter<TType> :
	fmt::formatter<typename TType::Value> {
	template <typename Context> requires
		Twinning::Kernel::NoneConstraint
	auto format (
		TType const & value,
		Context &     context
	) const -> typename Context::iterator {
		return fmt::formatter<typename TType::Value>::format(value.value, context);
	}
};

#pragma endregion
