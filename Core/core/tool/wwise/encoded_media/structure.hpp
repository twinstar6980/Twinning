#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/common/wave_structure.hpp"

namespace TwinKleS::Core::Tool::Wwise::EncodedMedia::Structure {

	namespace AudioFormatFlag {
		inline constexpr auto pcm = IntegerU16{0xFFFE_iu16};
		inline constexpr auto adpcm = IntegerU16{0x0002_iu16};
		inline constexpr auto aac = IntegerU16{0xAAC0_iu16};
		inline constexpr auto vorbis = IntegerU16{0xFFFF_iu16};
	}

	// ----------------

	struct FormatChunkExpand6 {
		IntegerU16 unknown_0;
		IntegerU32 channel_mask;
	};

}

namespace TwinKleS::Core {

	M_byte_stream_adapter_for_aggregate_by_field_of_unique(
		M_wrap(Tool::Wwise::EncodedMedia::Structure::FormatChunkExpand6),
		M_wrap(unknown_0, channel_mask),
	);

}
