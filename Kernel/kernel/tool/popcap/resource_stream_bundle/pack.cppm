module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_bundle.pack;
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
	struct Pack :
		Common<version> {

		using Common = Common<version>;

		using typename Common::Definition;

		using typename Common::Manifest;

		using Common::k_suffix_of_composite_shell_upper;

		using Common::k_suffix_of_composite_shell;

		using Common::k_suffix_of_automation_pool;

		// ----------------

		inline static auto make_standard_group_id (
			CStringView const & group_id,
			Boolean const &     is_composite
		) -> String {
			auto standard_id = String{group_id};
			if (!is_composite) {
				standard_id.append_list(k_suffix_of_composite_shell);
			}
			return standard_id;
		}

		// ----------------

		inline static auto process_package_manifest (
			OByteStreamView &                  data,
			Structure::Header<version> &       header_structure,
			typename Manifest::Package const & manifest
		) -> Void {
			auto resource_manifest_information_data_size = k_none_size;
			auto string_manifest_information_data_size = k_none_size;
			auto group_manifest_information_structure_list = List<Structure::GroupManifestInformation<version>>{};
			auto resource_detail_manifest_information_structure_list = List<Structure::ResourceDetailManifestInformation<version>>{};
			auto string_map = std::unordered_map<String, Size>{};
			auto string_list = List<String>{};
			auto set_string = [&] (
				String const & string
			) -> IntegerU32 {
				auto result = Size{};
				auto iterator = string_map.find(string);
				if (iterator != string_map.end()) {
					result = (*iterator).second;
				}
				else {
					result = string_manifest_information_data_size;
					string_list.append(string);
					string_map.emplace(std::make_pair(string, string_manifest_information_data_size));
					string_manifest_information_data_size += bs_size(string) + bs_size(CharacterType::k_null);
				}
				return cbox<IntegerU32>(result);
			};
			set_string(""_s);
			group_manifest_information_structure_list.allocate_full(manifest.group.size());
			constexpr auto image_property_key_list = make_static_array<CStringView>("type"_sv, "flag"_sv, "x"_sv, "y"_sv, "ax"_sv, "ay"_sv, "aw"_sv, "ah"_sv, "rows"_sv, "cols"_sv, "parent"_sv);
			for (auto & group_index : SizeRange{manifest.group.size()}) {
				auto & group_manifest = manifest.group[group_index];
				auto & group_manifest_information_structure = group_manifest_information_structure_list[group_index];
				group_manifest_information_structure.id_offset = set_string(make_standard_group_id(group_manifest.id, group_manifest.composite));
				group_manifest_information_structure.subgroup_count = cbox<IntegerU32>(group_manifest.subgroup.size());
				group_manifest_information_structure.subgroup_information_size = cbox<IntegerU32>(bs_static_size<Structure::SubgroupBasicManifestInformation<version>>());
				group_manifest_information_structure.subgroup_information.allocate_full(group_manifest.subgroup.size());
				for (auto & subgroup_index : SizeRange{group_manifest.subgroup.size()}) {
					auto & subgroup_manifest = group_manifest.subgroup[subgroup_index];
					auto & subgroup_manifest_information_structure = group_manifest_information_structure.subgroup_information[subgroup_index];
					if constexpr (check_version(version, {1}, {})) {
						if (!subgroup_manifest.category.resolution.has()) {
							subgroup_manifest_information_structure.resolution = 0x00000000_iu32;
						}
						else {
							subgroup_manifest_information_structure.resolution = cbox<IntegerU32>(subgroup_manifest.category.resolution.get());
						}
					}
					if constexpr (check_version(version, {3}, {})) {
						if (!subgroup_manifest.category.locale.has()) {
							subgroup_manifest_information_structure.locale = 0x00000000_iu32;
						}
						else {
							subgroup_manifest_information_structure.locale = fourcc_to_integer(subgroup_manifest.category.locale.get().template to_of<FourCC>());
						}
					}
					subgroup_manifest_information_structure.id_offset = set_string(subgroup_manifest.id);
					subgroup_manifest_information_structure.resource_count = cbox<IntegerU32>(subgroup_manifest.resource.size());
					subgroup_manifest_information_structure.resource_information.allocate_full(subgroup_manifest.resource.size());
					for (auto & resource_index : SizeRange{subgroup_manifest.resource.size()}) {
						auto & resource_manifest = subgroup_manifest.resource[resource_index];
						auto & resource_manifest_information_structure = subgroup_manifest_information_structure.resource_information[resource_index];
						auto & resource_detail_manifest_information_structure = resource_detail_manifest_information_structure_list.append();
						resource_manifest_information_structure.detail_offset = cbox<IntegerU32>(resource_manifest_information_data_size);
						resource_detail_manifest_information_structure.id_offset = set_string(resource_manifest.id);
						resource_detail_manifest_information_structure.path_offset = set_string(resource_manifest.path.to_string(CharacterType::k_path_separator_windows));
						resource_detail_manifest_information_structure.header_size = cbox<IntegerU16>(bs_static_size<Structure::ResourceBasicDetailManifestInformation<version>>());
						resource_detail_manifest_information_structure.type = cbox<IntegerU16>(resource_manifest.type);
						if (resource_detail_manifest_information_structure.type == 0_iu16) {
							auto & resource_image_property_detail_manifest_information_structure = resource_detail_manifest_information_structure.image_property_information.set();
							resource_image_property_detail_manifest_information_structure.type = cbox<IntegerU16>(resource_manifest.property["type"_sv].template to_of<Integer>());
							resource_image_property_detail_manifest_information_structure.flag = cbox<IntegerU16>(resource_manifest.property["flag"_sv].template to_of<Integer>());
							resource_image_property_detail_manifest_information_structure.x = cbox<IntegerU16>(resource_manifest.property["x"_sv].template to_of<Integer>());
							resource_image_property_detail_manifest_information_structure.y = cbox<IntegerU16>(resource_manifest.property["y"_sv].template to_of<Integer>());
							resource_image_property_detail_manifest_information_structure.ax = cbox<IntegerU16>(resource_manifest.property["ax"_sv].template to_of<Integer>());
							resource_image_property_detail_manifest_information_structure.ay = cbox<IntegerU16>(resource_manifest.property["ay"_sv].template to_of<Integer>());
							resource_image_property_detail_manifest_information_structure.aw = cbox<IntegerU16>(resource_manifest.property["aw"_sv].template to_of<Integer>());
							resource_image_property_detail_manifest_information_structure.ah = cbox<IntegerU16>(resource_manifest.property["ah"_sv].template to_of<Integer>());
							resource_image_property_detail_manifest_information_structure.rows = cbox<IntegerU16>(resource_manifest.property["rows"_sv].template to_of<Integer>());
							resource_image_property_detail_manifest_information_structure.cols = cbox<IntegerU16>(resource_manifest.property["cols"_sv].template to_of<Integer>());
							resource_image_property_detail_manifest_information_structure.parent_offset = set_string(resource_manifest.property["parent"_sv]);
						}
						else {
							resource_detail_manifest_information_structure.image_property_information.reset();
						}
						resource_detail_manifest_information_structure.property_information.allocate(resource_manifest.property.size());
						for (auto & resource_property_index : SizeRange{resource_manifest.property.size()}) {
							auto & resource_property_manifest = resource_manifest.property.at(resource_property_index);
							if (!(resource_detail_manifest_information_structure.type == 0_iu16 && Range::has(image_property_key_list, resource_property_manifest.key))) {
								auto & resource_property_detail_information_structure = resource_detail_manifest_information_structure.property_information.append();
								resource_property_detail_information_structure.key_offset = set_string(resource_property_manifest.key);
								resource_property_detail_information_structure.value_offset = set_string(resource_property_manifest.value);
							}
						}
						resource_detail_manifest_information_structure.property_information_count = cbox<IntegerU32>(resource_detail_manifest_information_structure.property_information.size());
						resource_manifest_information_data_size += bs_static_size<Structure::ResourceBasicDetailManifestInformation<version>>();
						if (resource_detail_manifest_information_structure.image_property_information.has()) {
							resource_detail_manifest_information_structure.image_property_information_offset = cbox<IntegerU32>(resource_manifest_information_data_size);
							resource_manifest_information_data_size += bs_static_size<Structure::ResourceImagePropertyDetailManifestInformation<version>>();
						}
						else {
							resource_detail_manifest_information_structure.image_property_information_offset = 0_iu32;
						}
						resource_detail_manifest_information_structure.property_information_offset = cbox<IntegerU32>(resource_manifest_information_data_size);
						resource_manifest_information_data_size += bs_static_size<List<Structure::ResourcePropertyDetailManifestInformation<version>>>(resource_detail_manifest_information_structure.property_information.size());
					}
				}
			}
			header_structure.group_manifest_information_section_offset = cbox<IntegerU32>(data.position());
			data.write(group_manifest_information_structure_list);
			header_structure.resource_manifest_information_section_offset = cbox<IntegerU32>(data.position());
			data.write(resource_detail_manifest_information_structure_list);
			header_structure.string_manifest_information_section_offset = cbox<IntegerU32>(data.position());
			for (auto & element : string_list) {
				StringParser::write_string_until(self_cast<OCharacterStreamView>(data), element, CharacterType::k_null);
				data.write_constant(CharacterType::k_null);
			}
			return;
		}

		inline static auto process_package (
			OByteStreamView &                            data,
			typename Definition::Package const &         definition,
			Optional<typename Manifest::Package> const & manifest,
			Path const &                                 resource_directory,
			Optional<Path> const &                       packet_file,
			Optional<Path> const &                       new_packet_file
		) -> Void {
			constexpr auto packet_version = ResourceStreamGroup::Version{.number = version.number};
			data.write_constant(Structure::k_magic_identifier);
			data.write_constant(cbox<Structure::VersionNumber>(version.number));
			struct {
				OByteStreamView header{};
				Size            group_id_offset{};
				OByteStreamView group_id{};
				Size            subgroup_id_offset{};
				OByteStreamView subgroup_id{};
				Size            resource_path_offset{};
				OByteStreamView resource_path{};
				Size            group_information_offset{};
				OByteStreamView group_information{};
				Size            subgroup_information_offset{};
				OByteStreamView subgroup_information{};
				Size            pool_information_offset{};
				OByteStreamView pool_information{};
				Size            texture_resource_information_offset{};
				OByteStreamView texture_resource_information{};
			} information_data = {};
			{
				auto information_structure = Structure::Information<version>{};
				auto global_group_count = k_none_size;
				auto global_subgroup_count = k_none_size;
				auto global_resource_count = k_none_size;
				auto global_general_resource_count = k_none_size;
				auto global_texture_resource_count = k_none_size;
				global_group_count += definition.group.size();
				for (auto & group_definition : definition.group) {
					global_subgroup_count += group_definition.subgroup.size();
					for (auto & subgroup_definition : group_definition.subgroup) {
						global_resource_count += subgroup_definition.resource.size();
						for (auto & resource_definition : subgroup_definition.resource) {
							switch (resource_definition.additional.type().value) {
								case ResourceType::Constant::general().value : {
									++global_general_resource_count;
									break;
								}
								case ResourceType::Constant::texture().value : {
									++global_texture_resource_count;
									break;
								}
							}
						}
					}
				}
				information_structure.group_id.allocate_full(global_group_count);
				information_structure.group_information.allocate_full(global_group_count);
				information_structure.subgroup_id.allocate_full(global_subgroup_count);
				information_structure.subgroup_information.allocate_full(global_subgroup_count);
				information_structure.pool_information.allocate_full(global_subgroup_count);
				information_structure.resource_path.allocate_full(global_resource_count);
				information_structure.texture_resource_information.allocate_full(global_texture_resource_count);
				auto global_group_index = k_begin_index;
				auto global_subgroup_index = k_begin_index;
				auto global_resource_index = k_begin_index;
				auto global_general_resource_index = k_begin_index;
				auto global_texture_resource_index = k_begin_index;
				for (auto & group_index : SizeRange{definition.group.size()}) {
					auto & group_definition = definition.group[group_index];
					auto & group_id_structure = information_structure.group_id.at(global_group_index);
					auto   standard_group_id = make_standard_group_id(group_definition.id, group_definition.composite);
					group_id_structure.key = standard_group_id;
					for (auto & subgroup_index : SizeRange{group_definition.subgroup.size()}) {
						auto & subgroup_definition = group_definition.subgroup[subgroup_index];
						auto & subgroup_id_structure = information_structure.subgroup_id.at(global_subgroup_index);
						subgroup_id_structure.key = subgroup_definition.id;
						for (auto & resource_index : SizeRange{subgroup_definition.resource.size()}) {
							auto & resource_definition = subgroup_definition.resource[resource_index];
							auto & resource_path_structure = information_structure.resource_path.at(global_resource_index);
							resource_path_structure.key = resource_definition.path.to_string(CharacterType::k_path_separator_windows);
							switch (resource_definition.additional.type().value) {
								case ResourceType::Constant::general().value : {
									auto & resource_additional_definition = resource_definition.additional.template get_of_type<ResourceType::Constant::general()>();
									++global_general_resource_index;
									break;
								}
								case ResourceType::Constant::texture().value : {
									auto & resource_additional_definition = resource_definition.additional.template get_of_type<ResourceType::Constant::texture()>();
									auto & texture_information_structure = information_structure.texture_resource_information[global_texture_resource_index];
									++global_texture_resource_index;
									break;
								}
							}
							++global_resource_index;
						}
						++global_subgroup_index;
					}
					++global_group_index;
				}
				CompiledMapData::adjust_sequence(information_structure.group_id);
				CompiledMapData::adjust_sequence(information_structure.subgroup_id);
				CompiledMapData::adjust_sequence(information_structure.resource_path);
				information_data.header = OByteStreamView{
					data.forward_view(bs_size(information_structure.header))
				};
				information_data.resource_path_offset = data.position();
				information_data.resource_path = OByteStreamView{
					data.forward_view(CompiledMapData::compute_ripe_size(information_structure.resource_path))
				};
				information_data.subgroup_id_offset = data.position();
				information_data.subgroup_id = OByteStreamView{
					data.forward_view(CompiledMapData::compute_ripe_size(information_structure.subgroup_id))
				};
				information_data.group_information_offset = data.position();
				information_data.group_information = OByteStreamView{
					data.forward_view(bs_size(information_structure.group_information))
				};
				information_data.group_id_offset = data.position();
				information_data.group_id = OByteStreamView{
					data.forward_view(CompiledMapData::compute_ripe_size(information_structure.group_id))
				};
				information_data.subgroup_information_offset = data.position();
				information_data.subgroup_information = OByteStreamView{
					data.forward_view(bs_size(information_structure.subgroup_information))
				};
				information_data.pool_information_offset = data.position();
				information_data.pool_information = OByteStreamView{
					data.forward_view(bs_size(information_structure.pool_information))
				};
				information_data.texture_resource_information_offset = data.position();
				information_data.texture_resource_information = OByteStreamView{
					data.forward_view(bs_size(information_structure.texture_resource_information))
				};
			}
			auto information_structure = Structure::Information<version>{};
			if constexpr (check_version(version, {1, 3}, {})) {
				information_structure.header.unknown_1 = 1_iu32;
			}
			if constexpr (check_version(version, {3}, {})) {
				information_structure.header.unknown_1 = 0_iu32;
			}
			auto global_group_count = k_none_size;
			auto global_subgroup_count = k_none_size;
			auto global_resource_count = k_none_size;
			auto global_general_resource_count = k_none_size;
			auto global_texture_resource_count = k_none_size;
			global_group_count += definition.group.size();
			for (auto & group_definition : definition.group) {
				global_subgroup_count += group_definition.subgroup.size();
				for (auto & subgroup_definition : group_definition.subgroup) {
					global_resource_count += subgroup_definition.resource.size();
					for (auto & resource_definition : subgroup_definition.resource) {
						switch (resource_definition.additional.type().value) {
							case ResourceType::Constant::general().value : {
								++global_general_resource_count;
								break;
							}
							case ResourceType::Constant::texture().value : {
								++global_texture_resource_count;
								break;
							}
						}
					}
				}
			}
			information_structure.header.resource_path_section_size = cbox<IntegerU32>(information_data.resource_path.size());
			information_structure.header.resource_path_section_offset = cbox<IntegerU32>(information_data.resource_path_offset);
			information_structure.header.subgroup_id_section_size = cbox<IntegerU32>(information_data.subgroup_id.size());
			information_structure.header.subgroup_id_section_offset = cbox<IntegerU32>(information_data.subgroup_id_offset);
			information_structure.header.subgroup_information_section_block_count = cbox<IntegerU32>(global_subgroup_count);
			information_structure.header.subgroup_information_section_offset = cbox<IntegerU32>(information_data.subgroup_information_offset);
			information_structure.header.subgroup_information_section_block_size = cbox<IntegerU32>(bs_static_size<Structure::SubgroupInformation<version>>());
			information_structure.header.group_information_section_block_count = cbox<IntegerU32>(global_group_count);
			information_structure.header.group_information_section_offset = cbox<IntegerU32>(information_data.group_information_offset);
			information_structure.header.group_information_section_block_size = cbox<IntegerU32>(bs_static_size<Structure::GroupInformation<version>>());
			information_structure.header.group_id_section_size = cbox<IntegerU32>(information_data.group_id.size());
			information_structure.header.group_id_section_offset = cbox<IntegerU32>(information_data.group_id_offset);
			information_structure.header.pool_information_section_block_count = cbox<IntegerU32>(global_subgroup_count);
			information_structure.header.pool_information_section_offset = cbox<IntegerU32>(information_data.pool_information_offset);
			information_structure.header.pool_information_section_block_size = cbox<IntegerU32>(bs_static_size<Structure::PoolInformation<version>>());
			information_structure.header.texture_resource_information_section_block_count = cbox<IntegerU32>(global_texture_resource_count);
			information_structure.header.texture_resource_information_section_offset = cbox<IntegerU32>(information_data.texture_resource_information_offset);
			information_structure.header.texture_resource_information_section_block_size = cbox<IntegerU32>(bs_static_size<Structure::TextureResourceInformation<version>>());
			if constexpr (check_version(version, {4}, {})) {
				data.write_space(k_null_byte, compute_padding_size(data.position(), k_padding_unit_size));
				information_structure.header.information_without_manifest_section_size = cbox<IntegerU32>(data.position());
			}
			if (manifest.has()) {
				process_package_manifest(data, information_structure.header, manifest.get());
			}
			else {
				information_structure.header.group_manifest_information_section_offset = cbox<IntegerU32>(k_none_size);
				information_structure.header.resource_manifest_information_section_offset = cbox<IntegerU32>(k_none_size);
				information_structure.header.string_manifest_information_section_offset = cbox<IntegerU32>(k_none_size);
			}
			data.write_space(k_null_byte, compute_padding_size(data.position(), k_padding_unit_size));
			information_structure.header.information_section_size = cbox<IntegerU32>(data.position());
			information_structure.group_id.allocate_full(global_group_count);
			information_structure.group_information.allocate_full(global_group_count);
			information_structure.subgroup_id.allocate_full(global_subgroup_count);
			information_structure.subgroup_information.allocate_full(global_subgroup_count);
			information_structure.pool_information.allocate_full(global_subgroup_count);
			information_structure.resource_path.allocate_full(global_resource_count);
			information_structure.texture_resource_information.allocate_full(global_texture_resource_count);
			auto global_group_index = k_begin_index;
			auto global_subgroup_index = k_begin_index;
			auto global_resource_index = k_begin_index;
			auto global_general_resource_index = k_begin_index;
			auto global_texture_resource_index = k_begin_index;
			for (auto & group_index : SizeRange{definition.group.size()}) {
				auto & group_definition = definition.group[group_index];
				auto & group_id_structure = information_structure.group_id.at(global_group_index);
				auto & group_information_structure = information_structure.group_information[global_group_index];
				auto   standard_group_id = make_standard_group_id(group_definition.id, group_definition.composite);
				group_id_structure.key = standard_group_id;
				group_id_structure.value = cbox<IntegerU32>(global_group_index);
				group_information_structure.id = string_block_fixed_128_from_string(standard_group_id);
				group_information_structure.subgroup_count = cbox<IntegerU32>(group_definition.subgroup.size());
				for (auto & subgroup_index : SizeRange{group_definition.subgroup.size()}) {
					auto & subgroup_definition = group_definition.subgroup[subgroup_index];
					auto & simple_subgroup_information_structure = group_information_structure.subgroup_information[subgroup_index];
					auto & subgroup_id_structure = information_structure.subgroup_id.at(global_subgroup_index);
					auto & subgroup_information_structure = information_structure.subgroup_information[global_subgroup_index];
					auto & pool_information_structure = information_structure.pool_information[global_subgroup_index];
					auto   packet_package_definition = typename ResourceStreamGroup::Definition<packet_version>::Package{};
					simple_subgroup_information_structure.index = cbox<IntegerU32>(global_subgroup_index);
					if constexpr (check_version(version, {1}, {})) {
						if (!subgroup_definition.category.resolution.has()) {
							simple_subgroup_information_structure.resolution = 0x00000000_iu32;
						}
						else {
							simple_subgroup_information_structure.resolution = cbox<IntegerU32>(subgroup_definition.category.resolution.get());
						}
					}
					if constexpr (check_version(version, {3}, {})) {
						if (!subgroup_definition.category.locale.has()) {
							simple_subgroup_information_structure.locale = 0x00000000_iu32;
						}
						else {
							simple_subgroup_information_structure.locale = fourcc_to_integer(subgroup_definition.category.locale.get().template to_of<FourCC>());
						}
					}
					subgroup_id_structure.key = subgroup_definition.id;
					subgroup_id_structure.value = cbox<IntegerU32>(global_subgroup_index);
					subgroup_information_structure.id = string_block_fixed_128_from_string(subgroup_definition.id);
					subgroup_information_structure.pool = cbox<IntegerU32>(global_subgroup_index);
					pool_information_structure.id = string_block_fixed_128_from_string(subgroup_definition.id + k_suffix_of_automation_pool);
					pool_information_structure.instance_count = 1_iu32;
					pool_information_structure.flag = 0_iu32;
					packet_package_definition.compression = subgroup_definition.compression;
					packet_package_definition.resource.allocate_full(subgroup_definition.resource.size());
					auto make_formatted_path = [&] (
						Path const & path_format
					) -> Path {
						return Path{format_string(path_format.to_string(), group_definition.id, subgroup_definition.id)};
					};
					auto texture_resource_begin = global_texture_resource_index;
					auto texture_resource_count = k_none_size;
					for (auto & resource_index : SizeRange{subgroup_definition.resource.size()}) {
						auto & resource_definition = subgroup_definition.resource[resource_index];
						auto & resource_path_structure = information_structure.resource_path.at(global_resource_index);
						auto & packet_resource_definition = packet_package_definition.resource[resource_index];
						resource_path_structure.key = resource_definition.path.to_string(CharacterType::k_path_separator_windows);
						resource_path_structure.value = cbox<IntegerU32>(global_subgroup_index);
						packet_resource_definition.path = resource_definition.path;
						switch (resource_definition.additional.type().value) {
							case ResourceType::Constant::general().value : {
								auto & resource_additional_definition = resource_definition.additional.template get_of_type<ResourceType::Constant::general()>();
								auto & packet_resource_additional_definition = packet_resource_definition.additional.template set_of_type<ResourceType::Constant::general()>();
								++global_general_resource_index;
								break;
							}
							case ResourceType::Constant::texture().value : {
								auto & resource_additional_definition = resource_definition.additional.template get_of_type<ResourceType::Constant::texture()>();
								auto & texture_information_structure = information_structure.texture_resource_information[global_texture_resource_index];
								auto & packet_resource_additional_definition = packet_resource_definition.additional.template set_of_type<ResourceType::Constant::texture()>();
								texture_information_structure.size_width = cbox<IntegerU32>(resource_additional_definition.size.width);
								texture_information_structure.size_height = cbox<IntegerU32>(resource_additional_definition.size.height);
								texture_information_structure.pitch = cbox<IntegerU32>(resource_additional_definition.pitch);
								texture_information_structure.format = cbox<IntegerU32>(resource_additional_definition.format);
								if constexpr (check_version(version, {4}, {1})) {
									texture_information_structure.additional_byte_count = cbox<IntegerU32>(resource_additional_definition.additional_byte_count);
								}
								if constexpr (check_version(version, {4}, {2})) {
									texture_information_structure.scale = cbox<IntegerU32>(resource_additional_definition.scale);
								}
								packet_resource_additional_definition.index = cbox<Integer>(texture_resource_count);
								packet_resource_additional_definition.size = resource_additional_definition.size;
								++texture_resource_count;
								++global_texture_resource_index;
								break;
							}
						}
						++global_resource_index;
					}
					auto packet_data = OByteStreamView{data.reserve_view()};
					auto use_legacy_packet = k_false;
					auto packet_header_structure = ResourceStreamGroup::Structure::Header<packet_version>{};
					if (packet_file.has()) {
						if (Storage::exist_file(make_formatted_path(packet_file.get()))) {
							auto legacy_packet_size = Storage::read_stream_file(make_formatted_path(packet_file.get()), packet_data);
							auto legacy_packet_stream = IByteStreamView{packet_data.previous_view(legacy_packet_size)};
							legacy_packet_stream.read_constant(ResourceStreamGroup::Structure::k_magic_identifier);
							legacy_packet_stream.read_constant(cbox<ResourceStreamGroup::Structure::VersionNumber>(packet_version.number));
							legacy_packet_stream.read(packet_header_structure);
							use_legacy_packet = k_true;
						}
					}
					if (!use_legacy_packet) {
						ResourceStreamGroup::Pack<packet_version>::process(packet_data, packet_package_definition, make_formatted_path(resource_directory));
						if (new_packet_file.has()) {
							Storage::write_file(make_formatted_path(new_packet_file.get()), packet_data.stream_view());
						}
						auto legacy_packet_stream = IByteStreamView{packet_data.stream_view(), bs_static_size<ResourceStreamGroup::Structure::MagicIdentifier>() + bs_static_size<ResourceStreamGroup::Structure::VersionNumber>()};
						legacy_packet_stream.read(packet_header_structure);
					}
					subgroup_information_structure.offset = cbox<IntegerU32>(data.position());
					subgroup_information_structure.size = cbox<IntegerU32>(packet_data.position());
					subgroup_information_structure.resource_data_section_compression = packet_header_structure.resource_data_section_compression;
					subgroup_information_structure.information_section_size = packet_header_structure.information_section_size;
					subgroup_information_structure.general_resource_data_section_offset = packet_header_structure.general_resource_data_section_offset;
					subgroup_information_structure.general_resource_data_section_size = packet_header_structure.general_resource_data_section_size;
					subgroup_information_structure.general_resource_data_section_size_original = packet_header_structure.general_resource_data_section_size_original;
					subgroup_information_structure.general_resource_data_section_size_pool = packet_header_structure.general_resource_data_section_size_original;
					subgroup_information_structure.texture_resource_data_section_offset = packet_header_structure.texture_resource_data_section_offset;
					subgroup_information_structure.texture_resource_data_section_size = packet_header_structure.texture_resource_data_section_size;
					subgroup_information_structure.texture_resource_data_section_size_original = packet_header_structure.texture_resource_data_section_size_original;
					subgroup_information_structure.texture_resource_data_section_size_pool = 0_iu32;
					pool_information_structure.texture_resource_data_section_offset = packet_header_structure.general_resource_data_section_offset + packet_header_structure.general_resource_data_section_size_original;
					pool_information_structure.texture_resource_data_section_size = packet_header_structure.texture_resource_data_section_size_original;
					if constexpr (check_version(version, {1, 3}, {})) {
						pool_information_structure.texture_resource_begin = cbox<IntegerU32>(texture_resource_begin);
						pool_information_structure.texture_resource_count = cbox<IntegerU32>(texture_resource_count);
					}
					if constexpr (check_version(version, {3}, {})) {
						subgroup_information_structure.texture_resource_begin = cbox<IntegerU32>(texture_resource_begin);
						subgroup_information_structure.texture_resource_count = cbox<IntegerU32>(texture_resource_count);
						pool_information_structure.texture_resource_begin = 0_iu32;
						pool_information_structure.texture_resource_count = 0_iu32;
					}
					data.forward(packet_data.position());
					++global_subgroup_index;
				}
				++global_group_index;
			}
			CompiledMapData::adjust_sequence(information_structure.group_id);
			CompiledMapData::adjust_sequence(information_structure.subgroup_id);
			CompiledMapData::adjust_sequence(information_structure.resource_path);
			{
				information_data.header.write(information_structure.header);
				CompiledMapData::encode(information_structure.resource_path, information_data.resource_path);
				CompiledMapData::encode(information_structure.subgroup_id, information_data.subgroup_id);
				information_data.group_information.write(information_structure.group_information);
				CompiledMapData::encode(information_structure.group_id, information_data.group_id);
				information_data.subgroup_information.write(information_structure.subgroup_information);
				information_data.pool_information.write(information_structure.pool_information);
				information_data.texture_resource_information.write(information_structure.texture_resource_information);
			}
			return;
		}

		// ----------------

		inline static auto process (
			OByteStreamView &                            data_,
			typename Definition::Package const &         definition,
			Optional<typename Manifest::Package> const & manifest,
			Path const &                                 resource_directory,
			Optional<Path> const &                       packet_file,
			Optional<Path> const &                       new_packet_file
		) -> Void {
			M_use_zps_of(data);
			return process_package(data, definition, manifest, resource_directory, packet_file, new_packet_file);
		}

	};

}
