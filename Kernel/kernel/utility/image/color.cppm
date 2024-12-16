module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.image.color;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.miscellaneous.math;

export namespace Twinning::Kernel::Image {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<>
		&& (IsBaseBoxValue<TValue>)
	M_simple_derived_class(BasicColor, IntegerBox<TValue>, IntegerBox);

	#pragma endregion

	#pragma region utility

	template <typename Value> requires
		CategoryConstraint<>
		&& (IsBaseBoxValue<Value>)
	inline auto compress_color (
		BasicColor<Value> const & value,
		Size const &              bit_count
	) -> BasicColor<Value> {
		return clip_bit(value, k_type_bit_count<BasicColor<Value>> - bit_count, bit_count);
	}

	template <typename Value> requires
		CategoryConstraint<>
		&& (IsBaseBoxValue<Value>)
	inline auto uncompress_color (
		BasicColor<Value> const & value,
		Size const &              bit_count
	) -> BasicColor<Value> {
		return Math::round<BasicColor<Value>>(cbox<Floater>(value) * cbox<Floater>(~BasicColor<Value>{0}) / cbox<Floater>((0b1_iu64 << bit_count) - 1_iu64));
	}

	// ----------------

	template <typename Value> requires
		CategoryConstraint<>
		&& (IsBaseBoxValue<Value>)
	inline auto convert_luminance_from_rgb (
		BasicColor<Value> const & red,
		BasicColor<Value> const & green,
		BasicColor<Value> const & blue
	) -> BasicColor<Value> {
		return Math::round<BasicColor<Value>>(cbox<Floater>(red) * 0.299_f + cbox<Floater>(green) * 0.587_f + cbox<Floater>(blue) * 0.114_f);
	}

	#pragma endregion

}
