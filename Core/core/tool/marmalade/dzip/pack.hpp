#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/marmalade/dzip/version.hpp"
#include "core/tool/marmalade/dzip/manifest.hpp"
#include "core/tool/marmalade/dzip/structure.hpp"

namespace TwinKleS::Core::Tool::Marmalade::DZip {

	inline namespace CommonOfPack {

		namespace Detail {

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
				OByteStreamView &                  package_data,
				Manifest::Package<version> const & package_manifest,
				Path const &                       resource_directory
			) -> Void {
				package_data.write(Structure::k_magic_identifier);
				struct {
					OByteStreamView archive_setting;
					OByteStreamView resource_file;
					OByteStreamView resource_directory;
					OByteStreamView resource_information;
					OByteStreamView chunk_setting;
					OByteStreamView chunk_information;
					OByteStreamView archive_resource_information;
				}
					information_data = {};
				{
					auto information_structure = Structure::Information<version>{};
					information_structure.resource_file.allocate(package_manifest.resource.size());
					information_structure.resource_directory.allocate(package_manifest.resource.size() + 1_sz);
					information_structure.resource_information.allocate_full(package_manifest.resource.size());
					information_structure.chunk_information.allocate_full(k_none_size);
					information_structure.archive_resource_information.allocate_full(k_none_size);
					information_structure.resource_directory.append(""_sv);
					auto global_chunk_index = k_begin_index;
					for (auto & resource_index : SizeRange{package_manifest.resource.size()}) {
						auto & resource_manifest = package_manifest.resource.at(resource_index);
						auto & resource_information_structure = information_structure.resource_information[resource_index];
						auto   resource_directory_string = resource_manifest.key.parent().to_string(CharacterType::PathSeparator::windows);
						information_structure.resource_file.append(resource_manifest.key.name());
						if (auto index = Range::find_index(information_structure.resource_directory, resource_directory_string)) {
						} else {
							information_structure.resource_directory.append(resource_directory_string);
						}
						resource_information_structure.chunk_index.allocate_full(cbw<Size>(resource_manifest.value.chunk.size()));
						for (auto & chunk_index : SizeRange{resource_manifest.value.chunk.size()}) {
							information_structure.chunk_information.append();
							++global_chunk_index;
						}
					}
					information_data.archive_setting = OByteStreamView{
						package_data.forward_view(bs_size(information_structure.archive_setting))
					};
					information_data.resource_file = OByteStreamView{
						package_data.forward_view(
							[&] {
								auto size = k_none_size;
								for (auto & element : information_structure.resource_file) {
									size += bs_size(element);
									size += bs_size(CharacterType::k_null);
								}
								return size;
							}()
						)
					};
					information_data.resource_directory = OByteStreamView{
						package_data.forward_view(
							[&] {
								auto size = k_none_size;
								for (auto & element : information_structure.resource_directory.tail(information_structure.resource_directory.size() - 1_sz)) {
									size += bs_size(element);
									size += bs_size(CharacterType::k_null);
								}
								return size;
							}()
						)
					};
					information_data.resource_information = OByteStreamView{
						package_data.forward_view(bs_size(information_structure.resource_information))
					};
					information_data.chunk_setting = OByteStreamView{
						package_data.forward_view(bs_size(information_structure.chunk_setting))
					};
					information_data.chunk_information = OByteStreamView{
						package_data.forward_view(bs_size(information_structure.chunk_information))
					};
					information_data.archive_resource_information = OByteStreamView{
						package_data.forward_view(bs_size(information_structure.archive_resource_information))
					};
				}
				auto information_structure = Structure::Information<version>{};
				information_structure.archive_setting.version = cbw<Structure::VersionNumber>(version.number);
				information_structure.resource_file.allocate(package_manifest.resource.size());
				information_structure.resource_directory.allocate(package_manifest.resource.size() + 1_sz);
				information_structure.resource_information.allocate_full(package_manifest.resource.size());
				information_structure.chunk_information.allocate_full(k_none_size);
				information_structure.archive_resource_information.allocate_full(k_none_size);
				information_structure.resource_directory.append(""_sv);
				auto global_chunk_index = k_begin_index;
				for (auto & resource_index : SizeRange{package_manifest.resource.size()}) {
					auto & resource_manifest = package_manifest.resource.at(resource_index);
					auto & resource_information_structure = information_structure.resource_information[resource_index];
					auto   resource_path = resource_directory / resource_manifest.key;
					auto   resource_directory_string = resource_manifest.key.parent().to_string(CharacterType::PathSeparator::windows);
					information_structure.resource_file.append(resource_manifest.key.name());
					if (auto index = Range::find_index(information_structure.resource_directory, resource_directory_string)) {
						resource_information_structure.directory_index = cbw<IntegerU16>(index.get());
					} else {
						information_structure.resource_directory.append(resource_directory_string);
						resource_information_structure.directory_index = cbw<IntegerU16>(information_structure.resource_directory.last_index());
					}
					resource_information_structure.chunk_index.allocate_full(cbw<Size>(resource_manifest.value.chunk.size()));
					for (auto & chunk_index : SizeRange{resource_manifest.value.chunk.size()}) {
						resource_information_structure.chunk_index[chunk_index] = cbw<IntegerU16>(global_chunk_index);
						auto & chunk_manifest = resource_manifest.value.chunk.at(chunk_index);
						auto & chunk_information_structure = information_structure.chunk_information.append();
						auto   chunk_flag = BitSet<Structure::ChunkFlag<version>::k_count.value>{};
						switch (chunk_manifest.flag.hash().value) {
							default : {
								assert_failed(R"(chunk.flag == /* valid */)");
								break;
							}
							case "zlib"_shz : {
								chunk_flag.set(Structure::ChunkFlag<version>::zlib);
								break;
							}
							case "zerod_out"_shz : {
								chunk_flag.set(Structure::ChunkFlag<version>::zerod_out);
								break;
							}
							case "copy_coded"_shz : {
								chunk_flag.set(Structure::ChunkFlag<version>::copy_coded);
								break;
							}
							case "lzma"_shz : {
								chunk_flag.set(Structure::ChunkFlag<version>::lzma);
								break;
							}
						}
						auto chunk_size_uncompressed = Size{};
						auto chunk_size_compressed = Size{};
						chunk_information_structure.offset = cbw<IntegerU32>(package_data.position());
						if (chunk_flag.get(Structure::ChunkFlag<version>::combuf)) {
							throw ToDoException{};
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::unused_2)) {
							throw ToDoException{};
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::dzip)) {
							throw ToDoException{};
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::zlib)) {
							auto chunk_data = FileSystem::read_file(resource_path);
							Data::Compress::Deflate::Compress::process(as_lvalue(IByteStreamView{chunk_data}), package_data, 9_sz, 15_sz, 9_sz, Data::Compress::Deflate::Strategy::default_mode, Data::Compress::Deflate::Wrapper::gzip);
							package_data.backward(8_sz); // NOTE : overwrite gzip trail
							chunk_size_uncompressed = chunk_data.size();
							chunk_size_compressed = chunk_size_uncompressed;
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::bzip2)) {
							auto chunk_data = FileSystem::read_file(resource_path);
							Data::Compress::BZip2::Compress::process(as_lvalue(IByteStreamView{chunk_data}), package_data, 9_sz, 0_sz);
							chunk_size_uncompressed = chunk_data.size();
							chunk_size_compressed = chunk_size_uncompressed;
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::mp3)) {
							throw ToDoException{};
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::jpeg)) {
							throw ToDoException{};
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::zerod_out)) {
							throw ToDoException{};
							// TODO
							// chunk_size_uncompressed = ;
							// chunk_size_compressed = k_none_size;
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::copy_coded)) {
							chunk_size_uncompressed = FileSystem::read_stream_file(resource_path, package_data);
							chunk_size_compressed = chunk_size_uncompressed;
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::lzma)) {
							auto chunk_data = FileSystem::read_file(resource_path);
							auto chunk_header = Structure::ChunkHeaderLzma<version>{};
							auto chunk_header_data = OByteStreamView{package_data.forward_view(bs_size(chunk_header))};
							Data::Compress::Lzma::Compress::process(as_lvalue(IByteStreamView{chunk_data}), package_data, as_lvalue(OByteStreamView{chunk_header.props.view()}), 9_sz);
							chunk_header.size = cbw<IntegerU32>(chunk_data.size());
							chunk_header_data.write(chunk_header);
							chunk_size_uncompressed = chunk_data.size();
							chunk_size_compressed = chunk_size_uncompressed;
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::random_access)) {
							throw ToDoException{};
						}
						chunk_information_structure.size_compressed = cbw<IntegerU32>(chunk_size_compressed);
						chunk_information_structure.size_uncompressed = cbw<IntegerU32>(chunk_size_uncompressed);
						chunk_information_structure.flag = chunk_flag.to_integer();
						chunk_information_structure.file = 0_iu16;
						++global_chunk_index;
					}
				}
				information_structure.chunk_setting.archive_resource_count = 1_iu16;
				information_structure.chunk_setting.chunk_count = cbw<IntegerU16>(global_chunk_index);
				information_structure.archive_setting.resource_file_count = cbw<IntegerU16>(information_structure.resource_file.size());
				information_structure.archive_setting.resource_directory_count = cbw<IntegerU16>(information_structure.resource_directory.size());
				{
					information_data.archive_setting.write(information_structure.archive_setting);
					for (auto & element : information_structure.resource_file) {
						StringParser::write_string(self_cast<OCharacterStreamView>(information_data.resource_file), element);
						information_data.resource_file.write(CharacterType::k_null);
					}
					for (auto & element : information_structure.resource_directory.tail(information_structure.resource_directory.size() - 1_sz)) {
						StringParser::write_string(self_cast<OCharacterStreamView>(information_data.resource_directory), element);
						information_data.resource_directory.write(CharacterType::k_null);
					}
					information_data.resource_information.write(information_structure.resource_information);
					information_data.chunk_setting.write(information_structure.chunk_setting);
					information_data.chunk_information.write(information_structure.chunk_information);
					information_data.archive_resource_information.write(information_structure.archive_resource_information);
				}
				return;
			}

			// ----------------

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process (
				OByteStreamView &                  data_,
				Manifest::Package<version> const & manifest,
				Path const &                       resource_directory
			) -> Void {
				M_use_zps_of(data);
				process_package<version>(data, manifest, resource_directory);
				return;
			}

			inline auto process (
				OByteStreamView &                data_,
				Manifest::PackageVariant const & manifest,
				Path const &                     resource_directory,
				Version const &                  version
			) -> Void {
				M_use_zps_of(data);
				Generalization::execute<VersionEnum>(
					version,
					[&] <auto index, auto version> (ValuePackage<index, version>) {
						process<version>(data, manifest.get<Manifest::Package<version>>(), resource_directory);
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
				IByteStreamView &            package_data,
				Manifest::Package<version> & package_manifest,
				Optional<Path> const &       resource_directory
			) -> Void {
				assert_condition(package_data.read_of<Structure::MagicIdentifier>() == Structure::k_magic_identifier);
				auto information_structure = Structure::Information<version>{};
				{
					package_data.read(information_structure.archive_setting);
					information_structure.resource_file.allocate_full(cbw<Size>(information_structure.archive_setting.resource_file_count));
					assert_condition(information_structure.archive_setting.version == cbw<IntegerU8>(version.number));
					for (auto & element : information_structure.resource_file) {
						auto string = CStringView{};
						StringParser::read_string(self_cast<ICharacterStreamView>(package_data), string);
						package_data.forward(bs_size(CharacterType::k_null));
						element = string;
					}
					information_structure.resource_directory.allocate_full(cbw<Size>(information_structure.archive_setting.resource_directory_count));
					information_structure.resource_directory[1_ix] = ""_sv;
					for (auto & element : information_structure.resource_directory.tail(information_structure.resource_directory.size() - 1_sz)) {
						auto string = CStringView{};
						StringParser::read_string(self_cast<ICharacterStreamView>(package_data), string);
						package_data.forward(bs_size(CharacterType::k_null));
						element = string;
					}
					package_data.read(information_structure.resource_information, cbw<Size>(information_structure.archive_setting.resource_file_count));
					package_data.read(information_structure.chunk_setting);
					package_data.read(information_structure.chunk_information, cbw<Size>(information_structure.chunk_setting.chunk_count));
					package_data.read(information_structure.archive_resource_information, cbw<Size>(information_structure.chunk_setting.archive_resource_count) - 1_sz);
				}
				package_manifest.resource.allocate_full(information_structure.resource_information.size());
				auto package_data_end_position = package_data.position();
				for (auto & resource_index : SizeRange{information_structure.resource_information.size()}) {
					auto & resource_information_structure = information_structure.resource_information[resource_index];
					auto & resource_manifest = package_manifest.resource.at(resource_index);
					resource_manifest.key = Path{information_structure.resource_directory[cbw<Size>(resource_information_structure.directory_index)]} / information_structure.resource_file[resource_index];
					resource_manifest.value.chunk.allocate_full(resource_information_structure.chunk_index.size());
					auto chunk_data_list = Array<ByteArray>{resource_information_structure.chunk_index.size()};
					for (auto & chunk_index : SizeRange{resource_information_structure.chunk_index.size()}) {
						auto & chunk_information_structure = information_structure.chunk_information[cbw<Size>(resource_information_structure.chunk_index[chunk_index])];
						auto & chunk_manifest = resource_manifest.value.chunk[chunk_index];
						auto & chunk_data = chunk_data_list[chunk_index];
						package_data.set_position(cbw<Size>(chunk_information_structure.offset));
						chunk_data.allocate(cbw<Size>(chunk_information_structure.size_uncompressed));
						auto chunk_size_compressed = cbw<Size>(chunk_information_structure.size_compressed);
						auto chunk_flag = BitSet<Structure::ChunkFlag<version>::k_count.value>{};
						chunk_flag.from_integer(chunk_information_structure.flag);
						assert_condition(!chunk_flag.get(Structure::ChunkFlag<version>::unused_2));
						auto chunk_ok = k_false;
						if (chunk_flag.get(Structure::ChunkFlag<version>::combuf)) {
							assert_condition(!chunk_ok);
							chunk_ok = k_true;
							throw ToDoException{};
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::dzip)) {
							assert_condition(!chunk_ok);
							chunk_ok = k_true;
							throw ToDoException{};
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::zlib)) {
							assert_condition(!chunk_ok);
							chunk_ok = k_true;
							assert_condition(chunk_size_compressed == chunk_data.size());
							chunk_manifest.flag = "zlib"_s;
							auto chunk_stream = OByteStreamView{chunk_data};
							package_data.forward(10_sz); // NOTE : skip gzip header todo
							Data::Compress::Deflate::Uncompress::process(package_data, chunk_stream, 15_sz, Data::Compress::Deflate::Wrapper::none);
							assert_condition(chunk_stream.full());
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::bzip2)) {
							assert_condition(!chunk_ok);
							chunk_ok = k_true;
							assert_condition(chunk_size_compressed == chunk_data.size());
							chunk_manifest.flag = "bzip2"_s;
							auto chunk_stream = OByteStreamView{chunk_data};
							Data::Compress::BZip2::Uncompress::process(package_data, chunk_stream, k_false);
							assert_condition(chunk_stream.full());
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::mp3)) {
							assert_condition(!chunk_ok);
							chunk_ok = k_true;
							throw ToDoException{};
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::jpeg)) {
							assert_condition(!chunk_ok);
							chunk_ok = k_true;
							throw ToDoException{};
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::zerod_out)) {
							assert_condition(!chunk_ok);
							chunk_ok = k_true;
							assert_condition(chunk_size_compressed == k_none_size);
							chunk_manifest.flag = "zerod_out"_s;
							Range::assign(chunk_data, k_null_byte);
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::copy_coded)) {
							assert_condition(!chunk_ok);
							chunk_ok = k_true;
							assert_condition(chunk_size_compressed == chunk_data.size());
							chunk_manifest.flag = "copy_coded"_s;
							Range::assign_from(chunk_data, package_data.forward_view(chunk_data.size()));
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::lzma)) {
							assert_condition(!chunk_ok);
							chunk_ok = k_true;
							assert_condition(chunk_size_compressed == chunk_data.size());
							chunk_manifest.flag = "lzma"_s;
							auto chunk_stream = OByteStreamView{chunk_data};
							auto chunk_header = package_data.read_of<Structure::ChunkHeaderLzma<version>>();
							assert_condition(cbw<Size>(chunk_header.size) == chunk_data.size());
							Data::Compress::Lzma::Uncompress::process(package_data, chunk_stream, as_lvalue(IByteStreamView{chunk_header.props.view()}));
							assert_condition(chunk_stream.full());
						}
						if (chunk_flag.get(Structure::ChunkFlag<version>::random_access)) {
							assert_condition(!chunk_ok);
							chunk_ok = k_true;
							throw ToDoException{};
						}
						assert_condition(chunk_ok);
						if (package_data.position() > package_data_end_position) {
							package_data_end_position = package_data.position();
						}
					}
					assert_condition(!chunk_data_list.empty() && Range::all_of(chunk_data_list.tail(chunk_data_list.size() - 1_sz), [&] (auto & element) { return element == chunk_data_list.first(); }));
					if (resource_directory) {
						FileSystem::write_file(resource_directory.get() / resource_manifest.key, chunk_data_list.first());
					}
				}
				package_data.set_position(package_data_end_position);
				return;
			}

			// ----------------

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process (
				IByteStreamView &            data_,
				Manifest::Package<version> & manifest,
				Optional<Path> const &       resource_directory
			) -> Void {
				M_use_zps_of(data);
				process_package<version>(data, manifest, resource_directory);
				return;
			}

			inline auto process (
				IByteStreamView &          data_,
				Manifest::PackageVariant & manifest,
				Optional<Path> const &     resource_directory,
				Version const &            version
			) -> Void {
				M_use_zps_of(data);
				Generalization::execute<VersionEnum>(
					version,
					[&] <auto index, auto version> (ValuePackage<index, version>) {
						process<version>(data, manifest.set<Manifest::Package<version>>(), resource_directory);
					}
				);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

}
