#pragma once

#include "kernel/utility/string/string.hpp"
#include "kernel/utility/base_wrapper/base.hpp"
#include "kernel/utility/file_system/path.hpp"

namespace fmt {

	#pragma region string

	template <auto t_constant> requires
		Twinning::Kernel::AutoConstraint
	struct formatter<Twinning::Kernel::StringView<t_constant>> :
		formatter<std::string_view> {
		template <typename Context>
		auto format (
			Twinning::Kernel::StringView<t_constant> const & value,
			Context &                                        context
		) {
			return formatter<std::string_view>::format(Twinning::Kernel::make_std_string_view(value), context);
		}
	};

	template <>
	struct formatter<Twinning::Kernel::String> :
		formatter<std::string_view> {
		template <typename Context>
		auto format (
			Twinning::Kernel::String const & value,
			Context &                        context
		) {
			return formatter<std::string_view>::format(Twinning::Kernel::make_std_string_view(value), context);
		}
	};

	#pragma endregion

	#pragma region base wrapper

	template <typename TType> requires
		Twinning::Kernel::AutoConstraint
		&& (Twinning::Kernel::IsBaseWrapper<TType>)
	#if defined M_compiler_clang // NOTE : avoid clang bug
		&& (Twinning::Kernel::IsDerivedFrom<TType, Twinning::Kernel::BaseWrapper<typename TType::Value>>)
	#endif
	struct formatter<TType> :
		formatter<typename TType::Value> {
		template <typename Context>
		auto format (
			TType const & value,
			Context &     context
		) {
			return formatter<typename TType::Value>::format(value.value, context);
		}
	};

	#pragma endregion

	#pragma region miscellaneous

	template <>
	struct formatter<Twinning::Kernel::Path> :
		formatter<Twinning::Kernel::String> {
		template <typename Context>
		auto format (
			Twinning::Kernel::Path const & value,
			Context &                      context
		) {
			return formatter<Twinning::Kernel::String>::format(value.to_string(), context);
		}
	};

	#pragma endregion

}
