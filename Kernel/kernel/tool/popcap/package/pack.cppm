module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.package.pack;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.package.version;
import twinning.kernel.tool.popcap.package.definition;
import twinning.kernel.tool.popcap.package.structure;
import twinning.kernel.tool.popcap.package.common;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.tool.data.compression.deflate.compress;

export namespace Twinning::Kernel::Tool::PopCap::Package {

	template <auto version> requires (check_version(version, {}, {}))
	struct Pack :
		Common<version> {

		using Common = Common<version>;

		using typename Common::Definition;

		// ----------------

		inline static auto process_package (
			OByteStreamView &                    data,
			typename Definition::Package const & definition,
			Path const &                         resource_directory
		) -> Void {
			data.write_constant(Structure::k_magic_identifier);
			data.write_constant(cbox<Structure::VersionNumber>(version.number));
			struct {
				OByteStreamView resource_information{};
			} information_data = {};
			{
				auto information_structure = Structure::Information<version>{};
				information_structure.resource_information.allocate_full(definition.resource.size());
				for (auto & resource_index : SizeRange{definition.resource.size()}) {
					auto & resource_definition = definition.resource[resource_index];
					auto & resource_information_structure = information_structure.resource_information[resource_index];
					resource_information_structure.path = StringBlock8{resource_definition.path.to_string(CharacterType::k_path_separator_windows)};
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
				resource_information_structure.path = StringBlock8{resource_definition.path.to_string(CharacterType::k_path_separator_windows)};
				resource_information_structure.time = cbox<IntegerU64>(resource_definition.time);
				if constexpr (check_version(version, {}, {false})) {
					auto resource_size = Storage::read_stream_file(resource_path, data);
					resource_information_structure.size = cbox<IntegerU32>(resource_size);
				}
				if constexpr (check_version(version, {}, {true})) {
					auto resource_data = Storage::read_file(resource_path);
					auto resource_data_stream = IByteStreamView{resource_data};
					auto resource_offset = data.position();
					Data::Compression::Deflate::Compress::process(resource_data_stream, data, 9_sz, 15_sz, 9_sz, Data::Compression::Deflate::Strategy::Constant::default_mode(), Data::Compression::Deflate::Wrapper::Constant::zlib());
					resource_information_structure.size = cbox<IntegerU32>(data.position() - resource_offset);
					resource_information_structure.size_original = cbox<IntegerU32>(resource_data.size());
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

		inline static auto process (
			OByteStreamView &                    data_,
			typename Definition::Package const & definition,
			Path const &                         resource_directory
		) -> Void {
			M_use_zps_of(data);
			return process_package(data, definition, resource_directory);
		}

	};

}
