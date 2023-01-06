#pragma once

#include "core/utility/string/string.hpp"
#include "core/utility/base_wrapper/base.hpp"
#include "core/utility/file_system/path.hpp"

namespace fmt {

	#pragma region string

	template <auto t_constant> requires
		TwinStar::Core::AutoConstraint
	struct formatter<TwinStar::Core::StringView<t_constant>> :
		formatter<std::string_view> {
		template <typename Context>
		auto format (
			TwinStar::Core::StringView<t_constant> const & value,
			Context &                                      context
		) {
			return formatter<std::string_view>::format(TwinStar::Core::make_std_string_view(value), context);
		}
	};

	template <>
	struct formatter<TwinStar::Core::String> :
		formatter<std::string_view> {
		template <typename Context>
		auto format (
			TwinStar::Core::String const & value,
			Context &                      context
		) {
			return formatter<std::string_view>::format(TwinStar::Core::make_std_string_view(value), context);
		}
	};

	#pragma endregion

	#pragma region base wrapper

	template <typename TType> requires
		TwinStar::Core::AutoConstraint
		&& (TwinStar::Core::IsBaseWrapper<TType>)
	#if defined M_compiler_clang // NOTE : avoid clang bug
		&& (TwinStar::Core::IsDerivedFrom<TType, TwinStar::Core::BaseWrapper<typename TType::Value>>)
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
	struct formatter<TwinStar::Core::Path> :
		formatter<TwinStar::Core::String> {
		template <typename Context>
		auto format (
			TwinStar::Core::Path const & value,
			Context &                    context
		) {
			return formatter<TwinStar::Core::String>::format(value.to_string(), context);
		}
	};

	#pragma endregion

}
