module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.sexy_texture.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.sexy_texture.version;

export namespace Twinning::Kernel::Tool::PopCap::SexyTexture {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using MagicIdentifier = StaticByteArray<8_sz>;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{{0x53_b, 0x45_b, 0x58_b, 0x59_b, 0x54_b, 0x45_b, 0x58_b, 0x00_b}};

		// ----------------

		using VersionNumber = IntegerU32;

		// ----------------

		M_record_of_data(
			M_wrap(Header),
			M_wrap(
				(IntegerU32) size_width,
				(IntegerU32) size_height,
				(IntegerU32) format,
				(IntegerU32) compress_data,
				(ConstantBlock<0x00000001_iu32>) unknown_5,
				(IntegerU32) compress_data_size,
				(ConstantBlock<0x00000000_iu32>) unknown_7,
				(ConstantBlock<0x00000000_iu32>) unknown_8,
				(ConstantBlock<0x00000000_iu32>) unknown_9,
			),
		);

		// ----------------

		struct FormatFlag {

			inline static constexpr auto lut_8 = Integer{1_i};

			inline static constexpr auto argb_8888 = Integer{2_i};

			inline static constexpr auto argb_4444 = Integer{3_i};

			inline static constexpr auto argb_1555 = Integer{4_i};

			inline static constexpr auto rgb_565 = Integer{5_i};

			inline static constexpr auto rgba_8888_o = Integer{6_i};

			inline static constexpr auto rgba_4444 = Integer{7_i};

			inline static constexpr auto rgba_5551 = Integer{8_i};

			inline static constexpr auto xrgb_8888 = Integer{9_i};

			inline static constexpr auto la_88 = Integer{10_i};

		};

	};

}
