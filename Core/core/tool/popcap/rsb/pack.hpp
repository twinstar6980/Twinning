#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/rsb/version.hpp"
#include "core/tool/popcap/rsb/manifest.hpp"
#include "core/tool/popcap/rsb/description.hpp"
#include "core/tool/popcap/rsb/structure.hpp"
#include "core/tool/popcap/rsgp/pack.hpp"
#include <unordered_map>

namespace TwinKleS::Core::Tool::PopCap::RSB {

	inline namespace CommonOfPack {

		namespace Detail {

		}

	}

	namespace Pack {

		namespace Detail {

			#pragma region using

			using namespace CommonOfPack::Detail;

			#pragma endregion

			#pragma region process

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_package_description (
				OByteStreamView &                     package_data,
				Structure::Header<version> &          header_structure,
				Description::Package<version> const & package_description
			) -> Void {
				auto resource_information_data_size = k_none_size;
				auto string_information_data_size = k_none_size;
				auto group_description_information_structure_list = List<Structure::GroupDescriptionInformation<version>>{};
				auto resource_detail_description_information_structure_list = List<Structure::ResourceDetailDescriptionInformation<version>>{};
				auto string_map = std::unordered_map<String, Size>{};
				auto string_list = List<String>{};
				auto set_string =
					[&] (
					String const & string
				) -> auto {
					auto result = Size{};
					auto iterator = string_map.find(string);
					if (iterator != string_map.end()) {
						result = (*iterator).second;
					} else {
						result = string_information_data_size;
						string_list.append(string);
						string_map.emplace(std::make_pair(string, string_information_data_size));
						string_information_data_size += bs_size(string) + bs_size(CharacterType::k_null);
					}
					return cbw<IntegerU32>(result);
				};
				set_string(""_s);
				group_description_information_structure_list.allocate_full(package_description.group.size());
				constexpr auto image_property_key_list = StaticArray<CStringView, 11_szz>{{"type"_sv, "flag"_sv, "x"_sv, "y"_sv, "ax"_sv, "ay"_sv, "aw"_sv, "ah"_sv, "rows"_sv, "cols"_sv, "parent"_sv}};
				for (auto & group_index : SizeRange{package_description.group.size()}) {
					auto & group_description = package_description.group.at(group_index);
					auto & group_description_information_structure = group_description_information_structure_list[group_index];
					group_description_information_structure.id_offset = set_string(make_standard_group_id(group_description.key, group_description.value.composite));
					group_description_information_structure.subgroup_count = cbw<IntegerU32>(group_description.value.subgroup.size());
					group_description_information_structure.subgroup_information_size = cbw<IntegerU32>(bs_static_size<Structure::SubgroupBasicDescriptionInformation<version>>());
					group_description_information_structure.subgroup_information.allocate_full(group_description.value.subgroup.size());
					for (auto & subgroup_index : SizeRange{group_description.value.subgroup.size()}) {
						auto & subgroup_description = group_description.value.subgroup.at(subgroup_index);
						auto & subgroup_description_information_structure = group_description_information_structure.subgroup_information[subgroup_index];
						subgroup_category_to_data(subgroup_description.value.category, subgroup_description_information_structure.resolution, subgroup_description_information_structure.locale);
						subgroup_description_information_structure.id_offset = set_string(subgroup_description.key);
						subgroup_description_information_structure.resource_count = cbw<IntegerU32>(subgroup_description.value.resource.size());
						subgroup_description_information_structure.resource_information.allocate_full(subgroup_description.value.resource.size());
						for (auto & resource_index : SizeRange{subgroup_description.value.resource.size()}) {
							auto & resource_description = subgroup_description.value.resource.at(resource_index);
							auto & resource_description_information_structure = subgroup_description_information_structure.resource_information[resource_index];
							auto & resource_detail_description_information_structure = resource_detail_description_information_structure_list.append();
							resource_description_information_structure.detail_offset = cbw<IntegerU32>(resource_information_data_size);
							resource_detail_description_information_structure.id_offset = set_string(resource_description.key);
							resource_detail_description_information_structure.path_offset = set_string(resource_description.value.path.to_string(CharacterType::PathSeparator::windows));
							resource_detail_description_information_structure.header_size = cbw<IntegerU16>(bs_static_size<Structure::ResourceBasicDetailDescriptionInformation<version>>());
							resource_detail_description_information_structure.type = cbw<IntegerU16>(resource_description.value.type);
							if (resource_detail_description_information_structure.type == 0_iu16) {
								auto & resource_image_property_detail_description_information_structure = resource_detail_description_information_structure.image_property_information.set();
								resource_image_property_detail_description_information_structure.type = cbw<IntegerU16>(resource_description.value.property["type"_sv].template to_of<Integer>());
								resource_image_property_detail_description_information_structure.flag = cbw<IntegerU16>(resource_description.value.property["flag"_sv].template to_of<Integer>());
								resource_image_property_detail_description_information_structure.x = cbw<IntegerU16>(resource_description.value.property["x"_sv].template to_of<Integer>());
								resource_image_property_detail_description_information_structure.y = cbw<IntegerU16>(resource_description.value.property["y"_sv].template to_of<Integer>());
								resource_image_property_detail_description_information_structure.ax = cbw<IntegerU16>(resource_description.value.property["ax"_sv].template to_of<Integer>());
								resource_image_property_detail_description_information_structure.ay = cbw<IntegerU16>(resource_description.value.property["ay"_sv].template to_of<Integer>());
								resource_image_property_detail_description_information_structure.aw = cbw<IntegerU16>(resource_description.value.property["aw"_sv].template to_of<Integer>());
								resource_image_property_detail_description_information_structure.ah = cbw<IntegerU16>(resource_description.value.property["ah"_sv].template to_of<Integer>());
								resource_image_property_detail_description_information_structure.rows = cbw<IntegerU16>(resource_description.value.property["rows"_sv].template to_of<Integer>());
								resource_image_property_detail_description_information_structure.cols = cbw<IntegerU16>(resource_description.value.property["cols"_sv].template to_of<Integer>());
								resource_image_property_detail_description_information_structure.parent_offset = set_string(resource_description.value.property["parent"_sv]);
							} else {
								resource_detail_description_information_structure.image_property_information.reset();
							}
							resource_detail_description_information_structure.property_information.allocate(resource_description.value.property.size());
							for (auto & resource_property_index : SizeRange{resource_description.value.property.size()}) {
								auto & resource_property_description = resource_description.value.property.at(resource_property_index);
								if (!(resource_detail_description_information_structure.type == 0_iu16 && Range::has(image_property_key_list, resource_property_description.key))) {
									auto & resource_property_detail_information_structure = resource_detail_description_information_structure.property_information.append();
									resource_property_detail_information_structure.key_offset = set_string(resource_property_description.key);
									resource_property_detail_information_structure.value_offset = set_string(resource_property_description.value);
								}
							}
							resource_detail_description_information_structure.property_information_count = cbw<IntegerU32>(resource_detail_description_information_structure.property_information.size());
							resource_information_data_size += bs_static_size<Structure::ResourceBasicDetailDescriptionInformation<version>>();
							if (resource_detail_description_information_structure.image_property_information) {
								resource_detail_description_information_structure.image_property_information_offset = cbw<IntegerU32>(resource_information_data_size);
								resource_information_data_size += bs_static_size<Structure::ResourceImagePropertyDetailDescriptionInformation<version>>();
							} else {
								resource_detail_description_information_structure.image_property_information_offset = 0_iu32;
							}
							resource_detail_description_information_structure.property_information_offset = cbw<IntegerU32>(resource_information_data_size);
							resource_information_data_size += bs_static_size<List<Structure::ResourcePropertyDetailDescriptionInformation<version>>>(resource_detail_description_information_structure.property_information.size());
						}
					}
				}
				header_structure.description_group_section_offset = cbw<IntegerU32>(package_data.position());
				package_data.write(group_description_information_structure_list);
				header_structure.description_resource_section_offset = cbw<IntegerU32>(package_data.position());
				package_data.write(resource_detail_description_information_structure_list);
				header_structure.description_string_section_offset = cbw<IntegerU32>(package_data.position());
				for (auto & element : string_list) {
					package_data.write(element);
					package_data.write(CharacterType::k_null);
				}
				return;
			}

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_package (
				OByteStreamView &                               package_data,
				Manifest::Package<version> const &              package_manifest,
				Optional<Description::Package<version>> const & package_description,
				Path const &                                    resource_directory,
				Optional<Path> const &                          packet_file,
				Optional<Path> const &                          new_packet_file
			) -> Void {
				constexpr auto packet_version = RSGP::Version{version.number};
				package_data.write(Structure::k_magic_identifier);
				package_data.write(cbw<Structure::VersionNumber>(version.number));
				struct {
					OByteStreamView header;
					Size            group_id_offset;
					OByteStreamView group_id;
					Size            subgroup_id_offset;
					OByteStreamView subgroup_id;
					Size            resource_path_offset;
					OByteStreamView resource_path;
					Size            group_information_offset;
					OByteStreamView group_information;
					Size            subgroup_information_offset;
					OByteStreamView subgroup_information;
					Size            subgroup_pool_information_offset;
					OByteStreamView subgroup_pool_information;
					Size            texture_resource_information_offset;
					OByteStreamView texture_resource_information;
				}
					information_data = {};
				{
					auto information_structure = Structure::Information<version>{};
					auto global_group_count = k_none_size;
					auto global_subgroup_count = k_none_size;
					auto global_resource_count = k_none_size;
					auto global_generic_resource_count = k_none_size;
					auto global_texture_resource_count = k_none_size;
					global_group_count += package_manifest.group.size();
					for (auto & group : package_manifest.group) {
						global_subgroup_count += group.value.subgroup.size();
						for (auto & subgroup : group.value.subgroup) {
							global_resource_count += subgroup.value.resource.size();
							for (auto & resource : subgroup.value.resource) {
								switch (resource.value.additional.type()) {
									case ResourceType::generic : {
										++global_generic_resource_count;
										break;
									}
									case ResourceType::texture : {
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
					information_structure.subgroup_pool_information.allocate_full(global_subgroup_count);
					information_structure.resource_path.allocate_full(global_resource_count);
					information_structure.texture_resource_information.allocate_full(global_texture_resource_count);
					auto global_group_index = k_begin_index;
					auto global_subgroup_index = k_begin_index;
					auto global_resource_index = k_begin_index;
					auto global_generic_resource_index = k_begin_index;
					auto global_texture_resource_index = k_begin_index;
					for (auto & group_index : SizeRange{package_manifest.group.size()}) {
						auto & group_manifest = package_manifest.group.at(group_index);
						auto & group_id_structure = information_structure.group_id.at(global_group_index);
						auto   standard_group_id = make_standard_group_id(group_manifest.key, group_manifest.value.composite);
						group_id_structure.key = standard_group_id;
						for (auto & subgroup_index : SizeRange{group_manifest.value.subgroup.size()}) {
							auto & subgroup_manifest = group_manifest.value.subgroup.at(subgroup_index);
							auto & subgroup_id_structure = information_structure.subgroup_id.at(global_subgroup_index);
							subgroup_id_structure.key = subgroup_manifest.key;
							for (auto & resource_index : SizeRange{subgroup_manifest.value.resource.size()}) {
								auto & resource_manifest = subgroup_manifest.value.resource.at(resource_index);
								auto & resource_path_structure = information_structure.resource_path.at(global_resource_index);
								resource_path_structure.key = resource_manifest.key.to_string(CharacterType::PathSeparator::windows);
								switch (resource_manifest.value.additional.type()) {
									case ResourceType::generic : {
										auto & resource_additional_manifest = resource_manifest.value.additional.template get_of_type<ResourceType::generic>();
										++global_generic_resource_index;
										break;
									}
									case ResourceType::texture : {
										auto & resource_additional_manifest = resource_manifest.value.additional.template get_of_type<ResourceType::texture>();
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
					StringMapSection::adjust_sequence(information_structure.resource_path);
					StringMapSection::adjust_sequence(information_structure.group_id);
					StringMapSection::adjust_sequence(information_structure.subgroup_id);
					information_data.header = OByteStreamView{
						package_data.forward_view(bs_size(information_structure.header))
					};
					information_data.resource_path_offset = package_data.position();
					information_data.resource_path = OByteStreamView{
						package_data.forward_view(StringMapSection::compute_ripe_size(information_structure.resource_path))
					};
					information_data.subgroup_id_offset = package_data.position();
					information_data.subgroup_id = OByteStreamView{
						package_data.forward_view(StringMapSection::compute_ripe_size(information_structure.subgroup_id))
					};
					information_data.group_information_offset = package_data.position();
					information_data.group_information = OByteStreamView{
						package_data.forward_view(bs_size(information_structure.group_information))
					};
					information_data.group_id_offset = package_data.position();
					information_data.group_id = OByteStreamView{
						package_data.forward_view(StringMapSection::compute_ripe_size(information_structure.group_id))
					};
					information_data.subgroup_information_offset = package_data.position();
					information_data.subgroup_information = OByteStreamView{
						package_data.forward_view(bs_size(information_structure.subgroup_information))
					};
					information_data.subgroup_pool_information_offset = package_data.position();
					information_data.subgroup_pool_information = OByteStreamView{
						package_data.forward_view(bs_size(information_structure.subgroup_pool_information))
					};
					information_data.texture_resource_information_offset = package_data.position();
					information_data.texture_resource_information = OByteStreamView{
						package_data.forward_view(bs_size(information_structure.texture_resource_information))
					};
				}
				auto information_structure = Structure::Information<version>{};
				auto global_group_count = k_none_size;
				auto global_subgroup_count = k_none_size;
				auto global_resource_count = k_none_size;
				auto global_generic_resource_count = k_none_size;
				auto global_texture_resource_count = k_none_size;
				global_group_count += package_manifest.group.size();
				for (auto & group : package_manifest.group) {
					global_subgroup_count += group.value.subgroup.size();
					for (auto & subgroup : group.value.subgroup) {
						global_resource_count += subgroup.value.resource.size();
						for (auto & resource : subgroup.value.resource) {
							switch (resource.value.additional.type()) {
								case ResourceType::generic : {
									++global_generic_resource_count;
									break;
								}
								case ResourceType::texture : {
									++global_texture_resource_count;
									break;
								}
							}
						}
					}
				}
				information_structure.header.resource_path_section_size = cbw<IntegerU32>(information_data.resource_path.size());
				information_structure.header.resource_path_section_offset = cbw<IntegerU32>(information_data.resource_path_offset);
				information_structure.header.subgroup_id_section_size = cbw<IntegerU32>(information_data.subgroup_id.size());
				information_structure.header.subgroup_id_section_offset = cbw<IntegerU32>(information_data.subgroup_id_offset);
				information_structure.header.subgroup_information_section_block_count = cbw<IntegerU32>(global_subgroup_count);
				information_structure.header.subgroup_information_section_offset = cbw<IntegerU32>(information_data.subgroup_information_offset);
				information_structure.header.subgroup_information_section_block_size = cbw<IntegerU32>(bs_static_size<Structure::SubgroupInformation<version>>());
				information_structure.header.group_information_section_block_count = cbw<IntegerU32>(global_group_count);
				information_structure.header.group_information_section_offset = cbw<IntegerU32>(information_data.group_information_offset);
				information_structure.header.group_information_section_block_size = cbw<IntegerU32>(bs_static_size<Structure::GroupInformation<version>>());
				information_structure.header.group_id_section_size = cbw<IntegerU32>(information_data.group_id.size());
				information_structure.header.group_id_section_offset = cbw<IntegerU32>(information_data.group_id_offset);
				information_structure.header.subgroup_pool_information_section_block_count = cbw<IntegerU32>(global_subgroup_count);
				information_structure.header.subgroup_pool_information_section_offset = cbw<IntegerU32>(information_data.subgroup_pool_information_offset);
				information_structure.header.subgroup_pool_information_section_block_size = cbw<IntegerU32>(bs_static_size<Structure::SubgroupPoolInformation<version>>());
				information_structure.header.texture_resource_information_section_block_count = cbw<IntegerU32>(global_texture_resource_count);
				information_structure.header.texture_resource_information_section_offset = cbw<IntegerU32>(information_data.texture_resource_information_offset);
				information_structure.header.texture_resource_information_section_block_size = cbw<IntegerU32>(bs_static_size<Structure::TextureResourceInformation<version>>());
				if constexpr (version.number >= 4_i) {
					package_data.pad_auto(k_padding_unit_size);
					information_structure.header.information_without_description_section_size = cbw<IntegerU32>(package_data.position());
				}
				if (package_description) {
					process_package_description<version>(package_data, information_structure.header, package_description.get());
				} else {
					information_structure.header.description_group_section_offset = cbw<IntegerU32>(k_none_size);
					information_structure.header.description_resource_section_offset = cbw<IntegerU32>(k_none_size);
					information_structure.header.description_string_section_offset = cbw<IntegerU32>(k_none_size);
				}
				package_data.pad_auto(k_padding_unit_size);
				information_structure.header.information_section_size = cbw<IntegerU32>(package_data.position());
				information_structure.group_id.allocate_full(global_group_count);
				information_structure.group_information.allocate_full(global_group_count);
				information_structure.subgroup_id.allocate_full(global_subgroup_count);
				information_structure.subgroup_information.allocate_full(global_subgroup_count);
				information_structure.subgroup_pool_information.allocate_full(global_subgroup_count);
				information_structure.resource_path.allocate_full(global_resource_count);
				information_structure.texture_resource_information.allocate_full(global_texture_resource_count);
				auto global_group_index = k_begin_index;
				auto global_subgroup_index = k_begin_index;
				auto global_resource_index = k_begin_index;
				auto global_generic_resource_index = k_begin_index;
				auto global_texture_resource_index = k_begin_index;
				for (auto & group_index : SizeRange{package_manifest.group.size()}) {
					auto & group_manifest = package_manifest.group.at(group_index);
					auto & group_id_structure = information_structure.group_id.at(global_group_index);
					auto & group_information_structure = information_structure.group_information[global_group_index];
					auto   standard_group_id = make_standard_group_id(group_manifest.key, group_manifest.value.composite);
					group_id_structure.key = standard_group_id;
					group_id_structure.value = cbw<IntegerU32>(global_group_index);
					group_information_structure.id = string_block_fixed_128_from_string(standard_group_id);
					group_information_structure.subgroup_count = cbw<IntegerU32>(group_manifest.value.subgroup.size());
					for (auto & subgroup_index : SizeRange{group_manifest.value.subgroup.size()}) {
						auto & subgroup_manifest = group_manifest.value.subgroup.at(subgroup_index);
						auto & simple_subgroup_information_structure = group_information_structure.subgroup_information[subgroup_index];
						auto & subgroup_id_structure = information_structure.subgroup_id.at(global_subgroup_index);
						auto & subgroup_information_structure = information_structure.subgroup_information[global_subgroup_index];
						auto & subgroup_pool_information_structure = information_structure.subgroup_pool_information[global_subgroup_index];
						auto   packet_package_manifest = RSGP::Manifest::Package<packet_version>{};
						simple_subgroup_information_structure.index = cbw<IntegerU32>(global_subgroup_index);
						subgroup_category_to_data(subgroup_manifest.value.category, simple_subgroup_information_structure.resolution, simple_subgroup_information_structure.locale);
						subgroup_id_structure.key = subgroup_manifest.key;
						subgroup_id_structure.value = cbw<IntegerU32>(global_subgroup_index);
						subgroup_information_structure.id = string_block_fixed_128_from_string(subgroup_manifest.key);
						subgroup_information_structure.index = cbw<IntegerU32>(global_subgroup_index);
						subgroup_information_structure.texture_resource_begin_index = cbw<IntegerU32>(global_texture_resource_index);
						subgroup_information_structure.texture_resource_count = cbw<IntegerU32>(k_none_size);
						subgroup_pool_information_structure.id = string_block_fixed_128_from_string(subgroup_manifest.key + k_suffix_of_auto_pool);
						subgroup_pool_information_structure.unknown_1 = 1_iu32;
						packet_package_manifest.resource_data_section_store_mode = subgroup_manifest.value.resource_data_section_store_mode;
						packet_package_manifest.resource.allocate_full(subgroup_manifest.value.resource.size());
						auto make_formatted_path =
							[&] (
							Path const & path_format
						) -> auto {
							return Path{format_string(path_format.to_string(), group_manifest.key, subgroup_manifest.key)};
						};
						auto generic_resource_index = k_begin_index;
						auto texture_resource_index = k_begin_index;
						for (auto & resource_index : SizeRange{subgroup_manifest.value.resource.size()}) {
							auto & resource_manifest = subgroup_manifest.value.resource.at(resource_index);
							auto & resource_path_structure = information_structure.resource_path.at(global_resource_index);
							auto & packet_resource_manifest = packet_package_manifest.resource.at(resource_index);
							resource_path_structure.key = resource_manifest.key.to_string(CharacterType::PathSeparator::windows);
							resource_path_structure.value = cbw<IntegerU32>(global_subgroup_index);
							packet_resource_manifest.key = resource_manifest.key;
							switch (resource_manifest.value.additional.type()) {
								case ResourceType::generic : {
									auto & resource_additional_manifest = resource_manifest.value.additional.template get_of_type<ResourceType::generic>();
									auto & packet_resource_additional_manifest = packet_resource_manifest.value.additional.template set_of_type<ResourceType::generic>();
									++generic_resource_index;
									++global_generic_resource_index;
									break;
								}
								case ResourceType::texture : {
									auto & resource_additional_manifest = resource_manifest.value.additional.template get_of_type<ResourceType::texture>();
									auto & texture_information_structure = information_structure.texture_resource_information[global_texture_resource_index];
									auto & packet_resource_additional_manifest = packet_resource_manifest.value.additional.template set_of_type<ResourceType::texture>();
									texture_information_structure.size_width = cbw<IntegerU32>(resource_additional_manifest.size.width);
									texture_information_structure.size_height = cbw<IntegerU32>(resource_additional_manifest.size.height);
									texture_information_structure.format = cbw<IntegerU32>(resource_additional_manifest.format);
									texture_information_structure.bit_per_channel_of_row_division_2 = cbw<IntegerU32>(resource_additional_manifest.size.width * resource_additional_manifest.bit_per_channel / 2_i);
									if constexpr (version.additional_texture_information_for_pvz_2_chinese_android >= 1_i) {
										if (resource_additional_manifest.format == 0x93_i || resource_additional_manifest.format == 0x96_i) {
											texture_information_structure.alpha_size = cbw<IntegerU32>(FileSystem::size_file(make_formatted_path(resource_directory) / resource_manifest.key) - cbw<Size>(resource_additional_manifest.size.area() / 2_i));
										} else {
											texture_information_structure.alpha_size = cbw<IntegerU32>(k_none_size);
										}
									}
									if constexpr (version.additional_texture_information_for_pvz_2_chinese_android >= 2_i) {
										texture_information_structure.scale = cbw<IntegerU32>(resource_additional_manifest.scale);
									}
									packet_resource_additional_manifest.index = cbw<Integer>(texture_resource_index);
									packet_resource_additional_manifest.size = resource_additional_manifest.size;
									++subgroup_information_structure.texture_resource_count;
									++texture_resource_index;
									++global_texture_resource_index;
									break;
								}
							}
							++global_resource_index;
						}
						auto packet_data = OByteStreamView{package_data.reserve_view()};
						auto use_legacy_packet = k_false;
						auto packet_header_structure = RSGP::Structure::Header<packet_version>{};
						if (packet_file) {
							if (FileSystem::exist_file(make_formatted_path(packet_file.get()))) {
								auto legacy_packet_size = FileSystem::read_stream_file(make_formatted_path(packet_file.get()), packet_data);
								auto legacy_packet_stream = IByteStreamView{packet_data.prev_view(legacy_packet_size)};
								assert_condition(legacy_packet_stream.read_of<RSGP::Structure::MagicIdentifier>() == RSGP::Structure::k_magic_identifier);
								assert_condition(legacy_packet_stream.read_of<RSGP::Structure::VersionNumber>() == cbw<IntegerU32>(version.number));
								legacy_packet_stream.read(packet_header_structure);
								use_legacy_packet = k_true;
							}
						}
						if (!use_legacy_packet) {
							RSGP::Pack::process<packet_version>(packet_data, packet_package_manifest, make_formatted_path(resource_directory));
							if (new_packet_file) {
								FileSystem::write_file(make_formatted_path(new_packet_file.get()), packet_data.stream_view());
							}
							auto legacy_packet_stream = IByteStreamView{packet_data.stream_view(), bs_static_size<RSGP::Structure::MagicIdentifier>() + bs_static_size<RSGP::Structure::VersionNumber>()};
							legacy_packet_stream.read(packet_header_structure);
						}
						subgroup_information_structure.offset = cbw<IntegerU32>(package_data.position());
						subgroup_information_structure.size = cbw<IntegerU32>(packet_data.position());
						subgroup_information_structure.resource_data_section_store_mode = packet_header_structure.resource_data_section_store_mode;
						subgroup_information_structure.information_section_size = packet_header_structure.information_section_size;
						subgroup_information_structure.generic_resource_data_section_offset = packet_header_structure.generic_resource_data_section_offset;
						subgroup_information_structure.generic_resource_data_section_size = packet_header_structure.generic_resource_data_section_size;
						subgroup_information_structure.generic_resource_data_section_size_original = packet_header_structure.generic_resource_data_section_size_original;
						subgroup_information_structure.generic_resource_data_section_size_original_1 = packet_header_structure.generic_resource_data_section_size_original;
						subgroup_information_structure.texture_resource_data_section_offset = packet_header_structure.texture_resource_data_section_offset;
						subgroup_information_structure.texture_resource_data_section_size = packet_header_structure.texture_resource_data_section_size;
						subgroup_information_structure.texture_resource_data_section_size_original = packet_header_structure.texture_resource_data_section_size_original;
						subgroup_pool_information_structure.texture_resource_data_section_offset = packet_header_structure.generic_resource_data_section_offset + packet_header_structure.generic_resource_data_section_size_original;
						subgroup_pool_information_structure.texture_resource_data_section_size = packet_header_structure.texture_resource_data_section_size_original;
						package_data.forward(packet_data.position());
						++global_subgroup_index;
					}
					++global_group_index;
				}
				StringMapSection::adjust_sequence(information_structure.resource_path);
				StringMapSection::adjust_sequence(information_structure.group_id);
				StringMapSection::adjust_sequence(information_structure.subgroup_id);
				{
					information_data.header.write(information_structure.header);
					StringMapSection::encode(information_structure.resource_path, information_data.resource_path);
					StringMapSection::encode(information_structure.subgroup_id, information_data.subgroup_id);
					information_data.group_information.write(information_structure.group_information);
					StringMapSection::encode(information_structure.group_id, information_data.group_id);
					information_data.subgroup_information.write(information_structure.subgroup_information);
					information_data.subgroup_pool_information.write(information_structure.subgroup_pool_information);
					information_data.texture_resource_information.write(information_structure.texture_resource_information);
				}
				return;
			}

			// ----------------

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process (
				OByteStreamView &                               data_,
				Manifest::Package<version> const &              manifest,
				Optional<Description::Package<version>> const & description,
				Path const &                                    resource_directory,
				Optional<Path> const &                          packet_file,
				Optional<Path> const &                          new_packet_file
			) -> Void {
				M_use_zps_of(data);
				process_package<version>(data, manifest, description, resource_directory, packet_file, new_packet_file);
				return;
			}

			inline auto process (
				OByteStreamView &                           data_,
				Manifest::PackageVariant const &            manifest,
				Description::PackageOptionalVariant const & description,
				Path const &                                resource_directory,
				Optional<Path> const &                      packet_file,
				Optional<Path> const &                      new_packet_file,
				Version const &                             version
			) -> Void {
				M_use_zps_of(data);
				Generalization::execute<VersionEnum>(
					version,
					[&] <auto index, auto version> (ValuePackage<index, version>) {
						process<version>(data, manifest.get<Manifest::Package<version>>(), description.get<Optional<Description::Package<version>>>(), resource_directory, packet_file, new_packet_file);
					}
				);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

	namespace Unpack {

		namespace Detail {

			#pragma region using

			using namespace CommonOfPack::Detail;

			#pragma endregion

			#pragma region process

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_package_description (
				IByteStreamView &                  package_data,
				Structure::Header<version> const & header_structure,
				Description::Package<version> &    package_description
			) -> Void {
				auto group_information_data = IByteStreamView{package_data.sub_view(cbw<Size>(header_structure.description_group_section_offset), cbw<Size>(header_structure.description_resource_section_offset - header_structure.description_group_section_offset))};
				auto resource_information_data = IByteStreamView{package_data.sub_view(cbw<Size>(header_structure.description_resource_section_offset), cbw<Size>(header_structure.description_string_section_offset - header_structure.description_resource_section_offset))};
				auto string_information_data = ICharacterStreamView{from_byte_view<Character, BasicCharacterListView>(package_data.sub_view(cbw<Size>(header_structure.description_string_section_offset), cbw<Size>(header_structure.information_section_size - header_structure.description_string_section_offset)))};
				auto get_string =
					[&] (
					IntegerU32 const & offset
				) -> auto {
					auto result = CStringView{};
					string_information_data.set_position(cbw<Size>(offset));
					StringParser::read_string(string_information_data, result);
					return result;
				};
				package_description.group.allocate_full(k_none_size);
				while (!group_information_data.full()) {
					auto   group_description_information_structure = group_information_data.read_of<Structure::GroupDescriptionInformation<version>>();
					auto & group_description = package_description.group.append();
					make_original_group_id(get_string(group_description_information_structure.id_offset), group_description.value.composite, group_description.key);
					group_description.value.subgroup.allocate_full(group_description_information_structure.subgroup_information.size());
					for (auto & subgroup_index : SizeRange{group_description_information_structure.subgroup_information.size()}) {
						auto & subgroup_description_information_structure = group_description_information_structure.subgroup_information[subgroup_index];
						auto & subgroup_description = group_description.value.subgroup.at(subgroup_index);
						subgroup_description.key = get_string(subgroup_description_information_structure.id_offset);
						subgroup_category_from_data(subgroup_description.value.category, subgroup_description_information_structure.resolution, subgroup_description_information_structure.locale);
						subgroup_description.value.resource.allocate_full(subgroup_description_information_structure.resource_information.size());
						for (auto & resource_index : SizeRange{subgroup_description_information_structure.resource_information.size()}) {
							auto & resource_description_information_structure = subgroup_description_information_structure.resource_information[resource_index];
							auto & resource_description = subgroup_description.value.resource.at(resource_index);
							resource_information_data.set_position(cbw<Size>(resource_description_information_structure.detail_offset));
							auto resource_detail_description_information_structure = resource_information_data.read_of<Structure::ResourceDetailDescriptionInformation<version>>();
							resource_description.key = get_string(resource_detail_description_information_structure.id_offset);
							resource_description.value.path = Path{String{get_string(resource_detail_description_information_structure.path_offset)}};
							resource_description.value.type = cbw<Integer>(resource_detail_description_information_structure.type);
							resource_description.value.property.allocate(resource_detail_description_information_structure.property_information.size() + (resource_detail_description_information_structure.image_property_information ? (11_sz) : (0_sz)));
							if (resource_detail_description_information_structure.image_property_information) {
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

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_package (
				IByteStreamView &                         package_data,
				Manifest::Package<version> &              package_manifest,
				Optional<Description::Package<version>> & package_description,
				Optional<Path> const &                    resource_directory,
				Optional<Path> const &                    packet_file
			) -> Void {
				constexpr auto packet_version = RSGP::Version{version.number};
				assert_condition(package_data.read_of<Structure::MagicIdentifier>() == Structure::k_magic_identifier);
				assert_condition(package_data.read_of<Structure::VersionNumber>() == cbw<Structure::VersionNumber>(version.number));
				auto information_structure = Structure::Information<version>{};
				{
					package_data.read(information_structure.header);
					assert_condition(cbw<Size>(information_structure.header.group_information_section_block_size) == bs_static_size<Structure::GroupInformation<version>>());
					assert_condition(cbw<Size>(information_structure.header.subgroup_information_section_block_size) == bs_static_size<Structure::SubgroupInformation<version>>());
					assert_condition(cbw<Size>(information_structure.header.subgroup_pool_information_section_block_size) == bs_static_size<Structure::SubgroupPoolInformation<version>>());
					assert_condition(cbw<Size>(information_structure.header.texture_resource_information_section_block_size) == bs_static_size<Structure::TextureResourceInformation<version>>());
					StringMapSection::decode(information_structure.group_id, package_data.sub_view(cbw<Size>(information_structure.header.group_id_section_offset), cbw<Size>(information_structure.header.group_id_section_size)));
					StringMapSection::decode(information_structure.subgroup_id, package_data.sub_view(cbw<Size>(information_structure.header.subgroup_id_section_offset), cbw<Size>(information_structure.header.subgroup_id_section_size)));
					StringMapSection::decode(information_structure.resource_path, package_data.sub_view(cbw<Size>(information_structure.header.resource_path_section_offset), cbw<Size>(information_structure.header.resource_path_section_size)));
					package_data.set_position(cbw<Size>(information_structure.header.group_information_section_offset));
					package_data.read(information_structure.group_information, cbw<Size>(information_structure.header.group_information_section_block_count));
					package_data.set_position(cbw<Size>(information_structure.header.subgroup_information_section_offset));
					package_data.read(information_structure.subgroup_information, cbw<Size>(information_structure.header.subgroup_information_section_block_count));
					package_data.set_position(cbw<Size>(information_structure.header.subgroup_pool_information_section_offset));
					package_data.read(information_structure.subgroup_pool_information, cbw<Size>(information_structure.header.subgroup_pool_information_section_block_count));
					package_data.set_position(cbw<Size>(information_structure.header.texture_resource_information_section_offset));
					package_data.read(information_structure.texture_resource_information, cbw<Size>(information_structure.header.texture_resource_information_section_block_count));
					if (information_structure.header.description_group_section_offset != 0_iu32 || information_structure.header.description_resource_section_offset != 0_iu32 || information_structure.header.description_string_section_offset != 0_iu32) {
						assert_condition(information_structure.header.description_group_section_offset != 0_iu32 && information_structure.header.description_resource_section_offset != 0_iu32 && information_structure.header.description_string_section_offset != 0_iu32);
						process_package_description(package_data, information_structure.header, package_description.set());
					}
				}
				package_manifest.group.allocate_full(information_structure.group_information.size());
				for (auto & group_index : SizeRange{information_structure.group_information.size()}) {
					auto & group_information_structure = information_structure.group_information[group_index];
					auto & group_manifest = package_manifest.group.at(group_index);
					make_original_group_id(string_block_fixed_128_to_string(group_information_structure.id), group_manifest.value.composite, group_manifest.key);
					group_manifest.value.subgroup.allocate_full(cbw<Size>(group_information_structure.subgroup_count));
					for (auto & subgroup_index : SizeRange{cbw<Size>(group_information_structure.subgroup_count)}) {
						auto & simple_subgroup_information_structure = group_information_structure.subgroup_information[subgroup_index];
						auto & subgroup_information_structure = information_structure.subgroup_information[cbw<Size>(simple_subgroup_information_structure.index)];
						auto & subgroup_manifest = group_manifest.value.subgroup.at(subgroup_index);
						subgroup_manifest.key = string_block_fixed_128_to_string(subgroup_information_structure.id);
						subgroup_category_from_data(subgroup_manifest.value.category, simple_subgroup_information_structure.resolution, simple_subgroup_information_structure.locale);
						auto make_formatted_path =
							[&] (
							Path const & path_format
						) -> auto {
							return Path{format_string(path_format.to_string(), group_manifest.key, subgroup_manifest.key)};
						};
						auto packet_data = package_data.sub_view(cbw<Size>(subgroup_information_structure.offset), cbw<Size>(subgroup_information_structure.size));
						auto packet_stream = IByteStreamView{packet_data};
						auto packet_package_manifest = RSGP::Manifest::Package<packet_version>{};
						RSGP::Unpack::process<packet_version>(packet_stream, packet_package_manifest, resource_directory ? (make_optional_of(make_formatted_path(resource_directory.get()))) : (k_null_optional));
						assert_condition(packet_stream.full());
						if (packet_file) {
							FileSystem::write_file(make_formatted_path(packet_file.get()), packet_data);
						}
						subgroup_manifest.value.resource_data_section_store_mode = packet_package_manifest.resource_data_section_store_mode;
						subgroup_manifest.value.resource.allocate_full(packet_package_manifest.resource.size());
						for (auto & resource_index : SizeRange{packet_package_manifest.resource.size()}) {
							auto & packet_resource_manifest = packet_package_manifest.resource.at(resource_index);
							auto & resource_manifest = subgroup_manifest.value.resource.at(resource_index);
							resource_manifest.key = packet_resource_manifest.key;
							switch (packet_resource_manifest.value.additional.type()) {
								case ResourceType::generic : {
									auto & packet_resource_additional_manifest = packet_resource_manifest.value.additional.template get_of_type<ResourceType::generic>();
									auto & resource_additional_manifest = resource_manifest.value.additional.template set_of_type<ResourceType::generic>();
									break;
								}
								case ResourceType::texture : {
									auto & packet_resource_additional_manifest = packet_resource_manifest.value.additional.template get_of_type<ResourceType::texture>();
									auto & resource_additional_manifest = resource_manifest.value.additional.template set_of_type<ResourceType::texture>();
									auto & texture_information_structure = information_structure.texture_resource_information[cbw<Size>(subgroup_information_structure.texture_resource_begin_index) + cbw<Size>(packet_resource_additional_manifest.index)];
									assert_condition(cbw<Integer>(texture_information_structure.size_width) == packet_resource_additional_manifest.size.width);
									assert_condition(cbw<Integer>(texture_information_structure.size_height) == packet_resource_additional_manifest.size.height);
									resource_additional_manifest.size = packet_resource_additional_manifest.size;
									if constexpr (version.additional_texture_information_for_pvz_2_chinese_android >= 2_i) {
										resource_additional_manifest.scale = cbw<Integer>(texture_information_structure.scale);
									}
									resource_additional_manifest.format = cbw<Integer>(texture_information_structure.format);
									assert_condition(is_padded_size(cbw<Size>(texture_information_structure.bit_per_channel_of_row_division_2 * 2_iu32), cbw<Size>(texture_information_structure.size_width)));
									resource_additional_manifest.bit_per_channel = cbw<Integer>(texture_information_structure.bit_per_channel_of_row_division_2 * 2_iu32 / texture_information_structure.size_width);
									break;
								}
							}
						}
					}
				}
				return;
			}

			// ----------------

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process (
				IByteStreamView &                         data_,
				Manifest::Package<version> &              manifest,
				Optional<Description::Package<version>> & description,
				Optional<Path> const &                    resource_directory,
				Optional<Path> const &                    packet_file
			) -> Void {
				M_use_zps_of(data);
				process_package<version>(data, manifest, description, resource_directory, packet_file);
				return;
			}

			inline auto process (
				IByteStreamView &                     data_,
				Manifest::PackageVariant &            manifest,
				Description::PackageOptionalVariant & description,
				Optional<Path> const &                resource_directory,
				Optional<Path> const &                packet_file,
				Version const &                       version
			) -> Void {
				M_use_zps_of(data);
				Generalization::execute<VersionEnum>(
					version,
					[&] <auto index, auto version> (ValuePackage<index, version>) {
						process<version>(data, manifest.set<Manifest::Package<version>>(), description.set<Optional<Description::Package<version>>>(), resource_directory, packet_file);
					}
				);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

}
