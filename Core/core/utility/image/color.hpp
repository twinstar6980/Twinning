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
		return Math::round<BasicColor<Value>>(cbw<Floater>(value) * cbw<Floater>(~BasicColor<Value>{0}) / cbw<Floater>((0b1_iu64 << bit_count) - 1_iu64));
	}

	// ----------------

	template <typename Value> requires
		CategoryConstraint<>
		&& (IsBaseWrapperValue<Value>)
	inline auto convert_luminance_from_rgb (
		BasicColor<Value> const & red,
		BasicColor<Value> const & green,
		BasicColor<Value> const & blue
	) -> BasicColor<Value> {
		return Math::round<BasicColor<Value>>(cbw<Floater>(red) * 0.299_f + cbw<Floater>(green) * 0.587_f + cbw<Floater>(blue) * 0.114_f);
	}

	#pragma endregion

}
