#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/popcap/package/common.hpp"
#include "kernel/tool/data/compression/deflate/compress.hpp"

namespace TwinStar::Kernel::Tool::PopCap::Package {

	template <auto version> requires (check_version(version, {}, {}))
	struct Pack :
		Common<version> {

		using Common = Common<version>;

		using typename Common::Definition;

		// ----------------

		static auto process_package (
			OByteStreamView &                    data,
			typename Definition::Package const & definition,
			Path const &                         resource_directory
		) -> Void {
			data.write_constant(Structure::k_magic_identifier);
			data.write_constant(cbw<Structure::VersionNumber>(version.number));
			struct {
				OByteStreamView resource_information;
			}
				information_data = {};
			{
				auto information_structure = Structure::Information<version>{};
				information_structure.resource_information.allocate_full(definition.resource.size());
				for (auto & resource_index : SizeRange{definition.resource.size()}) {
					auto & resource_definition = definition.resource[resource_index];
					auto & resource_information_structure = information_structure.resource_information[resource_index];
					resource_information_structure.path = StringBlock8{resource_definition.path.to_string(CharacterType::PathSeparator::windows)};
				}
				information_data.resource_information = OByteStreamView{
					data.forward_view(
						[&] {
							auto size = k_none_size;
							for (auto & element : information_structure.resource_information) {
								size += bs_size(Structure::ResourceInformationListStateFlag<version>::next);
								size += bs_size(element);
							}
							size += bs_size(Structure::ResourceInformationListStateFlag<version>::done);
							return size;
						}()
					)
				};
			}
			auto information_structure = Structure::Information<version>{};
			information_structure.resource_information.allocate_full(definition.resource.size());
			for (auto & resource_index : SizeRange{definition.resource.size()}) {
				auto & resource_definition = definition.resource[resource_index];
				auto & resource_information_structure = information_structure.resource_information[resource_index];
				auto   resource_path = resource_directory / resource_definition.path;
				resource_information_structure.path = StringBlock8{resource_definition.path.to_string(CharacterType::PathSeparator::windows)};
				resource_information_structure.time = cbw<IntegerU64>(resource_definition.time);
				if constexpr (check_version(version, {}, {false})) {
					auto resource_size = FileSystem::read_stream_file(resource_path, data);
					resource_information_structure.size = cbw<IntegerU32>(resource_size);
				}
				if constexpr (check_version(version, {}, {true})) {
					auto resource_data = FileSystem::read_file(resource_path);
					auto resource_data_stream = IByteStreamView{resource_data};
					auto resource_offset = data.position();
					Data::Compression::Deflate::Compress::process(resource_data_stream, data, 9_sz, 15_sz, 9_sz, Data::Compression::Deflate::Strategy::Constant::default_mode(), Data::Compression::Deflate::Wrapper::Constant::zlib());
					resource_information_structure.size = cbw<IntegerU32>(data.position() - resource_offset);
					resource_information_structure.size_original = cbw<IntegerU32>(resource_data.size());
				}
			}
			{
				for (auto & element : information_structure.resource_information) {
					information_data.resource_information.write(Structure::ResourceInformationListStateFlag<version>::next);
					information_data.resource_information.write(element);
				}
				information_data.resource_information.write(Structure::ResourceInformationListStateFlag<version>::done);
			}
			return;
		}

		// ----------------

		static auto process (
			OByteStreamView &                    data_,
			typename Definition::Package const & definition,
			Path const &                         resource_directory
		) -> Void {
			M_use_zps_of(data);
			return process_package(data, definition, resource_directory);
		}

	};

}
