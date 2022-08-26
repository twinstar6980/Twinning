#pragma once

#include "core/utility/string/string.hpp"
#include "core/utility/base_wrapper/base.hpp"
#include "core/utility/file_system/path.hpp"

namespace fmt {

	#pragma region string

	template <auto t_constant> requires
		TwinKleS::Core::AutoConstraint
	struct formatter<TwinKleS::Core::StringView<t_constant>> :
		formatter<std::string_view> {
		template <typename Context>
		auto format (
			TwinKleS::Core::StringView<t_constant> const & value,
			Context &                                      context
		) {
			return formatter<std::string_view>::format(TwinKleS::Core::make_std_string_view(value), context);
		}
	};

	template <>
	struct formatter<TwinKleS::Core::String> :
		formatter<std::string_view> {
		template <typename Context>
		auto format (
			TwinKleS::Core::String const & value,
			Context &                      context
		) {
			return formatter<std::string_view>::format(TwinKleS::Core::make_std_string_view(value), context);
		}
	};

	#pragma endregion

	#pragma region base wrapper

	template <typename TType> requires
		TwinKleS::Core::AutoConstraint
		&& (TwinKleS::Core::IsBaseWrapper<TType>)
	#if defined M_compiler_clang // NOTE : avoid clang bug
	&& (TwinKleS::Core::IsDerivedFrom<TType, TwinKleS::Core::BaseWrapper<typename TType::Value>>)
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

	#pragma region misc

	template <>
	struct formatter<TwinKleS::Core::Path> :
		formatter<TwinKleS::Core::String> {
		template <typename Context>
		auto format (
			TwinKleS::Core::Path const & value,
			Context &                    context
		) {
			return formatter<TwinKleS::Core::String>::format(value.to_string(), context);
		}
	};

	#pragma endregion

}
