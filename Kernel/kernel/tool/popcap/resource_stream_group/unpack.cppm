module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_group.unpack;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_group.version;
import twinning.kernel.tool.popcap.resource_stream_group.definition;
import twinning.kernel.tool.popcap.resource_stream_group.structure;
import twinning.kernel.tool.popcap.resource_stream_group.common;
import twinning.kernel.tool.popcap.resource_stream_bundle.shared;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.tool.data.compression.deflate.uncompress;

export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamGroup {

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
			data.read_constant(cbox<Structure::VersionNumber>(version.number));
			auto information_structure = Structure::Information<version>{};
			{
				data.read(information_structure.header);
				if constexpr (check_version(version, {1, 3})) {
					assert_test(information_structure.header.unknown_1 == 1_iu32);
				}
				if constexpr (check_version(version, {3})) {
					assert_test(information_structure.header.unknown_1 == 0_iu32);
				}
				CompiledMapData::decode(information_structure.resource_information, as_lvalue(IByteStreamView{data.sub_view(cbox<Size>(information_structure.header.resource_information_section_offset), cbox<Size>(information_structure.header.resource_information_section_size))}));
			}
			definition.compression = packet_compression_from_data(information_structure.header.resource_data_section_compression);
			definition.resource.allocate_full(information_structure.resource_information.size());
			for (auto & current_resource_type : make_static_array<ResourceType>(ResourceType::Constant::general(), ResourceType::Constant::texture())) {
				auto resource_data_section_view = CByteListView{};
				auto resource_data_section_container = ByteArray{};
				auto resource_data_section_offset = Size{};
				auto resource_data_section_size = Size{};
				auto resource_data_section_size_original = Size{};
				auto compress_resource_data_section = k_false;
				switch (current_resource_type.value) {
					case ResourceType::Constant::general().value : {
						resource_data_section_offset = cbox<Size>(information_structure.header.general_resource_data_section_offset);
						resource_data_section_size = cbox<Size>(information_structure.header.general_resource_data_section_size);
						resource_data_section_size_original = cbox<Size>(information_structure.header.general_resource_data_section_size_original);
						compress_resource_data_section = definition.compression.general;
						break;
					}
					case ResourceType::Constant::texture().value : {
						resource_data_section_offset = cbox<Size>(information_structure.header.texture_resource_data_section_offset);
						resource_data_section_size = cbox<Size>(information_structure.header.texture_resource_data_section_size);
						resource_data_section_size_original = cbox<Size>(information_structure.header.texture_resource_data_section_size_original);
						compress_resource_data_section = definition.compression.texture;
						break;
					}
				}
				auto resource_data_section_view_stored = data.sub_view(resource_data_section_offset, resource_data_section_size);
				if (!compress_resource_data_section) {
					resource_data_section_view = resource_data_section_view_stored;
				}
				else {
					resource_data_section_container.allocate(resource_data_section_size_original);
					if (resource_data_section_size_original != k_none_size) {
						auto resource_data_section_stored_stream = IByteStreamView{resource_data_section_view_stored};
						auto resource_data_section_original_stream = OByteStreamView{resource_data_section_container};
						Data::Compression::Deflate::Uncompress::process(resource_data_section_stored_stream, resource_data_section_original_stream, 15_sz, Data::Compression::Deflate::Wrapper::Constant::zlib());
					}
					resource_data_section_view = resource_data_section_container.view();
				}
				for (auto & resource_index : SizeRange{information_structure.resource_information.size()}) {
					auto & resource_information_structure = information_structure.resource_information.at(resource_index);
					if (resource_information_structure.value.additional.type() != current_resource_type) {
						continue;
					}
					auto & resource_definition = definition.resource[resource_index];
					resource_definition.path.from_string(resource_information_structure.key);
					switch (resource_information_structure.value.additional.type().value) {
						case ResourceType::Constant::general().value : {
							auto & resource_additional_information_structure = resource_information_structure.value.additional.template get_of_type<ResourceType::Constant::general()>();
							auto & resource_additional_definition = resource_definition.additional.template set_of_type<ResourceType::Constant::general()>();
							break;
						}
						case ResourceType::Constant::texture().value : {
							auto & resource_additional_information_structure = resource_information_structure.value.additional.template get_of_type<ResourceType::Constant::texture()>();
							auto & resource_additional_definition = resource_definition.additional.template set_of_type<ResourceType::Constant::texture()>();
							resource_additional_definition.index = cbox<Integer>(resource_additional_information_structure.index);
							resource_additional_definition.size.width = cbox<Integer>(resource_additional_information_structure.size_width);
							resource_additional_definition.size.height = cbox<Integer>(resource_additional_information_structure.size_height);
							break;
						}
					}
					auto resource_data = resource_data_section_view.sub(cbox<Size>(resource_information_structure.value.offset), cbox<Size>(resource_information_structure.value.size));
					if (resource_directory.has()) {
						Storage::write_file(resource_directory.get() / resource_definition.path, resource_data);
					}
				}
			}
			data.set_position(
				cbox<Size>(
					maximum(
						make_initializer_list(
							{
								information_structure.header.information_section_size,
								information_structure.header.general_resource_data_section_offset + information_structure.header.general_resource_data_section_size,
								information_structure.header.texture_resource_data_section_offset + information_structure.header.texture_resource_data_section_size,
							}
						)
					)
				)
			);
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
