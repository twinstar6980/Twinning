module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.marmalade.dzip.unpack;
import twinning.kernel.utility;
import twinning.kernel.tool.marmalade.dzip.version;
import twinning.kernel.tool.marmalade.dzip.definition;
import twinning.kernel.tool.marmalade.dzip.structure;
import twinning.kernel.tool.marmalade.dzip.common;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.tool.data.compression.deflate.uncompress;
import twinning.kernel.tool.data.compression.bzip2.common;
import twinning.kernel.tool.data.compression.bzip2.uncompress;
import twinning.kernel.tool.data.compression.lzma.common;
import twinning.kernel.tool.data.compression.lzma.uncompress;

export namespace Twinning::Kernel::Tool::Marmalade::DZip {

	template <auto version> requires (check_version(version, {}))
	struct Unpack :
		Common<version> {

		using Common = Common<version>;

		using typename Common::Definition;

		// ----------------

		inline static auto process_package (
			IByteStreamView &              data,
			typename Definition::Package & definition,
			Optional<Path> const &         resource_directory
		) -> Void {
			data.read_constant(Structure::k_magic_identifier);
			auto information_structure = Structure::Information<version>{};
			{
				data.read(information_structure.archive_setting);
				information_structure.resource_file.allocate_full(cbox<Size>(information_structure.archive_setting.resource_file_count));
				assert_test(information_structure.archive_setting.version == cbox<Structure::VersionNumber>(version.number));
				for (auto & element : information_structure.resource_file) {
					auto string = CStringView{};
					StringParser::read_string_until(self_cast<ICharacterStreamView>(data), string, CharacterType::k_null);
					self_cast<ICharacterStreamView>(data).read_constant(CharacterType::k_null);
					element = string;
				}
				information_structure.resource_directory.allocate_full(cbox<Size>(information_structure.archive_setting.resource_directory_count));
				information_structure.resource_directory[1_ix] = ""_sv;
				for (auto & element : information_structure.resource_directory.tail(information_structure.resource_directory.size() - 1_sz)) {
					auto string = CStringView{};
					StringParser::read_string_until(self_cast<ICharacterStreamView>(data), string, CharacterType::k_null);
					self_cast<ICharacterStreamView>(data).read_constant(CharacterType::k_null);
					element = string;
				}
				data.read(information_structure.resource_information, cbox<Size>(information_structure.archive_setting.resource_file_count));
				data.read(information_structure.chunk_setting);
				data.read(information_structure.chunk_information, cbox<Size>(information_structure.chunk_setting.chunk_count));
				data.read(information_structure.archive_resource_information, cbox<Size>(information_structure.chunk_setting.archive_resource_count) - 1_sz);
			}
			definition.resource.allocate_full(information_structure.resource_information.size());
			auto package_data_end_position = data.position();
			for (auto & resource_index : SizeRange{information_structure.resource_information.size()}) {
				auto & resource_information_structure = information_structure.resource_information[resource_index];
				auto & resource_definition = definition.resource[resource_index];
				resource_definition.path = Path{information_structure.resource_directory[cbox<Size>(resource_information_structure.directory_index)]} / information_structure.resource_file[resource_index];
				resource_definition.chunk.allocate_full(resource_information_structure.chunk_index.size());
				auto chunk_data_list = Array<ByteArray>{resource_information_structure.chunk_index.size()};
				for (auto & chunk_index : SizeRange{resource_information_structure.chunk_index.size()}) {
					auto & chunk_information_structure = information_structure.chunk_information[cbox<Size>(resource_information_structure.chunk_index[chunk_index])];
					auto & chunk_definition = resource_definition.chunk[chunk_index];
					auto & chunk_data = chunk_data_list[chunk_index];
					data.set_position(cbox<Size>(chunk_information_structure.offset));
					chunk_data.allocate(cbox<Size>(chunk_information_structure.size_uncompressed));
					auto chunk_size_compressed = cbox<Size>(chunk_information_structure.size_compressed);
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
						chunk_definition.flag = "zlib"_s;
						auto chunk_stream = OByteStreamView{chunk_data};
						data.forward(10_sz); // TODO NOTE : skip gzip header
						Data::Compression::Deflate::Uncompress::process(data, chunk_stream, 15_sz, Data::Compression::Deflate::Wrapper::Constant::none());
						assert_test(chunk_stream.full());
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::bzip2)) {
						assert_test(!chunk_ok);
						chunk_ok = k_true;
						assert_test(chunk_size_compressed == chunk_data.size());
						chunk_definition.flag = "bzip2"_s;
						auto chunk_stream = OByteStreamView{chunk_data};
						Data::Compression::BZip2::Uncompress::process(data, chunk_stream, k_false);
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
						chunk_definition.flag = "zerod_out"_s;
						Range::assign(chunk_data, k_null_byte);
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::copy_coded)) {
						assert_test(!chunk_ok);
						chunk_ok = k_true;
						assert_test(chunk_size_compressed == chunk_data.size());
						chunk_definition.flag = "copy_coded"_s;
						Range::assign_from(chunk_data, data.forward_view(chunk_data.size()));
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::lzma)) {
						assert_test(!chunk_ok);
						chunk_ok = k_true;
						assert_test(chunk_size_compressed == chunk_data.size());
						chunk_definition.flag = "lzma"_s;
						auto chunk_stream = OByteStreamView{chunk_data};
						Data::Compression::Lzma::Uncompress::process(data, chunk_stream);
						assert_test(chunk_stream.full());
					}
					if (chunk_flag.get(Structure::ChunkFlag<version>::random_access)) {
						assert_test(!chunk_ok);
						chunk_ok = k_true;
						throw IncompleteException{};
					}
					assert_test(chunk_ok);
					package_data_end_position = maximum(package_data_end_position, data.position());
				}
				assert_test(!chunk_data_list.empty() && Range::all_of(chunk_data_list.tail(chunk_data_list.size() - 1_sz), [&] (auto & element) -> auto { return element == chunk_data_list.first(); }));
				if (resource_directory.has()) {
					Storage::write_file(resource_directory.get() / resource_definition.path, chunk_data_list.first());
				}
			}
			data.set_position(package_data_end_position);
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView &              data_,
			typename Definition::Package & definition,
			Optional<Path> const &         resource_directory
		) -> Void {
			M_use_zps_of(data);
			restruct(definition);
			return process_package(data, definition, resource_directory);
		}

	};

}
