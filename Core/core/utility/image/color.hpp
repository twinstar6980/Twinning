#pragma once

#include "core/utility/data/json/value.hpp"

namespace TwinStar::Core::Image {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<>
		&& (IsBaseWrapperValue<TValue>)
	M_define_simple_derived_class(BasicColor, IntegerWrapper<TValue>, IntegerWrapper);

	#pragma endregion

	#pragma region utility

	template <typename Value> requires
		CategoryConstraint<>
		&& (IsBaseWrapperValue<Value>)
	inline auto compress_color (
		BasicColor<Value> const & value,
		Size const &              bit_count
	) -> BasicColor<Value> {
		return clip_bit(value, k_type_bit_count<BasicColor<Value>> - bit_count, bit_count);
	}

	template <typename Value> requires
		CategoryConstraint<>
		&& (IsBaseWrapperValue<Value>)
	inline auto uncompress_color (
		BasicColor<Value> const & value,
		Size const &              bit_count
	) -> BasicColor<Value> {
		return Math::round<BasicColor<Value>>(cbw<Floating64>(value) * cbw<Floating64>(~BasicColor<Value>{0}) / cbw<Floating64>((0b1_iu64 << bit_count) - 1_iu64));
	}

	#pragma endregion

}
