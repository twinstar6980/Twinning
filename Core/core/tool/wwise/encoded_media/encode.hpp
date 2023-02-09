#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Tool::Wwise::EncodedMedia {

	struct EncodeCommon {

	protected:

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

	struct Decode :
		EncodeCommon {

	protected:

		static auto reshuffle_adpcm (
			VListView<IntegerU32> const & raw,
			CListView<IntegerU32> const & ripe,
			Size const &                  block_align,
			Size const &                  channel_count
		) -> Void {
			auto block_size = block_align / bs_static_size<IntegerU32>();
			auto sub_block_size = channel_count;
			for (auto & block_index : SizeRange{ripe.size() / block_size}) {
				for (auto & sub_block_index : SizeRange{block_size / sub_block_size}) {
					for (auto & channel_index : SizeRange{sub_block_size}) {
						raw[(block_index * block_size) + (sub_block_index * sub_block_size) + (channel_index)] = ripe[(block_index * block_size) + (channel_index * (block_size / sub_block_size)) + (sub_block_index)];
					}
				}
			}
			return;
		}

		// ----------------

		static auto process_audio (
			CByteListView const & ripe,
			ByteArray &           raw,
			Path const &          ffmpeg_program,
			Path const &          ww2ogg_program,
			Path const &          ww2ogg_code_book,
			Path const &          temporary_directory
		) -> Void {
			auto external_program_execute_result = IntegerU32{};
			struct {
				Path ffmpeg_output;
				Path ffmpeg_error;
				Path ww2ogg_output;
				Path ffmpeg_input_audio;
				Path ffmpeg_output_audio;
				Path ww2ogg_input_audio;
			}
				temporary_file = {
					.ffmpeg_output = temporary_directory / "ffmpeg_output"_s,
					.ffmpeg_error = temporary_directory / "ffmpeg_error"_s,
					.ww2ogg_output = temporary_directory / "ww2ogg_output"_s,
					.ffmpeg_input_audio = temporary_directory / "ffmpeg_input_audio"_s,
					.ffmpeg_output_audio = temporary_directory / "ffmpeg_output_audio"_s,
					.ww2ogg_input_audio = temporary_directory / "ww2ogg_input_audio"_s,
				};
			FileSystem::create_file(temporary_file.ffmpeg_output);
			FileSystem::create_file(temporary_file.ffmpeg_error);
			FileSystem::create_file(temporary_file.ww2ogg_output);
			auto subchunk_location = [&] {
				auto it = Map<FourCC, Tuple<Size, Size>>{};
				auto ripe_stream = IByteStreamView{ripe};
				auto riff_sign = ripe_stream.read_of<WaveStructure::ChunkSign>();
				assert_test(riff_sign.id == WaveStructure::ChunkSignFlag::riff);
				auto chunk_offset = ripe_stream.position();
				auto riff_format = ripe_stream.read_of<WaveStructure::RIFFChunk::Format>();
				assert_test(riff_format.format == WaveStructure::RIFFChunk::FormatFlag::wave);
				while (!ripe_stream.full()) {
					auto sign = ripe_stream.read_of<WaveStructure::ChunkSign>();
					assert_test(!it.has_key(sign.id));
					auto location = make_tuple_of(ripe_stream.position(), cbw<Size>(sign.size));
					ripe_stream.forward(location.get<2_ix>());
					it.append(sign.id, location);
				}
				assert_test(ripe_stream.position() - chunk_offset == cbw<Size>(riff_sign.size));
				return it;
			}();
			auto get_subchunk =
				[&] (
				FourCC const & id
			) -> auto {
				auto location = subchunk_location[id];
				return ripe.sub(location.get<1_ix>(), location.get<2_ix>());
			};
			auto format_chunk_stream = IByteStreamView{get_subchunk(WaveStructure::ChunkSignFlag::fmt)};
			auto format = format_chunk_stream.read_of<WaveStructure::RIFFChunk::WaveChunk::FormatChunk>();
			auto data = get_subchunk(WaveStructure::ChunkSignFlag::data);
			switch (format.audio_format.value) {
				case AudioFormatFlag::pcm.value : {
					auto real_ripe = ByteArray{ripe.size()};
					auto real_ripe_stream = OByteStreamView{real_ripe};
					auto riff_chunk_sign_stream = OByteStreamView{real_ripe_stream.forward_view(bs_static_size<WaveStructure::ChunkSign>())};
					real_ripe_stream.write(WaveStructure::RIFFChunk::FormatFlag::wave);
					real_ripe_stream.write(
						WaveStructure::ChunkSign{
							.id = WaveStructure::ChunkSignFlag::fmt,
							.size = cbw<IntegerU32>(bs_static_size<WaveStructure::RIFFChunk::WaveChunk::FormatChunk>()),
						}
					);
					real_ripe_stream.write(
						[&] {
							auto it = format;
							it.audio_format = WaveStructure::RIFFChunk::WaveChunk::AudioFormatFlag::pcm;
							return it;
						}()
					);
					real_ripe_stream.write(
						WaveStructure::ChunkSign{
							.id = WaveStructure::ChunkSignFlag::data,
							.size = cbw<IntegerU32>(data.size()),
						}
					);
					real_ripe_stream.write(data);
					riff_chunk_sign_stream.write(
						WaveStructure::ChunkSign{
							.id = WaveStructure::ChunkSignFlag::riff,
							.size = cbw<IntegerU32>(real_ripe_stream.position() - bs_static_size<WaveStructure::ChunkSign>()),
						}
					);
					FileSystem::write_file(temporary_file.ffmpeg_input_audio, real_ripe);
					external_program_execute_result = Process::execute(
						ffmpeg_program,
						make_list<String>(
							"-v"_sv,
							"warning"_sv,
							"-y"_sv,
							"-i"_sv,
							temporary_file.ffmpeg_input_audio.to_string(),
							"-f"_sv,
							"wav"_sv,
							"-bitexact"_sv,
							temporary_file.ffmpeg_output_audio.to_string()
						),
						make_list<String>(
						),
						k_null_optional,
						make_optional_of(temporary_file.ffmpeg_output),
						make_optional_of(temporary_file.ffmpeg_error)
					);
					assert_test(external_program_execute_result == 0x00000000_iu32);
					raw = FileSystem::read_file(temporary_file.ffmpeg_output_audio);
					FileSystem::remove(temporary_file.ffmpeg_input_audio);
					FileSystem::remove(temporary_file.ffmpeg_output_audio);
					break;
				}
				case AudioFormatFlag::adpcm.value : {
					auto format_expand_size = cbw<Size>(format_chunk_stream.read_of<IntegerU16>());
					assert_test(format_expand_size == bs_static_size<FormatChunkExpand6>());
					auto format_expand = format_chunk_stream.read_of<FormatChunkExpand6>();
					auto real_ripe = ByteArray{ripe.size()};
					auto real_ripe_stream = OByteStreamView{real_ripe};
					auto riff_chunk_sign_stream = OByteStreamView{real_ripe_stream.forward_view(bs_static_size<WaveStructure::ChunkSign>())};
					real_ripe_stream.write(WaveStructure::RIFFChunk::FormatFlag::wave);
					real_ripe_stream.write(
						WaveStructure::ChunkSign{
							.id = WaveStructure::ChunkSignFlag::fmt,
							.size = cbw<IntegerU32>(bs_static_size<WaveStructure::RIFFChunk::WaveChunk::FormatChunk>() + bs_static_size<WaveStructure::RIFFChunk::WaveChunk::FormatChunkExpandOfADPCMIMA>()),
						}
					);
					real_ripe_stream.write(
						[&] {
							auto it = format;
							it.audio_format = WaveStructure::RIFFChunk::WaveChunk::AudioFormatFlag::adpcm_ima;
							return it;
						}()
					);
					real_ripe_stream.write(
						WaveStructure::RIFFChunk::WaveChunk::FormatChunkExpandOfADPCMIMA{
							.sample_per_block = cbw<IntegerU16>((cbw<IntegerU32>(format.block_align) - cbw<IntegerU32>(format.channel_count) * 4_iu32) * 8_iu32 / (cbw<IntegerU32>(format.bit_per_sample) * cbw<IntegerU32>(format.channel_count))) + 1_iu16,
						}
					);
					real_ripe_stream.write(
						WaveStructure::ChunkSign{
							.id = WaveStructure::ChunkSignFlag::data,
							.size = cbw<IntegerU32>(data.size()),
						}
					);
					auto raw_data_32 = from_byte_view<IntegerU32>(real_ripe_stream.forward_view(data.size()));
					reshuffle_adpcm(raw_data_32, from_byte_view<IntegerU32>(data), cbw<Size>(format.block_align), cbw<Size>(format.channel_count));
					riff_chunk_sign_stream.write(
						WaveStructure::ChunkSign{
							.id = WaveStructure::ChunkSignFlag::riff,
							.size = cbw<IntegerU32>(real_ripe_stream.position() - bs_static_size<WaveStructure::ChunkSign>()),
						}
					);
					FileSystem::write_file(temporary_file.ffmpeg_input_audio, real_ripe);
					external_program_execute_result = Process::execute(
						ffmpeg_program,
						make_list<String>(
							"-v"_sv,
							"warning"_sv,
							"-y"_sv,
							"-i"_sv,
							temporary_file.ffmpeg_input_audio.to_string(),
							"-f"_sv,
							"wav"_sv,
							"-bitexact"_sv,
							temporary_file.ffmpeg_output_audio.to_string()
						),
						make_list<String>(
						),
						k_null_optional,
						make_optional_of(temporary_file.ffmpeg_output),
						make_optional_of(temporary_file.ffmpeg_error)
					);
					assert_test(external_program_execute_result == 0x00000000_iu32);
					raw = FileSystem::read_file(temporary_file.ffmpeg_output_audio);
					FileSystem::remove(temporary_file.ffmpeg_input_audio);
					FileSystem::remove(temporary_file.ffmpeg_output_audio);
					break;
				}
				case AudioFormatFlag::aac.value : {
					FileSystem::write_file(temporary_file.ffmpeg_input_audio, data);
					external_program_execute_result = Process::execute(
						ffmpeg_program,
						make_list<String>(
							"-v"_sv,
							"warning"_sv,
							"-y"_sv,
							"-i"_sv,
							temporary_file.ffmpeg_input_audio.to_string(),
							"-f"_sv,
							"wav"_sv,
							"-bitexact"_sv,
							temporary_file.ffmpeg_output_audio.to_string()
						),
						make_list<String>(
						),
						k_null_optional,
						make_optional_of(temporary_file.ffmpeg_output),
						make_optional_of(temporary_file.ffmpeg_error)
					);
					assert_test(external_program_execute_result == 0x00000000_iu32);
					raw = FileSystem::read_file(temporary_file.ffmpeg_output_audio);
					FileSystem::remove(temporary_file.ffmpeg_input_audio);
					FileSystem::remove(temporary_file.ffmpeg_output_audio);
					break;
				}
				case AudioFormatFlag::vorbis.value : {
					FileSystem::write_file(temporary_file.ww2ogg_input_audio, ripe);
					external_program_execute_result = Process::execute(
						ww2ogg_program,
						make_list<String>(
							"--pcb"_sv,
							ww2ogg_code_book.to_string(),
							temporary_file.ww2ogg_input_audio.to_string(),
							"-o"_sv,
							temporary_file.ffmpeg_input_audio.to_string()
						),
						make_list<String>(
						),
						k_null_optional,
						make_optional_of(temporary_file.ww2ogg_output),
						k_null_optional
					);
					assert_test(external_program_execute_result == 0x00000000_iu32);
					external_program_execute_result = Process::execute(
						ffmpeg_program,
						make_list<String>(
							"-v"_sv,
							"warning"_sv,
							"-y"_sv,
							"-i"_sv,
							temporary_file.ffmpeg_input_audio.to_string(),
							"-f"_sv,
							"wav"_sv,
							"-bitexact"_sv,
							temporary_file.ffmpeg_output_audio.to_string()
						),
						make_list<String>(
						),
						k_null_optional,
						make_optional_of(temporary_file.ffmpeg_output),
						make_optional_of(temporary_file.ffmpeg_error)
					);
					assert_test(external_program_execute_result == 0x00000000_iu32);
					raw = FileSystem::read_file(temporary_file.ffmpeg_output_audio);
					FileSystem::remove(temporary_file.ww2ogg_input_audio);
					FileSystem::remove(temporary_file.ffmpeg_input_audio);
					FileSystem::remove(temporary_file.ffmpeg_output_audio);
					break;
				}
				default : {
					assert_fail(R"(format.audio_format == /* valid */)");
				}
			}
			return;
		}

	public:

		static auto do_process_audio (
			CByteListView const & ripe,
			ByteArray &           raw,
			Path const &          ffmpeg_program,
			Path const &          ww2ogg_program,
			Path const &          ww2ogg_code_book,
			Path const &          temporary_directory
		) -> Void {
			restruct(raw);
			return process_audio(ripe, raw, ffmpeg_program, ww2ogg_program, ww2ogg_code_book, temporary_directory);
		}

	};

}
