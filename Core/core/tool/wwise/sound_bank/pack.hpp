#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/wwise/sound_bank/version.hpp"
#include "core/tool/wwise/sound_bank/manifest.hpp"
#include "core/tool/wwise/sound_bank/structure.hpp"

namespace TwinKleS::Core::Tool::Wwise::SoundBank {

	inline namespace CommonOfPack {

		namespace Detail {

			inline auto compute_id (
				String const & name
			) -> IntegerU32 {
				auto lower_name = String{name};
				lower_name.as_lower_case();
				return Data::Hash::FNV::Hash::process_1_32(to_byte_view(lower_name.as_view()));
			}

		}

	}

	namespace Pack {

		namespace Detail {

			#pragma region using

			using namespace CommonOfPack::Detail;

			#pragma endregion

			#pragma region process

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_package (
				OByteStreamView &                    sound_bank_data,
				Manifest::SoundBank<version> const & sound_bank_manifest,
				Path const &                         embedded_audio_directory
			) -> Void {
				{
					auto sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<Structure::ChunkSign>())};
					auto chunk_data = OByteStreamView{sound_bank_data.reserve_view()};
					chunk_data.write(
						Structure::BKHDBegin<version>{
							.version = cbw<IntegerU32>(version.number),
							.id = cbw<IntegerU32>(sound_bank_manifest.id),
						}
					);
					chunk_data.write(sound_bank_manifest.header_expand);
					sign_data.write(
						Structure::ChunkSign{
							.id = Structure::ChunkSignFlag::bkhd,
							.size = cbw<IntegerU32>(chunk_data.position()),
						}
					);
					sound_bank_data.forward(chunk_data.position());
				}
				if (sound_bank_manifest.stmg) {
					auto sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<Structure::ChunkSign>())};
					auto chunk_data = OByteStreamView{sound_bank_data.reserve_view()};
					chunk_data.write(sound_bank_manifest.stmg.get());
					sign_data.write(
						Structure::ChunkSign{
							.id = Structure::ChunkSignFlag::stmg,
							.size = cbw<IntegerU32>(chunk_data.position()),
						}
					);
					sound_bank_data.forward(chunk_data.position());
				}
				if (sound_bank_manifest.embedded_audio) {
					auto didx_sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<Structure::ChunkSign>())};
					auto didx_chunk_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<List<Structure::DIDXItem<version>>>(sound_bank_manifest.embedded_audio.get().size()))};
					auto data_sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<Structure::ChunkSign>())};
					auto data_chunk_data = OByteStreamView{sound_bank_data.reserve_view()};
					for (auto & embedded_audio : sound_bank_manifest.embedded_audio.get()) {
						data_chunk_data.pad(compute_padding_size(sound_bank_data.position() + data_chunk_data.position(), Structure::k_data_block_padding_size));
						auto data_offset = data_chunk_data.position();
						auto data_size = FileSystem::read_stream_file(embedded_audio_directory / "{}.wem"_sf(embedded_audio), data_chunk_data);
						didx_chunk_data.write(
							Structure::DIDXItem<version>{
								.id = cbw<IntegerU32>(embedded_audio),
								.offset = cbw<IntegerU32>(data_offset),
								.size = cbw<IntegerU32>(data_size),
							}
						);
					}
					didx_sign_data.write(
						Structure::ChunkSign{
							.id = Structure::ChunkSignFlag::didx,
							.size = cbw<IntegerU32>(didx_chunk_data.position()),
						}
					);
					data_sign_data.write(
						Structure::ChunkSign{
							.id = Structure::ChunkSignFlag::data,
							.size = cbw<IntegerU32>(data_chunk_data.position()),
						}
					);
					sound_bank_data.forward(data_chunk_data.position());
				}
				if (sound_bank_manifest.hirc) {
					auto sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<Structure::ChunkSign>())};
					auto chunk_data = OByteStreamView{sound_bank_data.reserve_view()};
					chunk_data.write(
						Structure::HIRCBegin<version>{
							.item_count = cbw<IntegerU32>(sound_bank_manifest.hirc.get().size()),
						}
					);
					for (auto & hirc : sound_bank_manifest.hirc.get()) {
						chunk_data.write(
							Structure::HIRCItemBegin<version>{
								.type = cbw<IntegerU8>(hirc.type),
								.size = cbw<IntegerU32>(bs_static_size<IntegerU32>() + hirc.data.size()),
								.id = cbw<IntegerU32>(hirc.id),
							}
						);
						chunk_data.write(hirc.data);
					}
					sign_data.write(
						Structure::ChunkSign{
							.id = Structure::ChunkSignFlag::hirc,
							.size = cbw<IntegerU32>(chunk_data.position()),
						}
					);
					sound_bank_data.forward(chunk_data.position());
				}
				if (sound_bank_manifest.referenced_sound_bank) {
					auto sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<Structure::ChunkSign>())};
					auto chunk_data = OByteStreamView{sound_bank_data.reserve_view()};
					chunk_data.write(
						Structure::STIDBegin<version>{
							.unknown_1 = 1_iu32,
							.item_count = cbw<IntegerU32>(sound_bank_manifest.referenced_sound_bank.get().size()),
						}
					);
					for (auto & referenced_sound_bank : sound_bank_manifest.referenced_sound_bank.get()) {
						chunk_data.write(
							Structure::STIDItem<version>{
								.id = compute_id(referenced_sound_bank),
								.name = {referenced_sound_bank},
							}
						);
					}
					sign_data.write(
						Structure::ChunkSign{
							.id = Structure::ChunkSignFlag::stid,
							.size = cbw<IntegerU32>(chunk_data.position()),
						}
					);
					sound_bank_data.forward(chunk_data.position());
				}
				if (sound_bank_manifest.envs) {
					auto sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<Structure::ChunkSign>())};
					auto chunk_data = OByteStreamView{sound_bank_data.reserve_view()};
					chunk_data.write(sound_bank_manifest.envs.get());
					sign_data.write(
						Structure::ChunkSign{
							.id = Structure::ChunkSignFlag::envs,
							.size = cbw<IntegerU32>(chunk_data.position()),
						}
					);
					sound_bank_data.forward(chunk_data.position());
				}
				return;
			}

			// ----------------

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process (
				OByteStreamView &                    data_,
				Manifest::SoundBank<version> const & manifest,
				Path const &                         embedded_audio_directory
			) -> Void {
				M_use_zps_of(data);
				process_package<version>(data, manifest, embedded_audio_directory);
				return;
			}

			inline auto process (
				OByteStreamView &                  data_,
				Manifest::SoundBankVariant const & manifest,
				Path const &                       embedded_audio_directory,
				Version const &                    version
			) -> Void {
				M_use_zps_of(data);
				Generalization::execute<VersionEnum>(
					version,
					[&] <auto index, auto version> (ValuePackage<index, version>) {
						process<version>(data, manifest.get<Manifest::SoundBank<version>>(), embedded_audio_directory);
					}
				);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

	namespace Unpack {

		namespace Detail {

			#pragma region using

			using namespace CommonOfPack::Detail;

			#pragma endregion

			#pragma region process

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_package (
				IByteStreamView &              sound_bank_data,
				Manifest::SoundBank<version> & sound_bank_manifest,
				Optional<Path> const &         embedded_audio_directory
			) -> Void {
				auto state = k_false;
				auto sign = Structure::ChunkSign{};
				auto chunk = IByteStreamView{};
				auto next_chunk =
					[&] (
				) -> auto {
					state = !sound_bank_data.full();
					if (state) {
						sign = sound_bank_data.read_of<Structure::ChunkSign>();
						chunk = IByteStreamView{sound_bank_data.forward_view(cbw<Size>(sign.size))};
					}
					return;
				};
				next_chunk();
				assert_condition(state && sign.id == Structure::ChunkSignFlag::bkhd);
				{
					auto begin_structure = chunk.read_of<Structure::BKHDBegin<version>>();
					auto expand = chunk.forward_view(chunk.reserve());
					sound_bank_manifest.id = cbw<Integer>(begin_structure.id);
					sound_bank_manifest.header_expand = ByteList{expand};
					next_chunk();
				}
				if (state && sign.id == Structure::ChunkSignFlag::didx) {
					sound_bank_manifest.embedded_audio.set();
					assert_condition(is_padded_size(chunk.reserve(), bs_static_size<Structure::DIDXItem<version>>()));
					auto item_structure_list = chunk.read_of<List<Structure::DIDXItem<version>>>(chunk.reserve() / bs_static_size<Structure::DIDXItem<version>>());
					next_chunk();
					assert_condition(state && sign.id == Structure::ChunkSignFlag::data);
					sound_bank_manifest.embedded_audio.get().allocate_full(item_structure_list.size());
					for (auto & index : SizeRange{item_structure_list.size()}) {
						auto & item_structure = item_structure_list[index];
						auto   item_data = chunk.sub_view(cbw<Size>(item_structure.offset), cbw<Size>(item_structure.size));
						auto & embedded_audio_manifest = sound_bank_manifest.embedded_audio.get()[index];
						embedded_audio_manifest = cbw<Integer>(item_structure.id);
						if (embedded_audio_directory) {
							FileSystem::write_file(embedded_audio_directory.get() / "{}.wem"_sf(item_structure.id), item_data);
						}
					}
					next_chunk();
				} else {
					sound_bank_manifest.embedded_audio.reset();
				}
				if (state && sign.id == Structure::ChunkSignFlag::stmg) {
					sound_bank_manifest.stmg.set();
					sound_bank_manifest.stmg.get().assign(chunk.view());
					chunk.forward_view(chunk.reserve());
					next_chunk();
				} else {
					sound_bank_manifest.stmg.reset();
				}
				if (state && sign.id == Structure::ChunkSignFlag::hirc) {
					sound_bank_manifest.hirc.set();
					auto begin_structure = chunk.read_of<Structure::HIRCBegin<version>>();
					sound_bank_manifest.hirc.get().allocate_full(cbw<Size>(begin_structure.item_count));
					for (auto & index : SizeRange{cbw<Size>(begin_structure.item_count)}) {
						auto   item_begin_structure = chunk.read_of<Structure::HIRCItemBegin<version>>();
						auto   item_data = chunk.forward_view(cbw<Size>(item_begin_structure.size) - bs_static_size<IntegerU32>());
						auto & hirc_manifest = sound_bank_manifest.hirc.get()[index];
						hirc_manifest.id = cbw<Integer>(item_begin_structure.id);
						hirc_manifest.type = cbw<Integer>(item_begin_structure.type);
						hirc_manifest.data = ByteList{item_data};
					}
					next_chunk();
				} else {
					sound_bank_manifest.hirc.reset();
				}
				if (state && sign.id == Structure::ChunkSignFlag::stid) {
					sound_bank_manifest.referenced_sound_bank.set();
					auto begin_structure = chunk.read_of<Structure::STIDBegin<version>>();
					assert_condition(begin_structure.unknown_1 == 1_iu32);
					auto item_structure_list = chunk.read_of<List<Structure::STIDItem<version>>>(cbw<Size>(begin_structure.item_count));
					sound_bank_manifest.referenced_sound_bank.get().allocate_full(item_structure_list.size());
					for (auto & index : SizeRange{item_structure_list.size()}) {
						auto & item_structure = item_structure_list[index];
						auto & referenced_sound_bank_manifest = sound_bank_manifest.referenced_sound_bank.get()[index];
						assert_condition(compute_id(item_structure.name.value) == item_structure.id);
						referenced_sound_bank_manifest = item_structure.name.value;
					}
					next_chunk();
				} else {
					sound_bank_manifest.referenced_sound_bank.reset();
				}
				if (state && sign.id == Structure::ChunkSignFlag::envs) {
					sound_bank_manifest.envs.set();
					sound_bank_manifest.envs.get().assign(chunk.view());
					chunk.forward_view(chunk.reserve());
					next_chunk();
				} else {
					sound_bank_manifest.envs.reset();
				}
				assert_condition(!state);
				return;
			}

			// ----------------

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process (
				IByteStreamView &              data_,
				Manifest::SoundBank<version> & manifest,
				Optional<Path> const &         embedded_audio_directory
			) -> Void {
				M_use_zps_of(data);
				process_package<version>(data, manifest, embedded_audio_directory);
				return;
			}

			inline auto process (
				IByteStreamView &            data_,
				Manifest::SoundBankVariant & manifest,
				Optional<Path> const &       embedded_audio_directory,
				Version const &              version
			) -> Void {
				M_use_zps_of(data);
				Generalization::execute<VersionEnum>(
					version,
					[&] <auto index, auto version> (ValuePackage<index, version>) {
						process<version>(data, manifest.set<Manifest::SoundBank<version>>(), embedded_audio_directory);
					}
				);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

}
