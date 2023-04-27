#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/resource_stream_group/version.hpp"
#include "core/tool/popcap/resource_stream_group/manifest.hpp"
#include "core/tool/popcap/resource_stream_group/structure.hpp"
#include "core/tool/data/compression/deflate.hpp"

namespace TwinStar::Core::Tool::PopCap::ResourceStreamGroup {

	template <auto version> requires (check_version(version, {}))
	struct PackCommon {

	protected:

		using Manifest = Manifest<version>;

	};

	template <auto version> requires (check_version(version, {}))
	struct Pack :
		PackCommon<version> {

	protected:

		using Common = PackCommon<version>;

		using typename Common::Manifest;

		// ----------------

		static auto process_package (
			OByteStreamView &                  package_data,
			typename Manifest::Package const & package_manifest,
			Path const &                       resource_directory
		) -> Void {
			package_data.write_constant(Structure::k_magic_identifier);
			package_data.write_constant(cbw<Structure::VersionNumber>(version.number));
			struct {
				OByteStreamView header;
				OByteStreamView resource_information;
				Size            resource_information_offset;
			}
				information_data = {};
			{
				auto information_structure = Structure::Information<version>{};
				information_structure.resource_information.allocate_full(package_manifest.resource.size());
				for (auto & resource_index : SizeRange{package_manifest.resource.size()}) {
					auto & resource_manifest = package_manifest.resource.at(resource_index);
					auto & resource_information_structure = information_structure.resource_information.at(resource_index);
					resource_information_structure.key = resource_manifest.key.to_string(CharacterType::PathSeparator::windows);
					switch (resource_manifest.value.additional.type().value) {
						case ResourceType::Constant::generic().value : {
							resource_information_structure.value.type = Structure::ResourceTypeFlag<version>::generic;
							resource_information_structure.value.additional.template set_of_type<ResourceType::Constant::generic()>();
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
					package_data.forward_view(bs_size(information_structure.header))
				};
				information_data.resource_information_offset = package_data.position();
				information_data.resource_information = OByteStreamView{
					package_data.forward_view(CompiledMapData::compute_ripe_size(information_structure.resource_information))
				};
				package_data.write_space(k_null_byte, compute_padding_size(package_data.position(), k_padding_unit_size));
			}
			auto information_structure = Structure::Information<version>{};
			if constexpr (check_version(version, {1, 3})) {
				information_structure.header.unknown_1 = 1_iu32;
			}
			if constexpr (check_version(version, {3})) {
				information_structure.header.unknown_1 = 0_iu32;
			}
			information_structure.header.information_section_size = cbw<IntegerU32>(package_data.position());
			information_structure.header.resource_data_section_store_mode = resource_data_section_store_mode_to_data(package_manifest.resource_data_section_store_mode);
			information_structure.resource_information.allocate_full(package_manifest.resource.size());
			for (auto & current_resource_type : make_static_array<ResourceType>(ResourceType::Constant::generic(), ResourceType::Constant::texture())) {
				auto resource_data_section_view = VByteListView{};
				auto resource_data_section_container = ByteArray{};
				auto resource_data_section_offset = package_data.position();
				auto resource_data_section_size_original = k_none_size;
				for (auto & resource_index : SizeRange{package_manifest.resource.size()}) {
					auto & resource_manifest = package_manifest.resource.at(resource_index);
					if (resource_manifest.value.additional.type() != current_resource_type) {
						continue;
					}
					auto resource_path_full = resource_directory / resource_manifest.key;
					auto resource_size = FileSystem::size_file(resource_path_full);
					resource_data_section_size_original += compute_padded_size(resource_size, k_padding_unit_size);
				}
				auto compress_resource_data_section = k_false;
				switch (current_resource_type.value) {
					case ResourceType::Constant::generic().value : {
						compress_resource_data_section = package_manifest.resource_data_section_store_mode.compress_generic;
						break;
					}
					case ResourceType::Constant::texture().value : {
						compress_resource_data_section = package_manifest.resource_data_section_store_mode.compress_texture;
						break;
					}
				}
				if (!compress_resource_data_section) {
					resource_data_section_view = package_data.next_view(resource_data_section_size_original);
				} else {
					resource_data_section_container.allocate(resource_data_section_size_original);
					resource_data_section_view = resource_data_section_container.view();
				}
				auto resource_data_section_stream = OByteStreamView{resource_data_section_view};
				for (auto & resource_index : SizeRange{package_manifest.resource.size()}) {
					auto & resource_manifest = package_manifest.resource.at(resource_index);
					if (resource_manifest.value.additional.type() != current_resource_type) {
						continue;
					}
					auto & resource_information_structure = information_structure.resource_information.at(resource_index);
					auto   resource_path_full = resource_directory / resource_manifest.key;
					resource_information_structure.key = resource_manifest.key.to_string(CharacterType::PathSeparator::windows);
					resource_information_structure.value.offset = cbw<IntegerU32>(resource_data_section_stream.position());
					resource_information_structure.value.size = cbw<IntegerU32>(FileSystem::read_stream_file(resource_path_full, resource_data_section_stream));
					switch (resource_manifest.value.additional.type().value) {
						case ResourceType::Constant::generic().value : {
							auto & resource_additional_manifest = resource_manifest.value.additional.template get_of_type<ResourceType::Constant::generic()>();
							resource_information_structure.value.type = Structure::ResourceTypeFlag<version>::generic;
							auto & resource_additional_information_structure = resource_information_structure.value.additional.template set_of_type<ResourceType::Constant::generic()>();
							break;
						}
						case ResourceType::Constant::texture().value : {
							auto & resource_additional_manifest = resource_manifest.value.additional.template get_of_type<ResourceType::Constant::texture()>();
							resource_information_structure.value.type = Structure::ResourceTypeFlag<version>::texture;
							auto & resource_additional_information_structure = resource_information_structure.value.additional.template set_of_type<ResourceType::Constant::texture()>();
							resource_additional_information_structure.index = cbw<IntegerU32>(resource_additional_manifest.index);
							resource_additional_information_structure.size_width = cbw<IntegerU32>(resource_additional_manifest.size.width);
							resource_additional_information_structure.size_height = cbw<IntegerU32>(resource_additional_manifest.size.height);
							break;
						}
					}
					resource_data_section_stream.write_space(k_null_byte, compute_padding_size(resource_data_section_stream.position(), k_padding_unit_size));
				}
				if (!compress_resource_data_section || (current_resource_type == ResourceType::Constant::texture() && resource_data_section_size_original == k_none_size)) {
					package_data.forward(resource_data_section_view.size());
				} else {
					Data::Compression::Deflate::Compress::do_process_whole(as_lvalue(IByteStreamView{resource_data_section_view}), package_data, 9_sz, 15_sz, 9_sz, Data::Compression::Deflate::Strategy::Constant::default_mode(), Data::Compression::Deflate::Wrapper::Constant::zlib());
					package_data.write_space(k_null_byte, compute_padding_size(package_data.position(), k_padding_unit_size));
				}
				auto resource_data_section_size = package_data.position() - resource_data_section_offset;
				switch (current_resource_type.value) {
					case ResourceType::Constant::generic().value : {
						information_structure.header.generic_resource_data_section_offset = cbw<IntegerU32>(resource_data_section_offset);
						information_structure.header.generic_resource_data_section_size = cbw<IntegerU32>(resource_data_section_size);
						information_structure.header.generic_resource_data_section_size_original = cbw<IntegerU32>(resource_data_section_size_original);
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

	public:

		static auto do_process_package (
			OByteStreamView &                  package_data_,
			typename Manifest::Package const & package_manifest,
			Path const &                       resource_directory
		) -> Void {
			M_use_zps_of(package_data);
			return process_package(package_data, package_manifest, resource_directory);
		}

	};

	template <auto version> requires (check_version(version, {}))
	struct Unpack :
		PackCommon<version> {

	protected:

		using Common = PackCommon<version>;

		using typename Common::Manifest;

		// ----------------

		static auto process_package (
			IByteStreamView &            package_data,
			typename Manifest::Package & package_manifest,
			Optional<Path> const &       resource_directory
		) -> Void {
			package_data.read_constant(Structure::k_magic_identifier);
			package_data.read_constant(cbw<Structure::VersionNumber>(version.number));
			auto information_structure = Structure::Information<version>{};
			{
				package_data.read(information_structure.header);
				if constexpr (check_version(version, {1, 3})) {
					assert_test(information_structure.header.unknown_1 == 1_iu32);
				}
				if constexpr (check_version(version, {3})) {
					assert_test(information_structure.header.unknown_1 == 0_iu32);
				}
				CompiledMapData::decode(information_structure.resource_information, as_lvalue(IByteStreamView{package_data.sub_view(cbw<Size>(information_structure.header.resource_information_section_offset), cbw<Size>(information_structure.header.resource_information_section_size))}));
			}
			package_manifest.resource_data_section_store_mode = resource_data_section_store_mode_from_data(information_structure.header.resource_data_section_store_mode);
			package_manifest.resource.allocate_full(information_structure.resource_information.size());
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
						compress_resource_data_section = package_manifest.resource_data_section_store_mode.compress_generic;
						break;
					}
					case ResourceType::Constant::texture().value : {
						resource_data_section_offset = cbw<Size>(information_structure.header.texture_resource_data_section_offset);
						resource_data_section_size = cbw<Size>(information_structure.header.texture_resource_data_section_size);
						resource_data_section_size_original = cbw<Size>(information_structure.header.texture_resource_data_section_size_original);
						compress_resource_data_section = package_manifest.resource_data_section_store_mode.compress_texture;
						break;
					}
				}
				auto resource_data_section_view_stored = package_data.sub_view(resource_data_section_offset, resource_data_section_size);
				if (!compress_resource_data_section) {
					resource_data_section_view = resource_data_section_view_stored;
				} else {
					resource_data_section_container.allocate(resource_data_section_size_original);
					if (resource_data_section_size_original != k_none_size) {
						auto resource_data_section_stored_stream = IByteStreamView{resource_data_section_view_stored};
						auto resource_data_section_original_stream = OByteStreamView{resource_data_section_container};
						Data::Compression::Deflate::Uncompress::do_process_whole(resource_data_section_stored_stream, resource_data_section_original_stream, 15_sz, Data::Compression::Deflate::Wrapper::Constant::zlib());
					}
					resource_data_section_view = resource_data_section_container.view();
				}
				for (auto & resource_index : SizeRange{information_structure.resource_information.size()}) {
					auto & resource_information_structure = information_structure.resource_information.at(resource_index);
					if (resource_information_structure.value.additional.type() != current_resource_type) {
						continue;
					}
					auto & resource_manifest = package_manifest.resource.at(resource_index);
					resource_manifest.key.from_string(resource_information_structure.key);
					switch (resource_information_structure.value.additional.type().value) {
						case ResourceType::Constant::generic().value : {
							auto & resource_additional_information_structure = resource_information_structure.value.additional.template get_of_type<ResourceType::Constant::generic()>();
							auto & resource_additional_manifest = resource_manifest.value.additional.template set_of_type<ResourceType::Constant::generic()>();
							break;
						}
						case ResourceType::Constant::texture().value : {
							auto & resource_additional_information_structure = resource_information_structure.value.additional.template get_of_type<ResourceType::Constant::texture()>();
							auto & resource_additional_manifest = resource_manifest.value.additional.template set_of_type<ResourceType::Constant::texture()>();
							resource_additional_manifest.index = cbw<Integer>(resource_additional_information_structure.index);
							resource_additional_manifest.size.width = cbw<Integer>(resource_additional_information_structure.size_width);
							resource_additional_manifest.size.height = cbw<Integer>(resource_additional_information_structure.size_height);
							break;
						}
					}
					auto resource_data = resource_data_section_view.sub(cbw<Size>(resource_information_structure.value.offset), cbw<Size>(resource_information_structure.value.size));
					if (resource_directory.has()) {
						FileSystem::write_file(resource_directory.get() / resource_manifest.key, resource_data);
					}
				}
			}
			package_data.set_position(
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

	public:

		static auto do_process_package (
			IByteStreamView &            package_data_,
			typename Manifest::Package & package_manifest,
			Optional<Path> const &       resource_directory
		) -> Void {
			M_use_zps_of(package_data);
			restruct(package_manifest);
			return process_package(package_data, package_manifest, resource_directory);
		}

	};

}
