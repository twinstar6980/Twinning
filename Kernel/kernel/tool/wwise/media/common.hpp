#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/wwise/media/version.hpp"
#include "kernel/tool/common/wave_structure.hpp"

namespace TwinStar::Kernel::Tool::Wwise::Media {

	template <auto version> requires (check_version(version))
	struct Common {

		struct AudioFormatFlag {
			inline static constexpr auto pcm = IntegerU16{0xFFFE_iu16};
			inline static constexpr auto adpcm = IntegerU16{0x0002_iu16};
			inline static constexpr auto aac = IntegerU16{0xAAC0_iu16};
			inline static constexpr auto vorbis = IntegerU16{0xFFFF_iu16};
		};

		// ----------------

		M_record_of_data(
			M_wrap(FormatChunkExpand6),
			M_wrap(
				(IntegerU16) unknown_0,
				(IntegerU32) channel_mask,
			),
		);

	};

}
