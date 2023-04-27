#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/marmalade/dzip/version.hpp"
#include "core/tool/marmalade/dzip/manifest.hpp"
#include "core/tool/marmalade/dzip/structure.hpp"
#include "core/tool/data/compression/deflate.hpp"
#include "core/tool/data/compression/bzip2.hpp"
#include "core/tool/data/compression/lzma.hpp"

namespace TwinStar::Core::Tool::Marmalade::DZip {

	template <auto version> requires (check_version(version, {}))
	struct PackCommon {

	protected:

		using Manifest = Manifest<version>;

	};

	template <auto version> requires (check_version(version, {}))
	struct Pack :
		PackCommon<version> {

	protected:

		using Common = PackCommon<version>;

		using typename Common::Manifest;

		// ----------------

		static auto process_package (
			OByteStreamView &                  package_data,
			typename Manifest::Package const & package_manifest,
			Path const &                       resource_directory
		) -> Void {
			package_data.write_constant(Structure::k_magic_identifier);
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
					if (auto index = Range::find_index(information_structure.resource_directory, resource_directory_string); index.has()) {
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
				if (auto index = Range::find_index(information_structure.resource_directory, resource_directory_string); index.has()) {
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
					auto   chunk_flag = BitSet<Structure::ChunkFlag<version>::k_count>{};
					switch (chunk_manifest.flag.hash().value) {
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
						default : {
							assert_fail(R"(chunk.flag == /* valid */)");
						}
					}
					auto chunk_size_uncompressed = Size{};
					auto chunk_size_compressed = Size{};
					chunk_information_structure.offset = cbw<IntegerU32>(package_data.position());
					if (chunk_flag.get(Structure::ChunkFlag<version>::combuf)) {
						throw IncompleteException{};
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::unused_2)) {
						throw IncompleteException{};
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::dzip)) {
						throw IncompleteException{};
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::zlib)) {
						auto chunk_data = FileSystem::read_file(resource_path);
						Data::Compression::Deflate::Compress::do_process_whole(as_lvalue(IByteStreamView{chunk_data}), package_data, 9_sz, 15_sz, 9_sz, Data::Compression::Deflate::Strategy::Constant::default_mode(), Data::Compression::Deflate::Wrapper::Constant::gzip());
						package_data.backward(8_sz); // NOTE : overwrite gzip trail
						chunk_size_uncompressed = chunk_data.size();
						chunk_size_compressed = chunk_size_uncompressed;
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::bzip2)) {
						auto chunk_data = FileSystem::read_file(resource_path);
						Data::Compression::BZip2::Compress::do_process_whole(as_lvalue(IByteStreamView{chunk_data}), package_data, 9_sz, 0_sz);
						chunk_size_uncompressed = chunk_data.size();
						chunk_size_compressed = chunk_size_uncompressed;
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::mp3)) {
						throw IncompleteException{};
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::jpeg)) {
						throw IncompleteException{};
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::zerod_out)) {
						throw IncompleteException{};
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
						Data::Compression::Lzma::Compress::do_process_whole(as_lvalue(IByteStreamView{chunk_data}), package_data, 9_sz);
						chunk_size_uncompressed = chunk_data.size();
						chunk_size_compressed = chunk_size_uncompressed;
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::random_access)) {
						throw IncompleteException{};
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
					StringParser::write_string_until(self_cast<OCharacterStreamView>(information_data.resource_file), element, CharacterType::k_null);
					self_cast<OCharacterStreamView>(information_data.resource_file).write_constant(CharacterType::k_null);
				}
				for (auto & element : information_structure.resource_directory.tail(information_structure.resource_directory.size() - 1_sz)) {
					StringParser::write_string_until(self_cast<OCharacterStreamView>(information_data.resource_directory), element, CharacterType::k_null);
					self_cast<OCharacterStreamView>(information_data.resource_directory).write_constant(CharacterType::k_null);
				}
				information_data.resource_information.write(information_structure.resource_information);
				information_data.chunk_setting.write(information_structure.chunk_setting);
				information_data.chunk_information.write(information_structure.chunk_information);
				information_data.archive_resource_information.write(information_structure.archive_resource_information);
			}
			return;
		}

	public:

		static auto do_process_package (
			OByteStreamView &                  package_data_,
			typename Manifest::Package const & package_manifest,
			Path const &                       resource_directory
		) -> Void {
			M_use_zps_of(package_data);
			return process_package(package_data, package_manifest, resource_directory);
		}

	};

	template <auto version> requires (check_version(version, {}))
	struct Unpack :
		PackCommon<version> {

	protected:

		using Common = PackCommon<version>;

		using typename Common::Manifest;

		// ----------------

		static auto process_package (
			IByteStreamView &            package_data,
			typename Manifest::Package & package_manifest,
			Optional<Path> const &       resource_directory
		) -> Void {
			package_data.read_constant(Structure::k_magic_identifier);
			auto information_structure = Structure::Information<version>{};
			{
				package_data.read(information_structure.archive_setting);
				information_structure.resource_file.allocate_full(cbw<Size>(information_structure.archive_setting.resource_file_count));
				assert_test(information_structure.archive_setting.version == cbw<Structure::VersionNumber>(version.number));
				for (auto & element : information_structure.resource_file) {
					auto string = CStringView{};
					StringParser::read_string_until(self_cast<ICharacterStreamView>(package_data), string, CharacterType::k_null);
					self_cast<ICharacterStreamView>(package_data).read_constant(CharacterType::k_null);
					element = string;
				}
				information_structure.resource_directory.allocate_full(cbw<Size>(information_structure.archive_setting.resource_directory_count));
				information_structure.resource_directory[1_ix] = ""_sv;
				for (auto & element : information_structure.resource_directory.tail(information_structure.resource_directory.size() - 1_sz)) {
					auto string = CStringView{};
					StringParser::read_string_until(self_cast<ICharacterStreamView>(package_data), string, CharacterType::k_null);
					self_cast<ICharacterStreamView>(package_data).read_constant(CharacterType::k_null);
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
					auto chunk_flag = BitSet<Structure::ChunkFlag<version>::k_count>{};
					chunk_flag.from_integer(chunk_information_structure.flag);
					assert_test(!chunk_flag.get(Structure::ChunkFlag<version>::unused_2));
					auto chunk_ok = k_false;
					if (chunk_flag.get(Structure::ChunkFlag<version>::combuf)) {
						assert_test(!chunk_ok);
						chunk_ok = k_true;
						throw IncompleteException{};
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::dzip)) {
						assert_test(!chunk_ok);
						chunk_ok = k_true;
						throw IncompleteException{};
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::zlib)) {
						assert_test(!chunk_ok);
						chunk_ok = k_true;
						assert_test(chunk_size_compressed == chunk_data.size());
						chunk_manifest.flag = "zlib"_s;
						auto chunk_stream = OByteStreamView{chunk_data};
						package_data.forward(10_sz); // TODO NOTE : skip gzip header
						Data::Compression::Deflate::Uncompress::do_process_whole(package_data, chunk_stream, 15_sz, Data::Compression::Deflate::Wrapper::Constant::none());
						assert_test(chunk_stream.full());
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::bzip2)) {
						assert_test(!chunk_ok);
						chunk_ok = k_true;
						assert_test(chunk_size_compressed == chunk_data.size());
						chunk_manifest.flag = "bzip2"_s;
						auto chunk_stream = OByteStreamView{chunk_data};
						Data::Compression::BZip2::Uncompress::do_process_whole(package_data, chunk_stream, k_false);
						assert_test(chunk_stream.full());
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::mp3)) {
						assert_test(!chunk_ok);
						chunk_ok = k_true;
						throw IncompleteException{};
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::jpeg)) {
						assert_test(!chunk_ok);
						chunk_ok = k_true;
						throw IncompleteException{};
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::zerod_out)) {
						assert_test(!chunk_ok);
						chunk_ok = k_true;
						assert_test(chunk_size_compressed == k_none_size);
						chunk_manifest.flag = "zerod_out"_s;
						Range::assign(chunk_data, k_null_byte);
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::copy_coded)) {
						assert_test(!chunk_ok);
						chunk_ok = k_true;
						assert_test(chunk_size_compressed == chunk_data.size());
						chunk_manifest.flag = "copy_coded"_s;
						Range::assign_from(chunk_data, package_data.forward_view(chunk_data.size()));
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::lzma)) {
						assert_test(!chunk_ok);
						chunk_ok = k_true;
						assert_test(chunk_size_compressed == chunk_data.size());
						chunk_manifest.flag = "lzma"_s;
						auto chunk_stream = OByteStreamView{chunk_data};
						Data::Compression::Lzma::Uncompress::do_process_whole(package_data, chunk_stream);
						assert_test(chunk_stream.full());
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::random_access)) {
						assert_test(!chunk_ok);
						chunk_ok = k_true;
						throw IncompleteException{};
					}
					assert_test(chunk_ok);
					package_data_end_position = maximum(package_data_end_position, package_data.position());
				}
				assert_test(!chunk_data_list.empty() && Range::all_of(chunk_data_list.tail(chunk_data_list.size() - 1_sz), [&] (auto & element) { return element == chunk_data_list.first(); }));
				if (resource_directory.has()) {
					FileSystem::write_file(resource_directory.get() / resource_manifest.key, chunk_data_list.first());
				}
			}
			package_data.set_position(package_data_end_position);
			return;
		}

	public:

		static auto do_process_package (
			IByteStreamView &            package_data_,
			typename Manifest::Package & package_manifest,
			Optional<Path> const &       resource_directory
		) -> Void {
			M_use_zps_of(package_data);
			restruct(package_manifest);
			return process_package(package_data, package_manifest, resource_directory);
		}

	};

}
