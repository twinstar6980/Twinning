#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/resource_stream_bundle/common.hpp"
#include "core/tool/popcap/resource_stream_group/pack.hpp"
#include "core/tool/popcap/resource_stream_group/unpack.hpp"
#include <unordered_map>

namespace TwinStar::Core::Tool::PopCap::ResourceStreamBundle {

	template <auto version> requires (check_version(version, {}, {}))
	struct Unpack :
		Common<version> {

		using Common = Common<version>;

		using typename Common::Definition;

		using typename Common::Description;

		using Common::k_suffix_of_composite_shell_upper;

		using Common::k_suffix_of_composite_shell;

		using Common::k_suffix_of_automation_pool;

		// ----------------

		static auto make_original_group_id_upper (
			CStringView const & standard_id,
			Boolean &           is_composite,
			String &            original_id
		) -> Void {
			is_composite = !Range::end_with(standard_id, k_suffix_of_composite_shell_upper);
			if (is_composite) {
				original_id = standard_id;
			} else {
				original_id = standard_id.head(standard_id.size() - k_suffix_of_composite_shell_upper.size());
			}
			return;
		}

		static auto make_original_group_id (
			CStringView const & standard_id,
			Boolean &           is_composite,
			String &            original_id
		) -> Void {
			is_composite = !Range::end_with(standard_id, k_suffix_of_composite_shell);
			if (is_composite) {
				original_id = standard_id;
			} else {
				original_id = standard_id.head(standard_id.size() - k_suffix_of_composite_shell.size());
			}
			return;
		}

		// ----------------

		static auto process_package_description (
			IByteStreamView &                  data,
			Structure::Header<version> const & header_structure,
			typename Description::Package &    description
		) -> Void {
			auto group_information_data = IByteStreamView{data.sub_view(cbw<Size>(header_structure.description_group_section_offset), cbw<Size>(header_structure.description_resource_section_offset - header_structure.description_group_section_offset))};
			auto resource_information_data = IByteStreamView{data.sub_view(cbw<Size>(header_structure.description_resource_section_offset), cbw<Size>(header_structure.description_string_section_offset - header_structure.description_resource_section_offset))};
			auto string_information_data = ICharacterStreamView{from_byte_view<Character, BasicCharacterListView>(data.sub_view(cbw<Size>(header_structure.description_string_section_offset), cbw<Size>(header_structure.information_section_size - header_structure.description_string_section_offset)))};
			auto get_string =
				[&] (
				IntegerU32 const & offset
			) -> auto {
				auto result = CStringView{};
				string_information_data.set_position(cbw<Size>(offset));
				StringParser::read_string_until(string_information_data, result, CharacterType::k_null);
				return result;
			};
			description.group.allocate_full(k_none_size);
			while (!group_information_data.full()) {
				auto   group_description_information_structure = group_information_data.read_of<Structure::GroupDescriptionInformation<version>>();
				auto & group_description = description.group.append();
				make_original_group_id(get_string(group_description_information_structure.id_offset), group_description.value.composite, group_description.key);
				group_description.value.subgroup.allocate_full(group_description_information_structure.subgroup_information.size());
				for (auto & subgroup_index : SizeRange{group_description_information_structure.subgroup_information.size()}) {
					auto & subgroup_description_information_structure = group_description_information_structure.subgroup_information[subgroup_index];
					auto & subgroup_description = group_description.value.subgroup.at(subgroup_index);
					subgroup_description.key = get_string(subgroup_description_information_structure.id_offset);
					if constexpr (check_version(version, {1}, {})) {
						if (subgroup_description_information_structure.resolution == 0x00000000_iu32) {
							subgroup_description.value.category.resolution.reset();
						} else {
							subgroup_description.value.category.resolution.set(cbw<Integer>(subgroup_description_information_structure.resolution));
						}
					}
					if constexpr (check_version(version, {3}, {})) {
						if (subgroup_description_information_structure.locale == 0x00000000_iu32) {
							subgroup_description.value.category.locale.reset();
						} else {
							subgroup_description.value.category.locale.set().from(fourcc_from_integer(subgroup_description_information_structure.locale));
						}
					}
					subgroup_description.value.resource.allocate_full(subgroup_description_information_structure.resource_information.size());
					for (auto & resource_index : SizeRange{subgroup_description_information_structure.resource_information.size()}) {
						auto & resource_description_information_structure = subgroup_description_information_structure.resource_information[resource_index];
						auto & resource_description = subgroup_description.value.resource.at(resource_index);
						resource_information_data.set_position(cbw<Size>(resource_description_information_structure.detail_offset));
						auto resource_detail_description_information_structure = resource_information_data.read_of<Structure::ResourceDetailDescriptionInformation<version>>();
						resource_description.key = get_string(resource_detail_description_information_structure.id_offset);
						resource_description.value.path = Path{String{get_string(resource_detail_description_information_structure.path_offset)}};
						resource_description.value.type = cbw<Integer>(resource_detail_description_information_structure.type);
						resource_description.value.property.allocate(resource_detail_description_information_structure.property_information.size() + (!resource_detail_description_information_structure.image_property_information.has() ? (0_sz) : (11_sz)));
						if (resource_detail_description_information_structure.image_property_information.has()) {
							auto & resource_image_property_detail_description_information_structure = resource_detail_description_information_structure.image_property_information.get();
							resource_description.value.property("type"_sv).from(cbw<Integer>(resource_image_property_detail_description_information_structure.type), k_true);
							resource_description.value.property("flag"_sv).from(cbw<Integer>(resource_image_property_detail_description_information_structure.flag), k_true);
							resource_description.value.property("x"_sv).from(cbw<Integer>(resource_image_property_detail_description_information_structure.x), k_true);
							resource_description.value.property("y"_sv).from(cbw<Integer>(resource_image_property_detail_description_information_structure.y), k_true);
							resource_description.value.property("ax"_sv).from(cbw<Integer>(resource_image_property_detail_description_information_structure.ax), k_true);
							resource_description.value.property("ay"_sv).from(cbw<Integer>(resource_image_property_detail_description_information_structure.ay), k_true);
							resource_description.value.property("aw"_sv).from(cbw<Integer>(resource_image_property_detail_description_information_structure.aw), k_true);
							resource_description.value.property("ah"_sv).from(cbw<Integer>(resource_image_property_detail_description_information_structure.ah), k_true);
							resource_description.value.property("rows"_sv).from(cbw<Integer>(resource_image_property_detail_description_information_structure.rows), k_true);
							resource_description.value.property("cols"_sv).from(cbw<Integer>(resource_image_property_detail_description_information_structure.cols), k_true);
							resource_description.value.property("parent"_sv) = get_string(resource_image_property_detail_description_information_structure.parent_offset);
						}
						for (auto & resource_property_index : SizeRange{resource_detail_description_information_structure.property_information.size()}) {
							auto & resource_property_detail_information_description_structure = resource_detail_description_information_structure.property_information[resource_property_index];
							auto & resource_property_description = resource_description.value.property.append();
							resource_property_description.key = get_string(resource_property_detail_information_description_structure.key_offset);
							resource_property_description.value = get_string(resource_property_detail_information_description_structure.value_offset);
						}
					}
				}
			}
			return;
		}

		static auto process_package (
			IByteStreamView &                         data,
			typename Definition::Package &            definition,
			Optional<typename Description::Package> & description,
			Optional<Path> const &                    resource_directory,
			Optional<Path> const &                    packet_file
		) -> Void {
			constexpr auto packet_version = ResourceStreamGroup::Version{.number = version.number};
			data.read_constant(Structure::k_magic_identifier);
			data.read_constant(cbw<Structure::VersionNumber>(version.number));
			auto information_structure = Structure::Information<version>{};
			{
				data.read(information_structure.header);
				if constexpr (check_version(version, {1, 3}, {})) {
					assert_test(information_structure.header.unknown_1 == 1_iu32);
				}
				if constexpr (check_version(version, {3}, {})) {
					assert_test(information_structure.header.unknown_1 == 0_iu32);
				}
				assert_test(cbw<Size>(information_structure.header.group_information_section_block_size) == bs_static_size<Structure::GroupInformation<version>>());
				assert_test(cbw<Size>(information_structure.header.subgroup_information_section_block_size) == bs_static_size<Structure::SubgroupInformation<version>>());
				assert_test(cbw<Size>(information_structure.header.subgroup_pool_information_section_block_size) == bs_static_size<Structure::SubgroupPoolInformation<version>>());
				assert_test(cbw<Size>(information_structure.header.texture_resource_information_section_block_size) == bs_static_size<Structure::TextureResourceInformation<version>>());
				CompiledMapData::decode(information_structure.group_id, as_lvalue(IByteStreamView{data.sub_view(cbw<Size>(information_structure.header.group_id_section_offset), cbw<Size>(information_structure.header.group_id_section_size))}));
				CompiledMapData::decode(information_structure.subgroup_id, as_lvalue(IByteStreamView{data.sub_view(cbw<Size>(information_structure.header.subgroup_id_section_offset), cbw<Size>(information_structure.header.subgroup_id_section_size))}));
				CompiledMapData::decode(information_structure.resource_path, as_lvalue(IByteStreamView{data.sub_view(cbw<Size>(information_structure.header.resource_path_section_offset), cbw<Size>(information_structure.header.resource_path_section_size))}));
				data.set_position(cbw<Size>(information_structure.header.group_information_section_offset));
				data.read(information_structure.group_information, cbw<Size>(information_structure.header.group_information_section_block_count));
				data.set_position(cbw<Size>(information_structure.header.subgroup_information_section_offset));
				data.read(information_structure.subgroup_information, cbw<Size>(information_structure.header.subgroup_information_section_block_count));
				data.set_position(cbw<Size>(information_structure.header.subgroup_pool_information_section_offset));
				data.read(information_structure.subgroup_pool_information, cbw<Size>(information_structure.header.subgroup_pool_information_section_block_count));
				data.set_position(cbw<Size>(information_structure.header.texture_resource_information_section_offset));
				data.read(information_structure.texture_resource_information, cbw<Size>(information_structure.header.texture_resource_information_section_block_count));
				if (information_structure.header.description_group_section_offset != 0_iu32 || information_structure.header.description_resource_section_offset != 0_iu32 || information_structure.header.description_string_section_offset != 0_iu32) {
					assert_test(information_structure.header.description_group_section_offset != 0_iu32 && information_structure.header.description_resource_section_offset != 0_iu32 && information_structure.header.description_string_section_offset != 0_iu32);
					process_package_description(data, information_structure.header, description.set());
				}
				assert_test(information_structure.group_id.size() == cbw<Size>(information_structure.header.group_information_section_block_count));
				assert_test(information_structure.subgroup_id.size() == cbw<Size>(information_structure.header.subgroup_information_section_block_count));
			}
			auto group_id_list = Map<Size, String>{};
			auto subgroup_id_list = Map<Size, String>{};
			group_id_list.convert(
				information_structure.group_id,
				[] (auto & destination_element, auto & source_element) {
					destination_element.key = cbw<Size>(source_element.value);
					destination_element.value = source_element.key;
				}
			);
			subgroup_id_list.convert(
				information_structure.subgroup_id,
				[] (auto & destination_element, auto & source_element) {
					destination_element.key = cbw<Size>(source_element.value);
					destination_element.value = source_element.key;
				}
			);
			definition.group.allocate_full(information_structure.group_information.size());
			auto package_data_end_position = cbw<Size>(information_structure.header.information_section_size);
			for (auto & group_index : SizeRange{information_structure.group_information.size()}) {
				auto & group_information_structure = information_structure.group_information[group_index];
				auto & group_definition = definition.group.at(group_index);
				make_original_group_id_upper(group_id_list[group_index], group_definition.value.composite, group_definition.key);
				group_definition.value.subgroup.allocate_full(cbw<Size>(group_information_structure.subgroup_count));
				for (auto & subgroup_index : SizeRange{cbw<Size>(group_information_structure.subgroup_count)}) {
					auto & simple_subgroup_information_structure = group_information_structure.subgroup_information[subgroup_index];
					auto & subgroup_information_structure = information_structure.subgroup_information[cbw<Size>(simple_subgroup_information_structure.index)];
					auto & subgroup_pool_information_structure = information_structure.subgroup_pool_information[cbw<Size>(subgroup_information_structure.pool)];
					auto & subgroup_definition = group_definition.value.subgroup.at(subgroup_index);
					subgroup_definition.key = subgroup_id_list[cbw<Size>(simple_subgroup_information_structure.index)];
					if constexpr (check_version(version, {1}, {})) {
						if (simple_subgroup_information_structure.resolution == 0x00000000_iu32) {
							subgroup_definition.value.category.resolution.reset();
						} else {
							subgroup_definition.value.category.resolution.set(cbw<Integer>(simple_subgroup_information_structure.resolution));
						}
					}
					if constexpr (check_version(version, {3}, {})) {
						if (simple_subgroup_information_structure.locale == 0x00000000_iu32) {
							subgroup_definition.value.category.locale.reset();
						} else {
							subgroup_definition.value.category.locale.set().from(fourcc_from_integer(simple_subgroup_information_structure.locale));
						}
					}
					auto make_formatted_path =
						[&] (
						Path const & path_format
					) -> auto {
						return Path{format_string(path_format.to_string(), group_definition.key, subgroup_definition.key)};
					};
					auto packet_data = data.sub_view(cbw<Size>(subgroup_information_structure.offset), cbw<Size>(subgroup_information_structure.size));
					auto packet_stream = IByteStreamView{packet_data};
					auto packet_package_definition = typename ResourceStreamGroup::Definition<packet_version>::Package{};
					ResourceStreamGroup::Unpack<packet_version>::process(packet_stream, packet_package_definition, !resource_directory.has() ? (k_null_optional) : (make_optional_of(make_formatted_path(resource_directory.get()))));
					assert_test(packet_stream.full());
					if (packet_file.has()) {
						FileSystem::write_file(make_formatted_path(packet_file.get()), packet_data);
					}
					subgroup_definition.value.resource_data_section_store_mode = packet_package_definition.resource_data_section_store_mode;
					subgroup_definition.value.resource.allocate_full(packet_package_definition.resource.size());
					auto texture_resource_begin = Size{};
					auto texture_resource_count = Size{};
					if constexpr (check_version(version, {1, 3}, {})) {
						texture_resource_begin = cbw<Size>(subgroup_pool_information_structure.texture_resource_begin);
						texture_resource_count = cbw<Size>(subgroup_pool_information_structure.texture_resource_count);
					}
					if constexpr (check_version(version, {3}, {})) {
						texture_resource_begin = cbw<Size>(subgroup_information_structure.texture_resource_begin);
						texture_resource_count = cbw<Size>(subgroup_information_structure.texture_resource_count);
						assert_test(subgroup_pool_information_structure.texture_resource_begin == 0_iu32);
						assert_test(subgroup_pool_information_structure.texture_resource_count == 0_iu32);
					}
					for (auto & resource_index : SizeRange{packet_package_definition.resource.size()}) {
						auto & packet_resource_definition = packet_package_definition.resource.at(resource_index);
						auto & resource_definition = subgroup_definition.value.resource.at(resource_index);
						resource_definition.key = packet_resource_definition.key;
						switch (packet_resource_definition.value.additional.type().value) {
							case ResourceType::Constant::generic().value : {
								auto & packet_resource_additional_definition = packet_resource_definition.value.additional.template get_of_type<ResourceType::Constant::generic()>();
								auto & resource_additional_definition = resource_definition.value.additional.template set_of_type<ResourceType::Constant::generic()>();
								break;
							}
							case ResourceType::Constant::texture().value : {
								auto & packet_resource_additional_definition = packet_resource_definition.value.additional.template get_of_type<ResourceType::Constant::texture()>();
								auto & resource_additional_definition = resource_definition.value.additional.template set_of_type<ResourceType::Constant::texture()>();
								auto & texture_information_structure = information_structure.texture_resource_information[texture_resource_begin + cbw<Size>(packet_resource_additional_definition.index)];
								assert_test(cbw<Integer>(texture_information_structure.size_width) == packet_resource_additional_definition.size.width);
								assert_test(cbw<Integer>(texture_information_structure.size_height) == packet_resource_additional_definition.size.height);
								resource_additional_definition.size = packet_resource_additional_definition.size;
								resource_additional_definition.format = cbw<Integer>(texture_information_structure.format);
								resource_additional_definition.pitch = cbw<Integer>(texture_information_structure.pitch);
								if constexpr (check_version(version, {4}, {1})) {
									resource_additional_definition.additional_byte_count = cbw<Integer>(texture_information_structure.additional_byte_count);
								}
								if constexpr (check_version(version, {4}, {2})) {
									resource_additional_definition.scale = cbw<Integer>(texture_information_structure.scale);
								}
								break;
							}
						}
					}
					package_data_end_position = maximum(package_data_end_position, cbw<Size>(subgroup_information_structure.offset + subgroup_information_structure.size));
				}
			}
			data.set_position(package_data_end_position);
			return;
		}

		// ----------------

		static auto process (
			IByteStreamView &                         data_,
			typename Definition::Package &            definition,
			Optional<typename Description::Package> & description,
			Optional<Path> const &                    resource_directory,
			Optional<Path> const &                    packet_file
		) -> Void {
			M_use_zps_of(data);
			restruct(definition);
			restruct(description);
			return process_package(data, definition, description, resource_directory, packet_file);
		}

	};

}
