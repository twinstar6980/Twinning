#pragma once

#include "core/utility/utility.hpp"

namespace TwinKleS::Core::Tool::WaveStructure {

	struct ChunkSign {
		FourCC     id;
		IntegerU32 size;
	};

	namespace ChunkSignFlag {
		inline constexpr auto riff = FourCC{"RIFF"_fourcc};
		inline constexpr auto fmt = FourCC{"fmt "_fourcc};
		inline constexpr auto fact = FourCC{"fact"_fourcc};
		inline constexpr auto data = FourCC{"data"_fourcc};
	}

	// ----------------

	namespace RIFFChunk {

		struct Format {
			FourCC format;
		};

		namespace FormatFlag {
			inline constexpr auto wave = FourCC{"WAVE"_fourcc};
		}

		// ----------------

		namespace WaveChunk {

			struct FormatChunk {
				IntegerU16 audio_format;
				IntegerU16 channel_count;
				IntegerU32 sample_per_second;
				IntegerU32 byte_per_second;
				IntegerU16 block_align;
				IntegerU16 bit_per_sample;
			};

			namespace AudioFormatFlag {
				inline constexpr auto pcm = IntegerU16{0x0001_iu16};
				inline constexpr auto adpcm_ms = IntegerU16{0x0002_iu16};
				inline constexpr auto adpcm_ima = IntegerU16{0x0011_iu16};
			}

			// ----------------

			struct FormatChunkExpandOfADPCMIMA {
				IntegerU16 sample_per_block;
			};

			// ----------------

			struct FactChunk {
				IntegerU32 sample_total;
			};

		}

	}

}

namespace TwinKleS::Core {

	M_byte_stream_adapter_for_aggregate_by_field_of_unique(
		M_wrap(Tool::WaveStructure::ChunkSign),
		M_wrap(id, size),
	);

	M_byte_stream_adapter_for_aggregate_by_field_of_unique(
		M_wrap(Tool::WaveStructure::RIFFChunk::Format),
		M_wrap(format),
	);

	M_byte_stream_adapter_for_aggregate_by_field_of_unique(
		M_wrap(Tool::WaveStructure::RIFFChunk::WaveChunk::FormatChunk),
		M_wrap(audio_format, channel_count, sample_per_second, byte_per_second, block_align, bit_per_sample),
	);

	M_byte_stream_adapter_for_aggregate_by_field_of_unique(
		M_wrap(Tool::WaveStructure::RIFFChunk::WaveChunk::FormatChunkExpandOfADPCMIMA),
		M_wrap(sample_per_block),
	);

	M_byte_stream_adapter_for_aggregate_by_field_of_unique(
		M_wrap(Tool::WaveStructure::RIFFChunk::WaveChunk::FactChunk),
		M_wrap(sample_total),
	);

}
