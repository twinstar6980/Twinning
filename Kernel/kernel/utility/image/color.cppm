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

	template <typename TValue> requires
		CategoryConstraint<>
		&& (IsBaseBoxValue<TValue>)
	inline auto compress_color(
		BasicColor<TValue> const & value,
		Size const &               bit_count
	) -> BasicColor<TValue> {
		return clip_bit(value, k_type_bit_count<BasicColor<TValue>> - bit_count, bit_count);
	}

	template <typename TValue> requires
		CategoryConstraint<>
		&& (IsBaseBoxValue<TValue>)
	inline auto uncompress_color(
		BasicColor<TValue> const & value,
		Size const &               bit_count
	) -> BasicColor<TValue> {
		return Math::round<BasicColor<TValue>>(cbox<Floater>(value) * cbox<Floater>(~BasicColor<TValue>{0}) / cbox<Floater>((0b1_iu64 << bit_count) - 1_iu64));
	}

	// ----------------

	template <typename TValue> requires
		CategoryConstraint<>
		&& (IsBaseBoxValue<TValue>)
	inline auto convert_luminance_from_rgb(
		BasicColor<TValue> const & red,
		BasicColor<TValue> const & green,
		BasicColor<TValue> const & blue
	) -> BasicColor<TValue> {
		return Math::round<BasicColor<TValue>>(cbox<Floater>(red) * 0.299_f + cbox<Floater>(green) * 0.587_f + cbox<Floater>(blue) * 0.114_f);
	}

	#pragma endregion

}
