#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/wwise/encoded_media/structure.hpp"

namespace TwinKleS::Core::Tool::Wwise::EncodedMedia {

	inline namespace CommonOfEncode {

		namespace Detail {

		}

	}

	namespace Decode {

		namespace Detail {

			#pragma region using

			using namespace CommonOfEncode::Detail;

			#pragma endregion

			#pragma region adpcm

			inline auto reshuffle_adpcm (
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

			#pragma endregion

			#pragma region process

			inline auto process (
				CByteListView const & ripe,
				Path const &          ffmpeg_file,
				Path const &          ww2ogg_file,
				Path const &          ww2ogg_pcb_file,
				Path const &          temporary_directory
			) -> ByteArray {
				struct {
					Path encoded_audio;
					Path input_audio;
					Path output_wav;
				}
					temporary_file = {
						.encoded_audio = temporary_directory / "encoded_audio.tmp"_s,
						.input_audio = temporary_directory / "input_audio.tmp"_s,
						.output_wav = temporary_directory / "output_wav.tmp"_s,
					};
				auto raw = ByteArray{};
				auto subchunk_location = [&] {
					auto it = Map<FourCC, Tuple<Size, Size>>{};
					auto ripe_stream = IByteStreamView{ripe};
					auto riff_sign = ripe_stream.read_of<WaveStructure::ChunkSign>();
					assert_condition(riff_sign.id == WaveStructure::ChunkSignFlag::riff);
					auto chunk_offset = ripe_stream.position();
					auto riff_format = ripe_stream.read_of<WaveStructure::RIFFChunk::Format>();
					assert_condition(riff_format.format == WaveStructure::RIFFChunk::FormatFlag::wave);
					while (!ripe_stream.full()) {
						auto sign = ripe_stream.read_of<WaveStructure::ChunkSign>();
						assert_condition(!it.has_key(sign.id));
						auto location = make_tuple_of(ripe_stream.position(), cbw<Size>(sign.size));
						ripe_stream.forward(location.get<2_ixz>());
						it.append(sign.id, location);
					}
					assert_condition(ripe_stream.position() - chunk_offset == cbw<Size>(riff_sign.size));
					return it;
				}();
				auto get_subchunk =
					[&] (
					FourCC const & id
				) -> auto {
					auto location = subchunk_location[id];
					return ripe.sub(location.get<1_ixz>(), location.get<2_ixz>());
				};
				auto format_chunk_stream = IByteStreamView{get_subchunk(WaveStructure::ChunkSignFlag::fmt)};
				auto format = format_chunk_stream.read_of<WaveStructure::RIFFChunk::WaveChunk::FormatChunk>();
				auto data = get_subchunk(WaveStructure::ChunkSignFlag::data);
				switch (format.audio_format.value) {
					default : {
						assert_failed(R"(format.audio_format == /* valid */)");
						break;
					}
					case Structure::AudioFormatFlag::pcm.value : {
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
						FileSystem::write_file(temporary_file.input_audio, real_ripe);
						System::process(
							ffmpeg_file,
							make_list<String>(
								"-v"_sv,
								"warning"_sv,
								"-y"_sv,
								"-i"_sv,
								temporary_file.input_audio.to_string(),
								"-f"_sv,
								"wav"_sv,
								"-bitexact"_sv,
								temporary_file.output_wav.to_string()
							)
						);
						raw = FileSystem::read_file(temporary_file.output_wav);
						FileSystem::remove(temporary_file.input_audio);
						FileSystem::remove(temporary_file.output_wav);
						break;
					}
					case Structure::AudioFormatFlag::adpcm.value : {
						auto format_expand_size = cbw<Size>(format_chunk_stream.read_of<IntegerU16>());
						assert_condition(format_expand_size == bs_static_size<Structure::FormatChunkExpand6>());
						auto format_expand = format_chunk_stream.read_of<Structure::FormatChunkExpand6>();
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
						FileSystem::write_file(temporary_file.input_audio, real_ripe);
						System::process(
							ffmpeg_file,
							make_list<String>(
								"-v"_sv,
								"warning"_sv,
								"-y"_sv,
								"-i"_sv,
								temporary_file.input_audio.to_string(),
								"-f"_sv,
								"wav"_sv,
								"-bitexact"_sv,
								temporary_file.output_wav.to_string()
							)
						);
						raw = FileSystem::read_file(temporary_file.output_wav);
						FileSystem::remove(temporary_file.input_audio);
						FileSystem::remove(temporary_file.output_wav);
						break;
					}
					case Structure::AudioFormatFlag::aac.value : {
						FileSystem::write_file(temporary_file.input_audio, data);
						System::process(
							ffmpeg_file,
							make_list<String>(
								"-v"_sv,
								"warning"_sv,
								"-y"_sv,
								"-i"_sv,
								temporary_file.input_audio.to_string(),
								"-f"_sv,
								"wav"_sv,
								"-bitexact"_sv,
								temporary_file.output_wav.to_string()
							)
						);
						raw = FileSystem::read_file(temporary_file.output_wav);
						FileSystem::remove(temporary_file.input_audio);
						FileSystem::remove(temporary_file.output_wav);
						break;
					}
					case Structure::AudioFormatFlag::vorbis.value : {
						FileSystem::write_file(temporary_file.encoded_audio, ripe);
						System::process(
							ww2ogg_file,
							make_list<String>(
								"--pcb"_sv,
								ww2ogg_pcb_file.to_string(),
								temporary_file.encoded_audio.to_string(),
								"-o"_sv,
								temporary_file.input_audio.to_string()
							)
						);
						System::process(
							ffmpeg_file,
							make_list<String>(
								"-v"_sv,
								"warning"_sv,
								"-y"_sv,
								"-i"_sv,
								temporary_file.input_audio.to_string(),
								"-f"_sv,
								"wav"_sv,
								"-bitexact"_sv,
								temporary_file.output_wav.to_string()
							)
						);
						raw = FileSystem::read_file(temporary_file.output_wav);
						FileSystem::remove(temporary_file.encoded_audio);
						FileSystem::remove(temporary_file.input_audio);
						FileSystem::remove(temporary_file.output_wav);
						break;
					}
				}
				return raw;
			}

			#pragma endregion

		}

		using Detail::process;

	}

}
