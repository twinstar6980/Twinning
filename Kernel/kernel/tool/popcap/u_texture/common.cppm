module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.u_texture.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.u_texture.version;

export namespace Twinning::Kernel::Tool::PopCap::UTexture {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using MagicIdentifier = StaticByteArray<2_sz>;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{{0x75_b, 0x0A_b}};

		// ----------------

		M_record_of_data(
			M_wrap(Header),
			M_wrap(
				(IntegerU16) size_width,
				(IntegerU16) size_height,
				(IntegerU16) format,
			),
		);

		// ----------------

		struct FormatFlag {

			inline static constexpr auto rgba_8888_o = Integer{1_i};

			inline static constexpr auto rgba_4444 = Integer{2_i};

			inline static constexpr auto rgba_5551 = Integer{3_i};

			inline static constexpr auto rgb_565 = Integer{4_i};

		};

	};

}
