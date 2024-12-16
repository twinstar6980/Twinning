module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_bundle.unpack;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_bundle.version;
import twinning.kernel.tool.popcap.resource_stream_bundle.definition;
import twinning.kernel.tool.popcap.resource_stream_bundle.manifest;
import twinning.kernel.tool.popcap.resource_stream_bundle.structure;
import twinning.kernel.tool.popcap.resource_stream_bundle.shared;
import twinning.kernel.tool.popcap.resource_stream_bundle.common;
import twinning.kernel.tool.popcap.resource_stream_group.version;
import twinning.kernel.tool.popcap.resource_stream_group.definition;
import twinning.kernel.tool.popcap.resource_stream_group.structure;
import twinning.kernel.tool.popcap.resource_stream_group.common;
import twinning.kernel.tool.popcap.resource_stream_group.pack;
import twinning.kernel.tool.popcap.resource_stream_group.unpack;

export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamBundle {

	template <auto version> requires (check_version(version, {}, {}))
	struct Unpack :
		Common<version> {

		using Common = Common<version>;

		using typename Common::Definition;

		using typename Common::Manifest;

		using Common::k_suffix_of_composite_shell_upper;

		using Common::k_suffix_of_composite_shell;

		using Common::k_suffix_of_automation_pool;

		// ----------------

		inline static auto make_original_group_id_upper (
			CStringView const & standard_id,
			Boolean &           is_composite,
			String &            original_id
		) -> Void {
			is_composite = !Range::end_with(standard_id, k_suffix_of_composite_shell_upper);
			if (is_composite) {
				original_id = standard_id;
			}
			else {
				original_id = standard_id.head(standard_id.size() - k_suffix_of_composite_shell_upper.size());
			}
			return;
		}

		inline static auto make_original_group_id (
			CStringView const & standard_id,
			Boolean &           is_composite,
			String &            original_id
		) -> Void {
			is_composite = !Range::end_with(standard_id, k_suffix_of_composite_shell);
			if (is_composite) {
				original_id = standard_id;
			}
			else {
				original_id = standard_id.head(standard_id.size() - k_suffix_of_composite_shell.size());
			}
			return;
		}

		// ----------------

		inline static auto process_package_manifest (
			IByteStreamView &                  data,
			Structure::Header<version> const & header_structure,
			typename Manifest::Package &       manifest
		) -> Void {
			auto group_manifest_information_data = IByteStreamView{data.sub_view(cbox<Size>(header_structure.group_manifest_information_section_offset), cbox<Size>(header_structure.resource_manifest_information_section_offset - header_structure.group_manifest_information_section_offset))};
			auto resource_manifest_information_data = IByteStreamView{data.sub_view(cbox<Size>(header_structure.resource_manifest_information_section_offset), cbox<Size>(header_structure.string_manifest_information_section_offset - header_structure.resource_manifest_information_section_offset))};
			auto string_manifest_information_data = ICharacterStreamView{from_byte_view<Character, BasicCharacterListView>(data.sub_view(cbox<Size>(header_structure.string_manifest_information_section_offset), cbox<Size>(header_structure.information_section_size - header_structure.string_manifest_information_section_offset)))};
			auto get_string = [&] (
				IntegerU32 const & offset
			) -> CStringView {
				auto result = CStringView{};
				string_manifest_information_data.set_position(cbox<Size>(offset));
				StringParser::read_string_until(string_manifest_information_data, result, CharacterType::k_null);
				return result;
			};
			manifest.group.allocate_full(k_none_size);
			while (!group_manifest_information_data.full()) {
				auto   group_manifest_information_structure = group_manifest_information_data.read_of<Structure::GroupManifestInformation<version>>();
				auto & group_manifest = manifest.group.append();
				make_original_group_id(get_string(group_manifest_information_structure.id_offset), group_manifest.composite, group_manifest.id);
				group_manifest.subgroup.allocate_full(group_manifest_information_structure.subgroup_information.size());
				for (auto & subgroup_index : SizeRange{group_manifest_information_structure.subgroup_information.size()}) {
					auto & subgroup_manifest_information_structure = group_manifest_information_structure.subgroup_information[subgroup_index];
					auto & subgroup_manifest = group_manifest.subgroup[subgroup_index];
					subgroup_manifest.id = get_string(subgroup_manifest_information_structure.id_offset);
					if constexpr (check_version(version, {1}, {})) {
						if (subgroup_manifest_information_structure.resolution == 0x00000000_iu32) {
							subgroup_manifest.category.resolution.reset();
						}
						else {
							subgroup_manifest.category.resolution.set(cbox<Integer>(subgroup_manifest_information_structure.resolution));
						}
					}
					if constexpr (check_version(version, {3}, {})) {
						if (subgroup_manifest_information_structure.locale == 0x00000000_iu32) {
							subgroup_manifest.category.locale.reset();
						}
						else {
							subgroup_manifest.category.locale.set().from(fourcc_from_integer(subgroup_manifest_information_structure.locale));
						}
					}
					subgroup_manifest.resource.allocate_full(subgroup_manifest_information_structure.resource_information.size());
					for (auto & resource_index : SizeRange{subgroup_manifest_information_structure.resource_information.size()}) {
						auto & resource_manifest_information_structure = subgroup_manifest_information_structure.resource_information[resource_index];
						auto & resource_manifest = subgroup_manifest.resource[resource_index];
						resource_manifest_information_data.set_position(cbox<Size>(resource_manifest_information_structure.detail_offset));
						auto resource_detail_manifest_information_structure = resource_manifest_information_data.read_of<Structure::ResourceDetailManifestInformation<version>>();
						resource_manifest.id = get_string(resource_detail_manifest_information_structure.id_offset);
						resource_manifest.path = Path{String{get_string(resource_detail_manifest_information_structure.path_offset)}};
						resource_manifest.type = cbox<Integer>(resource_detail_manifest_information_structure.type);
						resource_manifest.property.allocate(resource_detail_manifest_information_structure.property_information.size() + (!resource_detail_manifest_information_structure.image_property_information.has() ? (0_sz) : (11_sz)));
						if (resource_detail_manifest_information_structure.image_property_information.has()) {
							auto & resource_image_property_detail_manifest_information_structure = resource_detail_manifest_information_structure.image_property_information.get();
							resource_manifest.property("type"_sv).from(cbox<Integer>(resource_image_property_detail_manifest_information_structure.type), k_true);
							resource_manifest.property("flag"_sv).from(cbox<Integer>(resource_image_property_detail_manifest_information_structure.flag), k_true);
							resource_manifest.property("x"_sv).from(cbox<Integer>(resource_image_property_detail_manifest_information_structure.x), k_true);
							resource_manifest.property("y"_sv).from(cbox<Integer>(resource_image_property_detail_manifest_information_structure.y), k_true);
							resource_manifest.property("ax"_sv).from(cbox<Integer>(resource_image_property_detail_manifest_information_structure.ax), k_true);
							resource_manifest.property("ay"_sv).from(cbox<Integer>(resource_image_property_detail_manifest_information_structure.ay), k_true);
							resource_manifest.property("aw"_sv).from(cbox<Integer>(resource_image_property_detail_manifest_information_structure.aw), k_true);
							resource_manifest.property("ah"_sv).from(cbox<Integer>(resource_image_property_detail_manifest_information_structure.ah), k_true);
							resource_manifest.property("rows"_sv).from(cbox<Integer>(resource_image_property_detail_manifest_information_structure.rows), k_true);
							resource_manifest.property("cols"_sv).from(cbox<Integer>(resource_image_property_detail_manifest_information_structure.cols), k_true);
							resource_manifest.property("parent"_sv) = get_string(resource_image_property_detail_manifest_information_structure.parent_offset);
						}
						for (auto & resource_property_index : SizeRange{resource_detail_manifest_information_structure.property_information.size()}) {
							auto & resource_property_detail_information_manifest_structure = resource_detail_manifest_information_structure.property_information[resource_property_index];
							auto & resource_property_manifest = resource_manifest.property.append();
							resource_property_manifest.key = get_string(resource_property_detail_information_manifest_structure.key_offset);
							resource_property_manifest.value = get_string(resource_property_detail_information_manifest_structure.value_offset);
						}
					}
				}
			}
			return;
		}

		inline static auto process_package (
			IByteStreamView &                      data,
			typename Definition::Package &         definition,
			Optional<typename Manifest::Package> & manifest,
			Optional<Path> const &                 resource_directory,
			Optional<Path> const &                 packet_file
		) -> Void {
			constexpr auto packet_version = ResourceStreamGroup::Version{.number = version.number};
			data.read_constant(Structure::k_magic_identifier);
			data.read_constant(cbox<Structure::VersionNumber>(version.number));
			auto information_structure = Structure::Information<version>{};
			{
				data.read(information_structure.header);
				if constexpr (check_version(version, {1, 3}, {})) {
					assert_test(information_structure.header.unknown_1 == 1_iu32);
				}
				if constexpr (check_version(version, {3}, {})) {
					assert_test(information_structure.header.unknown_1 == 0_iu32);
				}
				assert_test(cbox<Size>(information_structure.header.group_information_section_block_size) == bs_static_size<Structure::GroupInformation<version>>());
				assert_test(cbox<Size>(information_structure.header.subgroup_information_section_block_size) == bs_static_size<Structure::SubgroupInformation<version>>());
				assert_test(cbox<Size>(information_structure.header.pool_information_section_block_size) == bs_static_size<Structure::PoolInformation<version>>());
				assert_test(cbox<Size>(information_structure.header.texture_resource_information_section_block_size) == bs_static_size<Structure::TextureResourceInformation<version>>());
				CompiledMapData::decode(information_structure.group_id, as_lvalue(IByteStreamView{data.sub_view(cbox<Size>(information_structure.header.group_id_section_offset), cbox<Size>(information_structure.header.group_id_section_size))}));
				CompiledMapData::decode(information_structure.subgroup_id, as_lvalue(IByteStreamView{data.sub_view(cbox<Size>(information_structure.header.subgroup_id_section_offset), cbox<Size>(information_structure.header.subgroup_id_section_size))}));
				CompiledMapData::decode(information_structure.resource_path, as_lvalue(IByteStreamView{data.sub_view(cbox<Size>(information_structure.header.resource_path_section_offset), cbox<Size>(information_structure.header.resource_path_section_size))}));
				data.set_position(cbox<Size>(information_structure.header.group_information_section_offset));
				data.read(information_structure.group_information, cbox<Size>(information_structure.header.group_information_section_block_count));
				data.set_position(cbox<Size>(information_structure.header.subgroup_information_section_offset));
				data.read(information_structure.subgroup_information, cbox<Size>(information_structure.header.subgroup_information_section_block_count));
				data.set_position(cbox<Size>(information_structure.header.pool_information_section_offset));
				data.read(information_structure.pool_information, cbox<Size>(information_structure.header.pool_information_section_block_count));
				data.set_position(cbox<Size>(information_structure.header.texture_resource_information_section_offset));
				data.read(information_structure.texture_resource_information, cbox<Size>(information_structure.header.texture_resource_information_section_block_count));
				if (information_structure.header.group_manifest_information_section_offset != 0_iu32 || information_structure.header.resource_manifest_information_section_offset != 0_iu32 || information_structure.header.string_manifest_information_section_offset != 0_iu32) {
					assert_test(information_structure.header.group_manifest_information_section_offset != 0_iu32 && information_structure.header.resource_manifest_information_section_offset != 0_iu32 && information_structure.header.string_manifest_information_section_offset != 0_iu32);
					process_package_manifest(data, information_structure.header, manifest.set());
				}
				assert_test(information_structure.group_id.size() == cbox<Size>(information_structure.header.group_information_section_block_count));
				assert_test(information_structure.subgroup_id.size() == cbox<Size>(information_structure.header.subgroup_information_section_block_count));
			}
			auto group_id_list = Map<Size, String>{};
			auto subgroup_id_list = Map<Size, String>{};
			group_id_list.convert(
				information_structure.group_id,
				[] (auto & destination_element, auto & source_element) -> auto {
					destination_element.key = cbox<Size>(source_element.value);
					destination_element.value = source_element.key;
					return;
				}
			);
			subgroup_id_list.convert(
				information_structure.subgroup_id,
				[] (auto & destination_element, auto & source_element) -> auto {
					destination_element.key = cbox<Size>(source_element.value);
					destination_element.value = source_element.key;
					return;
				}
			);
			definition.group.allocate_full(information_structure.group_information.size());
			auto package_data_end_position = cbox<Size>(information_structure.header.information_section_size);
			for (auto & group_index : SizeRange{information_structure.group_information.size()}) {
				auto & group_information_structure = information_structure.group_information[group_index];
				auto & group_definition = definition.group[group_index];
				make_original_group_id_upper(group_id_list[group_index], group_definition.composite, group_definition.id);
				group_definition.subgroup.allocate_full(cbox<Size>(group_information_structure.subgroup_count));
				for (auto & subgroup_index : SizeRange{cbox<Size>(group_information_structure.subgroup_count)}) {
					auto & simple_subgroup_information_structure = group_information_structure.subgroup_information[subgroup_index];
					auto & subgroup_information_structure = information_structure.subgroup_information[cbox<Size>(simple_subgroup_information_structure.index)];
					auto & pool_information_structure = information_structure.pool_information[cbox<Size>(subgroup_information_structure.pool)];
					auto & subgroup_definition = group_definition.subgroup[subgroup_index];
					assert_test(subgroup_information_structure.general_resource_data_section_size_pool == subgroup_information_structure.general_resource_data_section_size_original);
					assert_test(subgroup_information_structure.texture_resource_data_section_size_pool == 0_iu32);
					assert_test(pool_information_structure.flag == 0_iu32);
					subgroup_definition.id = subgroup_id_list[cbox<Size>(simple_subgroup_information_structure.index)];
					if constexpr (check_version(version, {1}, {})) {
						if (simple_subgroup_information_structure.resolution == 0x00000000_iu32) {
							subgroup_definition.category.resolution.reset();
						}
						else {
							subgroup_definition.category.resolution.set(cbox<Integer>(simple_subgroup_information_structure.resolution));
						}
					}
					if constexpr (check_version(version, {3}, {})) {
						if (simple_subgroup_information_structure.locale == 0x00000000_iu32) {
							subgroup_definition.category.locale.reset();
						}
						else {
							subgroup_definition.category.locale.set().from(fourcc_from_integer(simple_subgroup_information_structure.locale));
						}
					}
					auto make_formatted_path = [&] (
						Path const & path_format
					) -> Path {
						return Path{format_string(path_format.to_string(), group_definition.id, subgroup_definition.id)};
					};
					auto packet_data = data.sub_view(cbox<Size>(subgroup_information_structure.offset), cbox<Size>(subgroup_information_structure.size));
					auto packet_stream = IByteStreamView{packet_data};
					auto packet_package_definition = typename ResourceStreamGroup::Definition<packet_version>::Package{};
					ResourceStreamGroup::Unpack<packet_version>::process(packet_stream, packet_package_definition, !resource_directory.has() ? (k_null_optional) : (make_optional_of(make_formatted_path(resource_directory.get()))));
					assert_test(packet_stream.full());
					if (packet_file.has()) {
						Storage::write_file(make_formatted_path(packet_file.get()), packet_data);
					}
					subgroup_definition.compression = packet_package_definition.compression;
					subgroup_definition.resource.allocate_full(packet_package_definition.resource.size());
					auto texture_resource_begin = Size{};
					auto texture_resource_count = Size{};
					if constexpr (check_version(version, {1, 3}, {})) {
						texture_resource_begin = cbox<Size>(pool_information_structure.texture_resource_begin);
						texture_resource_count = cbox<Size>(pool_information_structure.texture_resource_count);
					}
					if constexpr (check_version(version, {3}, {})) {
						texture_resource_begin = cbox<Size>(subgroup_information_structure.texture_resource_begin);
						texture_resource_count = cbox<Size>(subgroup_information_structure.texture_resource_count);
						assert_test(pool_information_structure.texture_resource_begin == 0_iu32);
						assert_test(pool_information_structure.texture_resource_count == 0_iu32);
					}
					for (auto & resource_index : SizeRange{packet_package_definition.resource.size()}) {
						auto & packet_resource_definition = packet_package_definition.resource[resource_index];
						auto & resource_definition = subgroup_definition.resource[resource_index];
						resource_definition.path = packet_resource_definition.path;
						switch (packet_resource_definition.additional.type().value) {
							case ResourceType::Constant::general().value : {
								auto & packet_resource_additional_definition = packet_resource_definition.additional.template get_of_type<ResourceType::Constant::general()>();
								auto & resource_additional_definition = resource_definition.additional.template set_of_type<ResourceType::Constant::general()>();
								break;
							}
							case ResourceType::Constant::texture().value : {
								auto & packet_resource_additional_definition = packet_resource_definition.additional.template get_of_type<ResourceType::Constant::texture()>();
								auto & resource_additional_definition = resource_definition.additional.template set_of_type<ResourceType::Constant::texture()>();
								auto & texture_information_structure = information_structure.texture_resource_information[texture_resource_begin + cbox<Size>(packet_resource_additional_definition.index)];
								assert_test(cbox<Integer>(texture_information_structure.size_width) == packet_resource_additional_definition.size.width);
								assert_test(cbox<Integer>(texture_information_structure.size_height) == packet_resource_additional_definition.size.height);
								resource_additional_definition.size = packet_resource_additional_definition.size;
								resource_additional_definition.format = cbox<Integer>(texture_information_structure.format);
								resource_additional_definition.pitch = cbox<Integer>(texture_information_structure.pitch);
								if constexpr (check_version(version, {4}, {1})) {
									resource_additional_definition.additional_byte_count = cbox<Integer>(texture_information_structure.additional_byte_count);
								}
								if constexpr (check_version(version, {4}, {2})) {
									resource_additional_definition.scale = cbox<Integer>(texture_information_structure.scale);
								}
								break;
							}
						}
					}
					package_data_end_position = maximum(package_data_end_position, cbox<Size>(subgroup_information_structure.offset + subgroup_information_structure.size));
				}
			}
			data.set_position(package_data_end_position);
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView &                      data_,
			typename Definition::Package &         definition,
			Optional<typename Manifest::Package> & manifest,
			Optional<Path> const &                 resource_directory,
			Optional<Path> const &                 packet_file
		) -> Void {
			M_use_zps_of(data);
			restruct(definition);
			restruct(manifest);
			return process_package(data, definition, manifest, resource_directory, packet_file);
		}

	};

}
