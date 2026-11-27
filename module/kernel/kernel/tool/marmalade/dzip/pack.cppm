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
import twinning.kernel.tool.data.compression.lzma1.common;
import twinning.kernel.tool.data.compression.lzma1.compress;

export namespace Twinning::Kernel::Tool::Marmalade::Dzip {

	template <auto t_version> requires (check_version(t_version, {}))
	struct Pack :
		Common<t_version> {

		using Common = Common<t_version>;

		using typename Common::Definition;

		// ----------------

		inline static auto process_package(
			OutputBinaryStreamView &    data,
			Definition::Package const & definition,
			Path const &                resource_directory
		) -> Void {
			data.write_constant(Structure::k_magic_marker);
			struct {
				OutputBinaryStreamView archive_setting{};
				OutputBinaryStreamView resource_file{};
				OutputBinaryStreamView resource_directory{};
				OutputBinaryStreamView resource_information{};
				OutputBinaryStreamView chunk_setting{};
				OutputBinaryStreamView chunk_information{};
				OutputBinaryStreamView archive_resource_information{};
			} information_data = {};
			{
				auto information_structure = Structure::Information<t_version>{};
				information_structure.resource_file.allocate(definition.resource.size());
				information_structure.resource_directory.allocate(definition.resource.size() + 1_sz);
				information_structure.resource_information.allocate_full(definition.resource.size());
				information_structure.chunk_information.allocate_full(0_sz);
				information_structure.archive_resource_information.allocate_full(0_sz);
				information_structure.resource_directory.append(""_sv);
				auto global_chunk_index = 0_sz;
				for (auto & resource_index : SizeRange{definition.resource.size()}) {
					auto & resource_definition = definition.resource[resource_index];
					auto & resource_information_structure = information_structure.resource_information[resource_index];
					assert_test(resource_definition.path.type() == Storage::PathType::Constant::detached());
					auto resource_directory_string = resource_definition.path.parent().get().emit_windows();
					information_structure.resource_file.append(resource_definition.path.name().get());
					auto resource_directory_index_if = Range::find_index(information_structure.resource_directory, resource_directory_string);
					if (resource_directory_index_if.has()) {
					}
					else {
						information_structure.resource_directory.append(resource_directory_string);
					}
					resource_information_structure.chunk_index.allocate_full(cast_box<Size>(resource_definition.chunk.size()));
					for (auto & chunk_index : SizeRange{resource_definition.chunk.size()}) {
						information_structure.chunk_information.append();
						++global_chunk_index;
					}
				}
				information_data.archive_setting = data.split_forward(binary_size_of(information_structure.archive_setting));
				information_data.resource_file = data.split_forward(
					[&] {
						auto size = 0_sz;
						for (auto & element : information_structure.resource_file) {
							size += binary_size_of(element);
							size += binary_size_of(StringUtility::k_null_character);
						}
						return size;
					}()
				);
				information_data.resource_directory = data.split_forward(
					[&] {
						auto size = 0_sz;
						for (auto & element : Range::slice_tail(information_structure.resource_directory, information_structure.resource_directory.size() - 1_sz)) {
							size += binary_size_of(element);
							size += binary_size_of(StringUtility::k_null_character);
						}
						return size;
					}()
				);
				information_data.resource_information = data.split_forward(binary_size_of(information_structure.resource_information));
				information_data.chunk_setting = data.split_forward(binary_size_of(information_structure.chunk_setting));
				information_data.chunk_information = data.split_forward(binary_size_of(information_structure.chunk_information));
				information_data.archive_resource_information = data.split_forward(binary_size_of(information_structure.archive_resource_information));
			}
			auto information_structure = Structure::Information<t_version>{};
			information_structure.archive_setting.version = cast_box<Structure::VersionNumber>(t_version.number);
			information_structure.resource_file.allocate(definition.resource.size());
			information_structure.resource_directory.allocate(definition.resource.size() + 1_sz);
			information_structure.resource_information.allocate_full(definition.resource.size());
			information_structure.chunk_information.allocate_full(0_sz);
			information_structure.archive_resource_information.allocate_full(0_sz);
			information_structure.resource_directory.append(""_sv);
			auto global_chunk_index = 0_sz;
			auto chunk_data_container = Binary{};
			for (auto & resource_index : SizeRange{definition.resource.size()}) {
				auto & resource_definition = definition.resource[resource_index];
				auto & resource_information_structure = information_structure.resource_information[resource_index];
				auto   resource_path = resource_directory.push(resource_definition.path);
				auto   resource_directory_string = resource_definition.path.parent().get().emit_windows();
				information_structure.resource_file.append(resource_definition.path.name().get());
				auto resource_directory_index_if = Range::find_index(information_structure.resource_directory, resource_directory_string);
				if (resource_directory_index_if.has()) {
					resource_information_structure.directory_index = cast_box<IntegerU16>(resource_directory_index_if.get());
				}
				else {
					information_structure.resource_directory.append(resource_directory_string);
					resource_information_structure.directory_index = cast_box<IntegerU16>(information_structure.resource_directory.last_index());
				}
				resource_information_structure.chunk_index.allocate_full(cast_box<Size>(resource_definition.chunk.size()));
				for (auto & chunk_index : SizeRange{resource_definition.chunk.size()}) {
					resource_information_structure.chunk_index[chunk_index] = cast_box<IntegerU16>(global_chunk_index);
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
					chunk_information_structure.offset = cast_box<IntegerU32>(data.position());
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
						chunk_size_uncompressed = Storage::size_file(resource_path);
						auto chunk_data = use_binary_buffer(chunk_data_container, chunk_size_uncompressed);
						Storage::read_file(resource_path, 0_sz, chunk_data);
						auto chunk_stream = like_binary_stream<StreamMode::Constant::input()>(data, chunk_data);
						Data::Compression::Deflate::Compress::process(chunk_stream, data, Data::Compression::Deflate::Container::Constant::gzip(), 9_i, 15_i, 9_i, Data::Compression::Deflate::StrategyMode::Constant::default_mode());
						data.backward(8_sz); // NOTE: EXPLAIN: overwrite gzip trail
						chunk_size_compressed = chunk_size_uncompressed;
					}
					if (chunk_flag.get(Structure::ChunkFlag<t_version>::bzip2)) {
						chunk_size_uncompressed = Storage::size_file(resource_path);
						auto chunk_data = use_binary_buffer(chunk_data_container, chunk_size_uncompressed);
						Storage::read_file(resource_path, 0_sz, chunk_data);
						auto chunk_stream = like_binary_stream<StreamMode::Constant::input()>(data, chunk_data);
						Data::Compression::Bzip2::Compress::process(chunk_stream, data, Data::Compression::Bzip2::Container::Constant::none(), 9_i, 0_i);
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
						// chunk_size_compressed = 0_sz;
					}
					if (chunk_flag.get(Structure::ChunkFlag<t_version>::copy_coded)) {
						chunk_size_uncompressed = Storage::size_file(resource_path);
						auto chunk_data = data.split_forward(chunk_size_uncompressed).view();
						Storage::read_file(resource_path, 0_sz, chunk_data);
						chunk_size_compressed = chunk_size_uncompressed;
					}
					if (chunk_flag.get(Structure::ChunkFlag<t_version>::lzma)) {
						chunk_size_uncompressed = Storage::size_file(resource_path);
						auto chunk_data = use_binary_buffer(chunk_data_container, chunk_size_uncompressed);
						Storage::read_file(resource_path, 0_sz, chunk_data);
						auto chunk_stream = like_binary_stream<StreamMode::Constant::input()>(data, chunk_data);
						Data::Compression::Lzma1::Compress::process(chunk_stream, data, Data::Compression::Lzma1::Container::Constant::alone(), 9_i);
						chunk_size_compressed = chunk_size_uncompressed;
					}
					if (chunk_flag.get(Structure::ChunkFlag<t_version>::random_access)) {
						throw UnimplementedException{};
					}
					chunk_information_structure.size_compressed = cast_box<IntegerU32>(chunk_size_compressed);
					chunk_information_structure.size_uncompressed = cast_box<IntegerU32>(chunk_size_uncompressed);
					chunk_information_structure.flag = cast_box<IntegerU16>(chunk_flag.to_integer());
					chunk_information_structure.file = 0_iu16;
					++global_chunk_index;
				}
			}
			chunk_data_container.reset();
			information_structure.chunk_setting.archive_resource_count = 1_iu16;
			information_structure.chunk_setting.chunk_count = cast_box<IntegerU16>(global_chunk_index);
			information_structure.archive_setting.resource_file_count = cast_box<IntegerU16>(information_structure.resource_file.size());
			information_structure.archive_setting.resource_directory_count = cast_box<IntegerU16>(information_structure.resource_directory.size());
			{
				information_data.archive_setting.write(information_structure.archive_setting);
				for (auto & element : information_structure.resource_file) {
					StringUtility::write_string_until(unsafe_cast<OutputStringStreamView>(information_data.resource_file), element, StringUtility::k_null_character);
					unsafe_cast<OutputStringStreamView>(information_data.resource_file).write_constant(StringUtility::k_null_character);
				}
				for (auto & element : Range::slice_tail(information_structure.resource_directory, information_structure.resource_directory.size() - 1_sz)) {
					StringUtility::write_string_until(unsafe_cast<OutputStringStreamView>(information_data.resource_directory), element, StringUtility::k_null_character);
					unsafe_cast<OutputStringStreamView>(information_data.resource_directory).write_constant(StringUtility::k_null_character);
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
			OutputBinaryStreamView &    data_,
			Definition::Package const & definition,
			Path const &                resource_directory
		) -> Void {
			M_use_split_stream_of(data);
			return process_package(data, definition, resource_directory);
		}

	};

}
