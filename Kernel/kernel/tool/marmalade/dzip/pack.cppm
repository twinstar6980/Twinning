module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.marmalade.dzip.pack;
import twinning.kernel.utility;
import twinning.kernel.tool.marmalade.dzip.version;
import twinning.kernel.tool.marmalade.dzip.definition;
import twinning.kernel.tool.marmalade.dzip.structure;
import twinning.kernel.tool.marmalade.dzip.common;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.tool.data.compression.deflate.compress;
import twinning.kernel.tool.data.compression.bzip2.common;
import twinning.kernel.tool.data.compression.bzip2.compress;
import twinning.kernel.tool.data.compression.lzma.common;
import twinning.kernel.tool.data.compression.lzma.compress;

export namespace Twinning::Kernel::Tool::Marmalade::Dzip {

	template <auto t_version> requires (check_version(t_version, {}))
	struct Pack :
		Common<t_version> {

		using Common = Common<t_version>;

		using typename Common::Definition;

		// ----------------

		inline static auto process_package(
			OutputByteStreamView &      data,
			Definition::Package const & definition,
			Path const &                resource_directory
		) -> Void {
			data.write_constant(Structure::k_magic_marker);
			struct {
				OutputByteStreamView archive_setting{};
				OutputByteStreamView resource_file{};
				OutputByteStreamView resource_directory{};
				OutputByteStreamView resource_information{};
				OutputByteStreamView chunk_setting{};
				OutputByteStreamView chunk_information{};
				OutputByteStreamView archive_resource_information{};
			} information_data = {};
			{
				auto information_structure = Structure::Information<t_version>{};
				information_structure.resource_file.allocate(definition.resource.size());
				information_structure.resource_directory.allocate(definition.resource.size() + 1_sz);
				information_structure.resource_information.allocate_full(definition.resource.size());
				information_structure.chunk_information.allocate_full(k_none_size);
				information_structure.archive_resource_information.allocate_full(k_none_size);
				information_structure.resource_directory.append(""_sv);
				auto global_chunk_index = k_begin_index;
				for (auto & resource_index : SizeRange{definition.resource.size()}) {
					auto & resource_definition = definition.resource[resource_index];
					auto & resource_information_structure = information_structure.resource_information[resource_index];
					auto   resource_directory_string = resource_definition.path.parent().to_string(CharacterType::k_path_separator_windows);
					information_structure.resource_file.append(resource_definition.path.name());
					if (auto index = Range::find_index(information_structure.resource_directory, resource_directory_string); index.has()) {
					}
					else {
						information_structure.resource_directory.append(resource_directory_string);
					}
					resource_information_structure.chunk_index.allocate_full(cbox<Size>(resource_definition.chunk.size()));
					for (auto & chunk_index : SizeRange{resource_definition.chunk.size()}) {
						information_structure.chunk_information.append();
						++global_chunk_index;
					}
				}
				information_data.archive_setting = OutputByteStreamView{
					data.forward_view(bs_size(information_structure.archive_setting))
				};
				information_data.resource_file = OutputByteStreamView{
					data.forward_view(
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
				information_data.resource_directory = OutputByteStreamView{
					data.forward_view(
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
				information_data.resource_information = OutputByteStreamView{
					data.forward_view(bs_size(information_structure.resource_information))
				};
				information_data.chunk_setting = OutputByteStreamView{
					data.forward_view(bs_size(information_structure.chunk_setting))
				};
				information_data.chunk_information = OutputByteStreamView{
					data.forward_view(bs_size(information_structure.chunk_information))
				};
				information_data.archive_resource_information = OutputByteStreamView{
					data.forward_view(bs_size(information_structure.archive_resource_information))
				};
			}
			auto information_structure = Structure::Information<t_version>{};
			information_structure.archive_setting.version = cbox<Structure::VersionNumber>(t_version.number);
			information_structure.resource_file.allocate(definition.resource.size());
			information_structure.resource_directory.allocate(definition.resource.size() + 1_sz);
			information_structure.resource_information.allocate_full(definition.resource.size());
			information_structure.chunk_information.allocate_full(k_none_size);
			information_structure.archive_resource_information.allocate_full(k_none_size);
			information_structure.resource_directory.append(""_sv);
			auto global_chunk_index = k_begin_index;
			for (auto & resource_index : SizeRange{definition.resource.size()}) {
				auto & resource_definition = definition.resource[resource_index];
				auto & resource_information_structure = information_structure.resource_information[resource_index];
				auto   resource_path = resource_directory / resource_definition.path;
				auto   resource_directory_string = resource_definition.path.parent().to_string(CharacterType::k_path_separator_windows);
				information_structure.resource_file.append(resource_definition.path.name());
				if (auto index = Range::find_index(information_structure.resource_directory, resource_directory_string); index.has()) {
					resource_information_structure.directory_index = cbox<IntegerU16>(index.get());
				}
				else {
					information_structure.resource_directory.append(resource_directory_string);
					resource_information_structure.directory_index = cbox<IntegerU16>(information_structure.resource_directory.last_index());
				}
				resource_information_structure.chunk_index.allocate_full(cbox<Size>(resource_definition.chunk.size()));
				for (auto & chunk_index : SizeRange{resource_definition.chunk.size()}) {
					resource_information_structure.chunk_index[chunk_index] = cbox<IntegerU16>(global_chunk_index);
					auto & chunk_definition = resource_definition.chunk[chunk_index];
					auto & chunk_information_structure = information_structure.chunk_information.append();
					auto   chunk_flag = BitSet<Structure::ChunkFlag<t_version>::k_count>{};
					switch (chunk_definition.flag.hash().value) {
						case "zlib"_shz: {
							chunk_flag.set(Structure::ChunkFlag<t_version>::zlib);
							break;
						}
						case "zerod_out"_shz: {
							chunk_flag.set(Structure::ChunkFlag<t_version>::zerod_out);
							break;
						}
						case "copy_coded"_shz: {
							chunk_flag.set(Structure::ChunkFlag<t_version>::copy_coded);
							break;
						}
						case "lzma"_shz: {
							chunk_flag.set(Structure::ChunkFlag<t_version>::lzma);
							break;
						}
						default: {
							assert_fail(R"(chunk.flag == /* valid */)");
							break;
						}
					}
					auto chunk_size_uncompressed = Size{};
					auto chunk_size_compressed = Size{};
					chunk_information_structure.offset = cbox<IntegerU32>(data.position());
					if (chunk_flag.get(Structure::ChunkFlag<t_version>::combuf)) {
						throw UnimplementedException{};
					}
					if (chunk_flag.get(Structure::ChunkFlag<t_version>::unused_2)) {
						throw UnimplementedException{};
					}
					if (chunk_flag.get(Structure::ChunkFlag<t_version>::dzip)) {
						throw UnimplementedException{};
					}
					if (chunk_flag.get(Structure::ChunkFlag<t_version>::zlib)) {
						auto chunk_data = Storage::read_file(resource_path);
						Data::Compression::Deflate::Compress::process(as_left(InputByteStreamView{chunk_data}), data, 9_sz, 15_sz, 9_sz, Data::Compression::Deflate::Strategy::Constant::default_mode(), Data::Compression::Deflate::Wrapper::Constant::gzip());
						data.backward(8_sz); // NOTE: EXPLAIN: overwrite gzip trail
						chunk_size_uncompressed = chunk_data.size();
						chunk_size_compressed = chunk_size_uncompressed;
					}
					if (chunk_flag.get(Structure::ChunkFlag<t_version>::bzip2)) {
						auto chunk_data = Storage::read_file(resource_path);
						Data::Compression::Bzip2::Compress::process(as_left(InputByteStreamView{chunk_data}), data, 9_sz, 0_sz);
						chunk_size_uncompressed = chunk_data.size();
						chunk_size_compressed = chunk_size_uncompressed;
					}
					if (chunk_flag.get(Structure::ChunkFlag<t_version>::mp3)) {
						throw UnimplementedException{};
					}
					if (chunk_flag.get(Structure::ChunkFlag<t_version>::jpeg)) {
						throw UnimplementedException{};
					}
					if (chunk_flag.get(Structure::ChunkFlag<t_version>::zerod_out)) {
						throw UnimplementedException{};
						// TODO
						// chunk_size_uncompressed = ;
						// chunk_size_compressed = k_none_size;
					}
					if (chunk_flag.get(Structure::ChunkFlag<t_version>::copy_coded)) {
						chunk_size_uncompressed = Storage::read_file_stream(resource_path, data);
						chunk_size_compressed = chunk_size_uncompressed;
					}
					if (chunk_flag.get(Structure::ChunkFlag<t_version>::lzma)) {
						auto chunk_data = Storage::read_file(resource_path);
						Data::Compression::Lzma::Compress::process(as_left(InputByteStreamView{chunk_data}), data, 9_sz);
						chunk_size_uncompressed = chunk_data.size();
						chunk_size_compressed = chunk_size_uncompressed;
					}
					if (chunk_flag.get(Structure::ChunkFlag<t_version>::random_access)) {
						throw UnimplementedException{};
					}
					chunk_information_structure.size_compressed = cbox<IntegerU32>(chunk_size_compressed);
					chunk_information_structure.size_uncompressed = cbox<IntegerU32>(chunk_size_uncompressed);
					chunk_information_structure.flag = chunk_flag.to_integer();
					chunk_information_structure.file = 0_iu16;
					++global_chunk_index;
				}
			}
			information_structure.chunk_setting.archive_resource_count = 1_iu16;
			information_structure.chunk_setting.chunk_count = cbox<IntegerU16>(global_chunk_index);
			information_structure.archive_setting.resource_file_count = cbox<IntegerU16>(information_structure.resource_file.size());
			information_structure.archive_setting.resource_directory_count = cbox<IntegerU16>(information_structure.resource_directory.size());
			{
				information_data.archive_setting.write(information_structure.archive_setting);
				for (auto & element : information_structure.resource_file) {
					StringParser::write_string_until(self_cast<OutputCharacterStreamView>(information_data.resource_file), element, CharacterType::k_null);
					self_cast<OutputCharacterStreamView>(information_data.resource_file).write_constant(CharacterType::k_null);
				}
				for (auto & element : information_structure.resource_directory.tail(information_structure.resource_directory.size() - 1_sz)) {
					StringParser::write_string_until(self_cast<OutputCharacterStreamView>(information_data.resource_directory), element, CharacterType::k_null);
					self_cast<OutputCharacterStreamView>(information_data.resource_directory).write_constant(CharacterType::k_null);
				}
				information_data.resource_information.write(information_structure.resource_information);
				information_data.chunk_setting.write(information_structure.chunk_setting);
				information_data.chunk_information.write(information_structure.chunk_information);
				information_data.archive_resource_information.write(information_structure.archive_resource_information);
			}
			return;
		}

		// ----------------

		inline static auto process(
			OutputByteStreamView &      data_,
			Definition::Package const & definition,
			Path const &                resource_directory
		) -> Void {
			M_use_zps_of(data);
			return process_package(data, definition, resource_directory);
		}

	};

}
