#pragma once

#include "core/utility/base_wrapper/wrapper.hpp"
#include "core/utility/string/string.hpp"
#include "core/utility/string/format.hpp"
#include "core/utility/exception/base_exception/base_exception.hpp"

namespace TwinKleS::Core {

	#pragma region make

	template <typename Enum, typename Value> requires
		CategoryConstraint<IsPureInstance<Enum> && IsPureInstance<Value>>
		&& (IsEnum<Enum>)
		&& (IsIntegerWrapper<Value>)
	inline constexpr auto make_enum (
		Value const & value
	) -> Enum {
		return static_cast<Enum>(value.value);
	}

	template <typename Value> requires
		CategoryConstraint<IsPureInstance<Value>>
		&& (IsEnum<Value>)
	inline constexpr auto underlying_enum (
		Value const & value
	) -> IntegerWrapper<AsEnumUnderlying<Value>> {
		return IntegerWrapper<AsEnumUnderlying<Value>>{static_cast<AsEnumUnderlying<Value>>(value)};
	}

	#pragma endregion

}
