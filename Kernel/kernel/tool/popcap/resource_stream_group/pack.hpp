#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/popcap/resource_stream_group/common.hpp"
#include "kernel/tool/data/compression/deflate/compress.hpp"

namespace TwinStar::Kernel::Tool::PopCap::ResourceStreamGroup {

	template <auto version> requires (check_version(version, {}))
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
				OByteStreamView header{};
				OByteStreamView resource_information{};
				Size            resource_information_offset{};
			} information_data = {};
			{
				auto information_structure = Structure::Information<version>{};
				information_structure.resource_information.allocate_full(definition.resource.size());
				for (auto & resource_index : SizeRange{definition.resource.size()}) {
					auto & resource_definition = definition.resource[resource_index];
					auto & resource_information_structure = information_structure.resource_information.at(resource_index);
					resource_information_structure.key = resource_definition.path.to_string(CharacterType::PathSeparator::windows);
					switch (resource_definition.additional.type().value) {
						case ResourceType::Constant::general().value : {
							resource_information_structure.value.type = Structure::ResourceTypeFlag<version>::general;
							resource_information_structure.value.additional.template set_of_type<ResourceType::Constant::general()>();
							break;
						}
						case ResourceType::Constant::texture().value : {
							resource_information_structure.value.type = Structure::ResourceTypeFlag<version>::texture;
							resource_information_structure.value.additional.template set_of_type<ResourceType::Constant::texture()>();
							break;
						}
					}
				}
				CompiledMapData::adjust_sequence(information_structure.resource_information);
				information_data.header = OByteStreamView{
					data.forward_view(bs_size(information_structure.header))
				};
				information_data.resource_information_offset = data.position();
				information_data.resource_information = OByteStreamView{
					data.forward_view(CompiledMapData::compute_ripe_size(information_structure.resource_information))
				};
				data.write_space(k_null_byte, compute_padding_size(data.position(), k_padding_unit_size));
			}
			auto information_structure = Structure::Information<version>{};
			if constexpr (check_version(version, {1, 3})) {
				information_structure.header.unknown_1 = 1_iu32;
			}
			if constexpr (check_version(version, {3})) {
				information_structure.header.unknown_1 = 0_iu32;
			}
			information_structure.header.information_section_size = cbw<IntegerU32>(data.position());
			information_structure.header.resource_data_section_compression = packet_compression_to_data(definition.compression);
			information_structure.resource_information.allocate_full(definition.resource.size());
			for (auto & current_resource_type : make_static_array<ResourceType>(ResourceType::Constant::general(), ResourceType::Constant::texture())) {
				auto resource_data_section_view = VByteListView{};
				auto resource_data_section_container = ByteArray{};
				auto resource_data_section_offset = data.position();
				auto resource_data_section_size_original = k_none_size;
				for (auto & resource_index : SizeRange{definition.resource.size()}) {
					auto & resource_definition = definition.resource[resource_index];
					if (resource_definition.additional.type() != current_resource_type) {
						continue;
					}
					auto resource_path_full = resource_directory / resource_definition.path;
					auto resource_size = FileSystem::size_file(resource_path_full);
					resource_data_section_size_original += compute_padded_size(resource_size, k_padding_unit_size);
				}
				auto compress_resource_data_section = k_false;
				switch (current_resource_type.value) {
					case ResourceType::Constant::general().value : {
						compress_resource_data_section = definition.compression.general;
						break;
					}
					case ResourceType::Constant::texture().value : {
						compress_resource_data_section = definition.compression.texture;
						break;
					}
				}
				if (!compress_resource_data_section) {
					resource_data_section_view = data.next_view(resource_data_section_size_original);
				}
				else {
					resource_data_section_container.allocate(resource_data_section_size_original);
					resource_data_section_view = resource_data_section_container.view();
				}
				auto resource_data_section_stream = OByteStreamView{resource_data_section_view};
				for (auto & resource_index : SizeRange{definition.resource.size()}) {
					auto & resource_definition = definition.resource[resource_index];
					if (resource_definition.additional.type() != current_resource_type) {
						continue;
					}
					auto & resource_information_structure = information_structure.resource_information.at(resource_index);
					auto   resource_path_full = resource_directory / resource_definition.path;
					resource_information_structure.key = resource_definition.path.to_string(CharacterType::PathSeparator::windows);
					resource_information_structure.value.offset = cbw<IntegerU32>(resource_data_section_stream.position());
					resource_information_structure.value.size = cbw<IntegerU32>(FileSystem::read_stream_file(resource_path_full, resource_data_section_stream));
					switch (resource_definition.additional.type().value) {
						case ResourceType::Constant::general().value : {
							auto & resource_additional_definition = resource_definition.additional.template get_of_type<ResourceType::Constant::general()>();
							resource_information_structure.value.type = Structure::ResourceTypeFlag<version>::general;
							auto & resource_additional_information_structure = resource_information_structure.value.additional.template set_of_type<ResourceType::Constant::general()>();
							break;
						}
						case ResourceType::Constant::texture().value : {
							auto & resource_additional_definition = resource_definition.additional.template get_of_type<ResourceType::Constant::texture()>();
							resource_information_structure.value.type = Structure::ResourceTypeFlag<version>::texture;
							auto & resource_additional_information_structure = resource_information_structure.value.additional.template set_of_type<ResourceType::Constant::texture()>();
							resource_additional_information_structure.index = cbw<IntegerU32>(resource_additional_definition.index);
							resource_additional_information_structure.size_width = cbw<IntegerU32>(resource_additional_definition.size.width);
							resource_additional_information_structure.size_height = cbw<IntegerU32>(resource_additional_definition.size.height);
							break;
						}
					}
					resource_data_section_stream.write_space(k_null_byte, compute_padding_size(resource_data_section_stream.position(), k_padding_unit_size));
				}
				if (!compress_resource_data_section || (current_resource_type == ResourceType::Constant::texture() && resource_data_section_size_original == k_none_size)) {
					data.forward(resource_data_section_view.size());
				}
				else {
					Data::Compression::Deflate::Compress::process(as_lvalue(IByteStreamView{resource_data_section_view}), data, 9_sz, 15_sz, 9_sz, Data::Compression::Deflate::Strategy::Constant::default_mode(), Data::Compression::Deflate::Wrapper::Constant::zlib());
					data.write_space(k_null_byte, compute_padding_size(data.position(), k_padding_unit_size));
				}
				auto resource_data_section_size = data.position() - resource_data_section_offset;
				switch (current_resource_type.value) {
					case ResourceType::Constant::general().value : {
						information_structure.header.general_resource_data_section_offset = cbw<IntegerU32>(resource_data_section_offset);
						information_structure.header.general_resource_data_section_size = cbw<IntegerU32>(resource_data_section_size);
						information_structure.header.general_resource_data_section_size_original = cbw<IntegerU32>(resource_data_section_size_original);
						break;
					}
					case ResourceType::Constant::texture().value : {
						information_structure.header.texture_resource_data_section_offset = cbw<IntegerU32>(resource_data_section_offset);
						information_structure.header.texture_resource_data_section_size = cbw<IntegerU32>(resource_data_section_size);
						information_structure.header.texture_resource_data_section_size_original = cbw<IntegerU32>(resource_data_section_size_original);
						break;
					}
				}
			}
			information_structure.header.resource_information_section_offset = cbw<IntegerU32>(information_data.resource_information_offset);
			information_structure.header.resource_information_section_size = cbw<IntegerU32>(information_data.resource_information.size());
			CompiledMapData::adjust_sequence(information_structure.resource_information);
			{
				information_data.header.write(information_structure.header);
				CompiledMapData::encode(information_structure.resource_information, information_data.resource_information);
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
