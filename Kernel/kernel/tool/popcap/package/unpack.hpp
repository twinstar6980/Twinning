#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/popcap/package/common.hpp"
#include "kernel/tool/data/compression/deflate/uncompress.hpp"

namespace TwinStar::Kernel::Tool::PopCap::Package {

	template <auto version> requires (check_version(version, {}, {}))
	struct Unpack :
		Common<version> {

		using Common = Common<version>;

		using typename Common::Definition;

		// ----------------

		static auto process_package (
			IByteStreamView &              data,
			typename Definition::Package & definition,
			Optional<Path> const &         resource_directory
		) -> Void {
			data.read_constant(Structure::k_magic_identifier);
			data.read_constant(cbw<Structure::VersionNumber>(version.number));
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
				auto & resource_definition = definition.resource.at(resource_index);
				resource_definition.key = Path{resource_information_structure.path.value};
				resource_definition.value.time = cbw<Integer>(resource_information_structure.time);
				auto resource_data = data.forward_view(cbw<Size>(resource_information_structure.size));
				if constexpr (check_version(version, {}, {false})) {
					if (resource_directory.has()) {
						FileSystem::write_file(resource_directory.get() / resource_definition.key, resource_data);
					}
				}
				if constexpr (check_version(version, {}, {true})) {
					auto resource_data_original = ByteArray{cbw<Size>(resource_information_structure.size_original)};
					auto resource_data_stream = IByteStreamView{resource_data};
					auto resource_data_original_stream = OByteStreamView{resource_data_original};
					Data::Compression::Deflate::Uncompress::process(resource_data_stream, resource_data_original_stream, 15_sz, Data::Compression::Deflate::Wrapper::Constant::zlib());
					assert_test(resource_data_stream.full() && resource_data_original_stream.full());
					if (resource_directory.has()) {
						FileSystem::write_file(resource_directory.get() / resource_definition.key, resource_data_original);
					}
				}
			}
			return;
		}

		// ----------------

		static auto process (
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
