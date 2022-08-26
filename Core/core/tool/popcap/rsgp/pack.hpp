#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/rsgp/version.hpp"
#include "core/tool/popcap/rsgp/manifest.hpp"
#include "core/tool/popcap/rsgp/structure.hpp"
#include "core/tool/data/compress/deflate.hpp"

namespace TwinKleS::Core::Tool::PopCap::RSGP {

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
			inline auto process_package (
				OByteStreamView &                  package_data,
				Manifest::Package<version> const & package_manifest,
				Path const &                       resource_directory
			) -> Void {
				package_data.write(Structure::k_magic_identifier);
				package_data.write(cbw<Structure::VersionNumber>(version.number));
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
						switch (resource_manifest.value.additional.type()) {
							case ResourceType::generic : {
								resource_information_structure.value.type = Structure::ResourceTypeFlag<version>::generic;
								resource_information_structure.value.additional.template set_of_type<ResourceType::generic>();
								break;
							}
							case ResourceType::texture : {
								resource_information_structure.value.type = Structure::ResourceTypeFlag<version>::texture;
								resource_information_structure.value.additional.template set_of_type<ResourceType::texture>();
								break;
							}
						}
					}
					information_data.header = OByteStreamView{
						package_data.forward_view(bs_size(information_structure.header))
					};
					information_data.resource_information_offset = package_data.position();
					information_data.resource_information = OByteStreamView{
						package_data.forward_view(StringMapSection::compute_ripe_size(information_structure.resource_information))
					};
					package_data.pad_auto(k_padding_unit_size);
				}
				auto information_structure = Structure::Information<version>{};
				information_structure.header.information_section_size = cbw<IntegerU32>(package_data.position());
				information_structure.header.resource_data_section_store_mode = resource_data_section_store_mode_to_data(package_manifest.resource_data_section_store_mode);
				information_structure.resource_information.allocate_full(package_manifest.resource.size());
				for (auto & current_resource_type : StaticArray{{ResourceType::generic, ResourceType::texture}}) {
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
					switch (current_resource_type) {
						case ResourceType::generic : {
							compress_resource_data_section = package_manifest.resource_data_section_store_mode.compress_generic;
							break;
						}
						case ResourceType::texture : {
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
						switch (resource_manifest.value.additional.type()) {
							case ResourceType::generic : {
								auto & resource_additional_manifest = resource_manifest.value.additional.template get_of_type<ResourceType::generic>();
								resource_information_structure.value.type = Structure::ResourceTypeFlag<version>::generic;
								auto & resource_additional_information_structure = resource_information_structure.value.additional.template set_of_type<ResourceType::generic>();
								break;
							}
							case ResourceType::texture : {
								auto & resource_additional_manifest = resource_manifest.value.additional.template get_of_type<ResourceType::texture>();
								resource_information_structure.value.type = Structure::ResourceTypeFlag<version>::texture;
								auto & resource_additional_information_structure = resource_information_structure.value.additional.template set_of_type<ResourceType::texture>();
								resource_additional_information_structure.index = cbw<IntegerU32>(resource_additional_manifest.index);
								resource_additional_information_structure.size_width = cbw<IntegerU32>(resource_additional_manifest.size.width);
								resource_additional_information_structure.size_height = cbw<IntegerU32>(resource_additional_manifest.size.height);
								break;
							}
						}
						resource_data_section_stream.pad_auto(k_padding_unit_size);
					}
					if (!compress_resource_data_section || (current_resource_type == ResourceType::texture && resource_data_section_size_original == k_none_size)) {
						package_data.forward(resource_data_section_view.size());
					} else {
						Data::Compress::Deflate::Compress::process(as_lvalue(IByteStreamView{resource_data_section_view}), package_data, 9_sz, 15_sz, 9_sz, Data::Compress::Deflate::Strategy::default_mode, Data::Compress::Deflate::Wrapper::zlib);
						package_data.pad_auto(k_padding_unit_size);
					}
					auto resource_data_section_size = package_data.position() - resource_data_section_offset;
					switch (current_resource_type) {
						case ResourceType::generic : {
							information_structure.header.generic_resource_data_section_offset = cbw<IntegerU32>(resource_data_section_offset);
							information_structure.header.generic_resource_data_section_size = cbw<IntegerU32>(resource_data_section_size);
							information_structure.header.generic_resource_data_section_size_original = cbw<IntegerU32>(resource_data_section_size_original);
							break;
						}
						case ResourceType::texture : {
							information_structure.header.texture_resource_data_section_offset = cbw<IntegerU32>(resource_data_section_offset);
							information_structure.header.texture_resource_data_section_size = cbw<IntegerU32>(resource_data_section_size);
							information_structure.header.texture_resource_data_section_size_original = cbw<IntegerU32>(resource_data_section_size_original);
							break;
						}
					}
				}
				information_structure.header.resource_information_section_offset = cbw<IntegerU32>(information_data.resource_information_offset);
				information_structure.header.resource_information_section_size = cbw<IntegerU32>(information_data.resource_information.size());
				{
					information_data.header.write(information_structure.header);
					StringMapSection::encode(information_structure.resource_information, information_data.resource_information);
				}
				return;
			}

			// ----------------

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process (
				OByteStreamView &                  data_,
				Manifest::Package<version> const & manifest,
				Path const &                       resource_directory
			) -> Void {
				M_use_zps_of(data);
				process_package<version>(data, manifest, resource_directory);
				return;
			}

			inline auto process (
				OByteStreamView &                data_,
				Manifest::PackageVariant const & manifest,
				Path const &                     resource_directory,
				Version const &                  version
			) -> Void {
				M_use_zps_of(data);
				Generalization::execute<VersionEnum>(
					version,
					[&] <auto index, auto version> (ValuePackage<index, version>) {
						process<version>(data, manifest.get<Manifest::Package<version>>(), resource_directory);
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
			inline auto process_package (
				IByteStreamView &            package_data,
				Manifest::Package<version> & package_manifest,
				Optional<Path> const &       resource_directory
			) -> Void {
				assert_condition(package_data.read_of<Structure::MagicIdentifier>() == Structure::k_magic_identifier);
				assert_condition(package_data.read_of<Structure::VersionNumber>() == cbw<Structure::VersionNumber>(version.number));
				auto information_structure = Structure::Information<version>{};
				{
					package_data.read(information_structure.header);
					StringMapSection::decode(information_structure.resource_information, package_data.sub_view(cbw<Size>(information_structure.header.resource_information_section_offset), cbw<Size>(information_structure.header.resource_information_section_size)));
				}
				package_manifest.resource_data_section_store_mode = resource_data_section_store_mode_from_data(information_structure.header.resource_data_section_store_mode);
				package_manifest.resource.allocate_full(information_structure.resource_information.size());
				for (auto & current_resource_type : StaticArray{{ResourceType::generic, ResourceType::texture}}) {
					auto resource_data_section_view = CByteListView{};
					auto resource_data_section_container = ByteArray{};
					auto resource_data_section_offset = Size{};
					auto resource_data_section_size = Size{};
					auto resource_data_section_size_original = Size{};
					auto compress_resource_data_section = k_false;
					switch (current_resource_type) {
						case ResourceType::generic : {
							resource_data_section_offset = cbw<Size>(information_structure.header.generic_resource_data_section_offset);
							resource_data_section_size = cbw<Size>(information_structure.header.generic_resource_data_section_size);
							resource_data_section_size_original = cbw<Size>(information_structure.header.generic_resource_data_section_size_original);
							compress_resource_data_section = package_manifest.resource_data_section_store_mode.compress_generic;
							break;
						}
						case ResourceType::texture : {
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
							Data::Compress::Deflate::Uncompress::process(resource_data_section_stored_stream, resource_data_section_original_stream, 15_sz, Data::Compress::Deflate::Wrapper::zlib);
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
						switch (resource_information_structure.value.additional.type()) {
							case ResourceType::generic : {
								auto & resource_additional_information_structure = resource_information_structure.value.additional.template get_of_type<ResourceType::generic>();
								auto & resource_additional_manifest = resource_manifest.value.additional.template set_of_type<ResourceType::generic>();
								break;
							}
							case ResourceType::texture : {
								auto & resource_additional_information_structure = resource_information_structure.value.additional.template get_of_type<ResourceType::texture>();
								auto & resource_additional_manifest = resource_manifest.value.additional.template set_of_type<ResourceType::texture>();
								resource_additional_manifest.index = cbw<Integer>(resource_additional_information_structure.index);
								resource_additional_manifest.size.width = cbw<Integer>(resource_additional_information_structure.size_width);
								resource_additional_manifest.size.height = cbw<Integer>(resource_additional_information_structure.size_height);
								break;
							}
						}
						auto resource_data = resource_data_section_view.sub(cbw<Size>(resource_information_structure.value.offset), cbw<Size>(resource_information_structure.value.size));
						if (resource_directory) {
							FileSystem::write_file(resource_directory.get() / resource_manifest.key, resource_data);
						}
					}
				}
				package_data.set_position(
					max(
						{
							cbw<Size>(information_structure.header.information_section_size),
							cbw<Size>(information_structure.header.generic_resource_data_section_offset + information_structure.header.generic_resource_data_section_size),
							cbw<Size>(information_structure.header.texture_resource_data_section_offset + information_structure.header.texture_resource_data_section_size),
						}
					)
				);
				return;
			}

			// ----------------

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process (
				IByteStreamView &            data_,
				Manifest::Package<version> & manifest,
				Optional<Path> const &       resource_directory
			) -> Void {
				M_use_zps_of(data);
				process_package<version>(data, manifest, resource_directory);
				return;
			}

			inline auto process (
				IByteStreamView &          data_,
				Manifest::PackageVariant & manifest,
				Optional<Path> const &     resource_directory,
				Version const &            version
			) -> Void {
				M_use_zps_of(data);
				Generalization::execute<VersionEnum>(
					version,
					[&] <auto index, auto version> (ValuePackage<index, version>) {
						process<version>(data, manifest.set<Manifest::Package<version>>(), resource_directory);
					}
				);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

}
