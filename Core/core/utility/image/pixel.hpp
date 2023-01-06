#pragma once

#include "core/utility/image/channel.hpp"

namespace TwinStar::Core::Image {

	#pragma region type

	struct PixelRGB {

		Channel red;
		Channel green;
		Channel blue;

		// ----------------

		friend constexpr auto operator == (
			PixelRGB const & thix,
			PixelRGB const & that
		) -> bool = default;

	};

	struct PixelRGBA {

		Channel red;
		Channel green;
		Channel blue;
		Channel alpha;

		// ----------------

		friend constexpr auto operator == (
			PixelRGBA const & thix,
			PixelRGBA const & that
		) -> bool = default;

	};

	#pragma endregion

	#pragma region alias

	using Pixel = PixelRGBA;

	#pragma endregion

}
