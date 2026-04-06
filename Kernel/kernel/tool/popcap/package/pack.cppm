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

export namespace Twinning::Kernel::Tool::Popcap::Package {

	template <auto t_version> requires (check_version(t_version, {}, {}))
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
			data.write_constant(cbox<Structure::VersionNumber>(t_version.number));
			struct {
				OutputByteStreamView resource_information{};
			} information_data = {};
			{
				auto information_structure = Structure::Information<t_version>{};
				information_structure.resource_information.allocate_full(definition.resource.size());
				for (auto & resource_index : SizeRange{definition.resource.size()}) {
					auto & resource_definition = definition.resource[resource_index];
					auto & resource_information_structure = information_structure.resource_information[resource_index];
					assert_test(resource_definition.path.type() == Storage::PathType::Constant::detached());
					resource_information_structure.path = StringBlock8{resource_definition.path.emit_windows()};
				}
				information_data.resource_information = OutputByteStreamView{
					data.forward_view(
						[&] {
							auto size = k_none_size;
							for (auto & element : information_structure.resource_information) {
								size += bs_size(Structure::ResourceInformationListStateFlag<t_version>::next);
								size += bs_size(element);
							}
							size += bs_size(Structure::ResourceInformationListStateFlag<t_version>::done);
							return size;
						}()
					)
				};
			}
			auto information_structure = Structure::Information<t_version>{};
			information_structure.resource_information.allocate_full(definition.resource.size());
			for (auto & resource_index : SizeRange{definition.resource.size()}) {
				auto & resource_definition = definition.resource[resource_index];
				auto & resource_information_structure = information_structure.resource_information[resource_index];
				auto   resource_path = resource_directory.push(resource_definition.path);
				resource_information_structure.path = StringBlock8{resource_definition.path.emit_windows()};
				resource_information_structure.time = cbox<IntegerU64>(resource_definition.time);
				if constexpr (check_version(t_version, {}, {false})) {
					auto resource_size = Storage::read_file_stream(resource_path, data);
					resource_information_structure.size = cbox<IntegerU32>(resource_size);
				}
				if constexpr (check_version(t_version, {}, {true})) {
					auto resource_data = Storage::read_file(resource_path);
					auto resource_data_stream = InputByteStreamView{resource_data};
					auto resource_offset = data.position();
					Data::Compression::Deflate::Compress::process(resource_data_stream, data, 9_sz, 15_sz, 9_sz, Data::Compression::Deflate::Strategy::Constant::default_mode(), Data::Compression::Deflate::Wrapper::Constant::zlib());
					resource_information_structure.size = cbox<IntegerU32>(data.position() - resource_offset);
					resource_information_structure.size_original = cbox<IntegerU32>(resource_data.size());
				}
			}
			{
				for (auto & element : information_structure.resource_information) {
					information_data.resource_information.write(Structure::ResourceInformationListStateFlag<t_version>::next);
					information_data.resource_information.write(element);
				}
				information_data.resource_information.write(Structure::ResourceInformationListStateFlag<t_version>::done);
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
