module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.package.unpack;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.package.version;
import twinning.kernel.tool.popcap.package.definition;
import twinning.kernel.tool.popcap.package.structure;
import twinning.kernel.tool.popcap.package.common;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.tool.data.compression.deflate.uncompress;

export namespace Twinning::Kernel::Tool::Popcap::Package {

	template <auto t_version> requires (check_version(t_version, {}, {}))
	struct Unpack :
		Common<t_version> {

		using Common = Common<t_version>;

		using typename Common::Definition;

		// ----------------

		inline static auto process_package(
			InputBinaryStreamView & data,
			Definition::Package &   definition,
			Optional<Path> const &  resource_directory
		) -> Void {
			data.read_constant(Structure::k_magic_marker);
			data.read_constant(cast_box<Structure::VersionNumber>(t_version.number));
			auto information_structure = Structure::Information<t_version>{};
			{
				information_structure.resource_information.allocate(0_sz);
				while (k_true) {
					auto flag = data.read_of<IntegerU8>();
					if (flag == Structure::ResourceInformationListStateFlag<t_version>::done) {
						break;
					}
					if (flag == Structure::ResourceInformationListStateFlag<t_version>::next) {
						information_structure.resource_information.append();
						data.read(information_structure.resource_information.last());
						continue;
					}
					assert_fail(R"(flag == /* valid */)");
				}
			}
			definition.resource.allocate_full(information_structure.resource_information.size());
			auto resource_data_container = Binary{};
			for (auto & resource_index : SizeRange{information_structure.resource_information.size()}) {
				auto & resource_information_structure = information_structure.resource_information[resource_index];
				auto & resource_definition = definition.resource[resource_index];
				resource_definition.path = Path{resource_information_structure.path.value};
				assert_test(resource_definition.path.type() == Storage::PathType::Constant::detached());
				resource_definition.time = cast_box<Integer>(resource_information_structure.time);
				auto resource_data = data.split_forward(cast_box<Size>(resource_information_structure.size));
				if constexpr (check_version(t_version, {}, {false})) {
					if (resource_directory.has()) {
						auto resource_path = resource_directory.get().push(resource_definition.path);
						if (!Storage::exist_file(resource_path)) {
							Storage::create_file(resource_path);
						}
						Storage::resize_file(resource_path, resource_data.view().size());
						Storage::write_file(resource_path, 0_sz, resource_data.view());
					}
				}
				if constexpr (check_version(t_version, {}, {true})) {
					auto resource_size_original = cast_box<Size>(resource_information_structure.size_original);
					auto resource_data_original = use_binary_buffer(resource_data_container, resource_size_original);
					auto resource_data_original_stream = like_binary_stream<StreamMode::Constant::output()>(data, resource_data_original);
					Data::Compression::Deflate::Uncompress::process(resource_data_original_stream, resource_data, Data::Compression::Deflate::Container::Constant::zlib(), 15_i);
					assert_test(resource_data.full() && resource_data_original_stream.full());
					if (resource_directory.has()) {
						auto resource_path = resource_directory.get().push(resource_definition.path);
						if (!Storage::exist_file(resource_path)) {
							Storage::create_file(resource_path);
						}
						Storage::resize_file(resource_path, resource_data_original.size());
						Storage::write_file(resource_path, 0_sz, resource_data_original);
					}
				}
			}
			resource_data_container.reset();
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView & data_,
			Definition::Package &   definition,
			Optional<Path> const &  resource_directory
		) -> Void {
			M_use_split_stream_of(data);
			restruct(definition);
			return process_package(data, definition, resource_directory);
		}

	};

}
