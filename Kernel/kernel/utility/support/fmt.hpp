#pragma once

#include "kernel/utility/string/string.hpp"
#include "kernel/utility/base_wrapper/base.hpp"
#include "kernel/utility/file_system/path.hpp"

namespace fmt {

	#pragma region string

	template <auto t_constant> requires
		TwinStar::Kernel::AutoConstraint
	struct formatter<TwinStar::Kernel::StringView<t_constant>> :
		formatter<std::string_view> {
		template <typename Context>
		auto format (
			TwinStar::Kernel::StringView<t_constant> const & value,
			Context &                                        context
		) {
			return formatter<std::string_view>::format(TwinStar::Kernel::make_std_string_view(value), context);
		}
	};

	template <>
	struct formatter<TwinStar::Kernel::String> :
		formatter<std::string_view> {
		template <typename Context>
		auto format (
			TwinStar::Kernel::String const & value,
			Context &                        context
		) {
			return formatter<std::string_view>::format(TwinStar::Kernel::make_std_string_view(value), context);
		}
	};

	#pragma endregion

	#pragma region base wrapper

	template <typename TType> requires
		TwinStar::Kernel::AutoConstraint
		&& (TwinStar::Kernel::IsBaseWrapper<TType>)
	#if defined M_compiler_clang // NOTE : avoid clang bug
		&& (TwinStar::Kernel::IsDerivedFrom<TType, TwinStar::Kernel::BaseWrapper<typename TType::Value>>)
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
	struct formatter<TwinStar::Kernel::Path> :
		formatter<TwinStar::Kernel::String> {
		template <typename Context>
		auto format (
			TwinStar::Kernel::Path const & value,
			Context &                      context
		) {
			return formatter<TwinStar::Kernel::String>::format(value.to_string(), context);
		}
	};

	#pragma endregion

}
