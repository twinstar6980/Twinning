#pragma once

#include "core/utility/data/json/value.hpp"

namespace TwinStar::Core::Image {

	#pragma region type

	using Channel = IntegerU8;

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_opaque_alpha = Channel{0xFF_iu8};

	#pragma endregion

	#pragma region utility

	inline auto compress_channel (
		Channel const & value,
		Size const &    bit_count
	) -> Channel {
		return clip_bit(value, k_type_bit_count<Channel> - bit_count, bit_count);
	}

	inline auto uncompress_channel (
		Channel const & value,
		Size const &    bit_count
	) -> Channel {
		return Math::round<Channel>(cbw<Floating64>(value) * 255.0_f64 / cbw<Floating64>((0b1_iu16 << bit_count) - 1_iu16));
	}

	#pragma endregion

}
