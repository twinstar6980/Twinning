module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.image.image;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.image.size;
import twinning.kernel.utility.image.basic_color;
import twinning.kernel.utility.image.basic_pixel;
import twinning.kernel.utility.image.basic_image_view;
import twinning.kernel.utility.image.basic_image;
import twinning.kernel.utility.math.utility;
import twinning.kernel.utility.memory.bitwise;

export namespace Twinning::Kernel::Image {

	#pragma region alias

	using Color = BasicColor<ZIntegerU8>;

	// ----------------

	using Pixel = BasicPixelRgba<Color>;

	// ----------------

	using VariableImageView = VariableBasicImageView<Pixel>;

	using ConstantImageView = ConstantBasicImageView<Pixel>;

	using Image = BasicImage<Pixel>;

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_color_minimum = Color{0x00};

	inline constexpr auto k_color_maximum = Color{0xFF};

	#pragma endregion

	#pragma region utility

	template <typename TValue> requires
		CategoryConstraint<>
		&& (IsBaseBoxValue<TValue>)
	inline constexpr auto compress_color(
		BasicColor<TValue> const & value,
		Size const &               bit_count
	) -> BasicColor<TValue> {
		assert_test(0_sz < bit_count && bit_count <= type_bit_size_of<BasicColor<TValue>>());
		return Bitwise::extract(value, type_bit_size_of<BasicColor<TValue>>() - bit_count, bit_count);
	}

	template <typename TValue> requires
		CategoryConstraint<>
		&& (IsBaseBoxValue<TValue>)
	inline constexpr auto uncompress_color(
		BasicColor<TValue> const & value,
		Size const &               bit_count
	) -> BasicColor<TValue> {
		assert_test(0_sz < bit_count && bit_count <= type_bit_size_of<BasicColor<TValue>>());
		return cast_box<BasicColor<TValue>>(Math::round(cast_box<Floater>(value) * cast_box<Floater>(~BasicColor<TValue>{0}) / cast_box<Floater>(~BasicColor<TValue>{0} >> (type_bit_size_of<BasicColor<TValue>>() - bit_count))));
	}

	// ----------------

	template <typename TValue> requires
		CategoryConstraint<>
		&& (IsBaseBoxValue<TValue>)
	inline constexpr auto convert_luminance_from_rgb(
		BasicColor<TValue> const & red,
		BasicColor<TValue> const & green,
		BasicColor<TValue> const & blue
	) -> BasicColor<TValue> {
		return cast_box<BasicColor<TValue>>(Math::round(cast_box<Floater>(red) * 0.299_f + cast_box<Floater>(green) * 0.587_f + cast_box<Floater>(blue) * 0.114_f));
	}

	#pragma endregion

}
