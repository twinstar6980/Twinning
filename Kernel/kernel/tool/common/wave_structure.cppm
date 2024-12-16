module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.common.wave_structure;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::WaveStructure {

	#pragma region type

	M_record_of_data(
		M_wrap(ChunkSign),
		M_wrap(
			(FourCC) id,
			(IntegerU32) size,
		),
	);

	namespace ChunkSignFlag {
		inline constexpr auto riff = FourCC{"RIFF"_fourcc};
		inline constexpr auto fmt = FourCC{"fmt "_fourcc};
		inline constexpr auto fact = FourCC{"fact"_fourcc};
		inline constexpr auto data = FourCC{"data"_fourcc};
	}

	// ----------------

	namespace RIFFChunk {

		M_record_of_data(
			M_wrap(Format),
			M_wrap(
				(FourCC) format,
			),
		);

		namespace FormatFlag {
			inline constexpr auto wave = FourCC{"WAVE"_fourcc};
		}

		// ----------------

		namespace WaveChunk {

			M_record_of_data(
				M_wrap(FormatChunk),
				M_wrap(
					(IntegerU16) audio_format,
					(IntegerU16) channel_count,
					(IntegerU32) sample_per_second,
					(IntegerU32) byte_per_second,
					(IntegerU16) block_align,
					(IntegerU16) bit_per_sample,
				),
			);

			namespace AudioFormatFlag {
				inline constexpr auto pcm = IntegerU16{0x0001_iu16};
				inline constexpr auto adpcm_ms = IntegerU16{0x0002_iu16};
				inline constexpr auto adpcm_ima = IntegerU16{0x0011_iu16};
			}

			// ----------------

			M_record_of_data(
				M_wrap(FormatChunkExpandOfADPCMIMA),
				M_wrap(
					(IntegerU16) sample_per_block,
				),
			);

			// ----------------

			M_record_of_data(
				M_wrap(FactChunk),
				M_wrap(
					(IntegerU32) sample_total,
				),
			);

		}

	}

	#pragma endregion

}
