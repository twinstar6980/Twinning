#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/resource_stream_group/common.hpp"
#include "core/tool/data/compression/deflate/uncompress.hpp"

namespace TwinStar::Core::Tool::PopCap::ResourceStreamGroup {

	template <auto version> requires (check_version(version, {}))
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
				data.read(information_structure.header);
				if constexpr (check_version(version, {1, 3})) {
					assert_test(information_structure.header.unknown_1 == 1_iu32);
				}
				if constexpr (check_version(version, {3})) {
					assert_test(information_structure.header.unknown_1 == 0_iu32);
				}
				CompiledMapData::decode(information_structure.resource_information, as_lvalue(IByteStreamView{data.sub_view(cbw<Size>(information_structure.header.resource_information_section_offset), cbw<Size>(information_structure.header.resource_information_section_size))}));
			}
			definition.resource_data_section_store_mode = resource_data_section_store_mode_from_data(information_structure.header.resource_data_section_store_mode);
			definition.resource.allocate_full(information_structure.resource_information.size());
			for (auto & current_resource_type : make_static_array<ResourceType>(ResourceType::Constant::generic(), ResourceType::Constant::texture())) {
				auto resource_data_section_view = CByteListView{};
				auto resource_data_section_container = ByteArray{};
				auto resource_data_section_offset = Size{};
				auto resource_data_section_size = Size{};
				auto resource_data_section_size_original = Size{};
				auto compress_resource_data_section = k_false;
				switch (current_resource_type.value) {
					case ResourceType::Constant::generic().value : {
						resource_data_section_offset = cbw<Size>(information_structure.header.generic_resource_data_section_offset);
						resource_data_section_size = cbw<Size>(information_structure.header.generic_resource_data_section_size);
						resource_data_section_size_original = cbw<Size>(information_structure.header.generic_resource_data_section_size_original);
						compress_resource_data_section = definition.resource_data_section_store_mode.compress_generic;
						break;
					}
					case ResourceType::Constant::texture().value : {
						resource_data_section_offset = cbw<Size>(information_structure.header.texture_resource_data_section_offset);
						resource_data_section_size = cbw<Size>(information_structure.header.texture_resource_data_section_size);
						resource_data_section_size_original = cbw<Size>(information_structure.header.texture_resource_data_section_size_original);
						compress_resource_data_section = definition.resource_data_section_store_mode.compress_texture;
						break;
					}
				}
				auto resource_data_section_view_stored = data.sub_view(resource_data_section_offset, resource_data_section_size);
				if (!compress_resource_data_section) {
					resource_data_section_view = resource_data_section_view_stored;
				} else {
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
					auto & resource_definition = definition.resource.at(resource_index);
					resource_definition.key.from_string(resource_information_structure.key);
					switch (resource_information_structure.value.additional.type().value) {
						case ResourceType::Constant::generic().value : {
							auto & resource_additional_information_structure = resource_information_structure.value.additional.template get_of_type<ResourceType::Constant::generic()>();
							auto & resource_additional_definition = resource_definition.value.additional.template set_of_type<ResourceType::Constant::generic()>();
							break;
						}
						case ResourceType::Constant::texture().value : {
							auto & resource_additional_information_structure = resource_information_structure.value.additional.template get_of_type<ResourceType::Constant::texture()>();
							auto & resource_additional_definition = resource_definition.value.additional.template set_of_type<ResourceType::Constant::texture()>();
							resource_additional_definition.index = cbw<Integer>(resource_additional_information_structure.index);
							resource_additional_definition.size.width = cbw<Integer>(resource_additional_information_structure.size_width);
							resource_additional_definition.size.height = cbw<Integer>(resource_additional_information_structure.size_height);
							break;
						}
					}
					auto resource_data = resource_data_section_view.sub(cbw<Size>(resource_information_structure.value.offset), cbw<Size>(resource_information_structure.value.size));
					if (resource_directory.has()) {
						FileSystem::write_file(resource_directory.get() / resource_definition.key, resource_data);
					}
				}
			}
			data.set_position(
				cbw<Size>(
					maximum(
						make_initializer_list(
							{
								information_structure.header.information_section_size,
								information_structure.header.generic_resource_data_section_offset + information_structure.header.generic_resource_data_section_size,
								information_structure.header.texture_resource_data_section_offset + information_structure.header.texture_resource_data_section_size,
							}
						)
					)
				)
			);
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
