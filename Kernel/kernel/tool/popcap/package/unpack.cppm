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

export namespace Twinning::Kernel::Tool::PopCap::Package {

	template <auto version> requires (check_version(version, {}, {}))
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
			data.read_constant(cbox<Structure::VersionNumber>(version.number));
			auto information_structure = Structure::Information<version>{};
			{
				information_structure.resource_information.allocate(k_none_size);
				while (k_true) {
					auto flag = data.read_of<IntegerU8>();
					if (flag == Structure::ResourceInformationListStateFlag<version>::done) {
						break;
					}
					if (flag == Structure::ResourceInformationListStateFlag<version>::next) {
						information_structure.resource_information.append();
						data.read(information_structure.resource_information.last());
						continue;
					}
					assert_fail(R"(flag == /* valid */)");
				}
			}
			definition.resource.allocate_full(information_structure.resource_information.size());
			for (auto & resource_index : SizeRange{information_structure.resource_information.size()}) {
				auto & resource_information_structure = information_structure.resource_information[resource_index];
				auto & resource_definition = definition.resource[resource_index];
				resource_definition.path = Path{resource_information_structure.path.value};
				resource_definition.time = cbox<Integer>(resource_information_structure.time);
				auto resource_data = data.forward_view(cbox<Size>(resource_information_structure.size));
				if constexpr (check_version(version, {}, {false})) {
					if (resource_directory.has()) {
						Storage::write_file(resource_directory.get() / resource_definition.path, resource_data);
					}
				}
				if constexpr (check_version(version, {}, {true})) {
					auto resource_data_original = ByteArray{cbox<Size>(resource_information_structure.size_original)};
					auto resource_data_stream = IByteStreamView{resource_data};
					auto resource_data_original_stream = OByteStreamView{resource_data_original};
					Data::Compression::Deflate::Uncompress::process(resource_data_stream, resource_data_original_stream, 15_sz, Data::Compression::Deflate::Wrapper::Constant::zlib());
					assert_test(resource_data_stream.full() && resource_data_original_stream.full());
					if (resource_directory.has()) {
						Storage::write_file(resource_directory.get() / resource_definition.path, resource_data_original);
					}
				}
			}
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
