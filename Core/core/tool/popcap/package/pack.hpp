#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/package/version.hpp"
#include "core/tool/popcap/package/manifest.hpp"
#include "core/tool/popcap/package/structure.hpp"
#include "core/tool/data/compression/deflate.hpp"

namespace TwinStar::Core::Tool::PopCap::Package {

	template <auto version> requires (check_version(version, {}, {}))
	struct PackCommon {

	protected:

		using Manifest = Manifest<version>;

	};

	template <auto version> requires (check_version(version, {}, {}))
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
				OByteStreamView resource_information;
			}
				information_data = {};
			{
				auto information_structure = Structure::Information<version>{};
				information_structure.resource_information.allocate_full(package_manifest.resource.size());
				for (auto & resource_index : SizeRange{package_manifest.resource.size()}) {
					auto & resource_manifest = package_manifest.resource.at(resource_index);
					auto & resource_information_structure = information_structure.resource_information[resource_index];
					resource_information_structure.path = {resource_manifest.key.to_string(CharacterType::PathSeparator::windows)};
				}
				information_data.resource_information = OByteStreamView{
					package_data.forward_view(
						[&] {
							auto size = k_none_size;
							for (auto & element : information_structure.resource_information) {
								size += bs_size(Structure::ResourceInformationListStateFlag<version>::next);
								size += bs_size(element);
							}
							size += bs_size(Structure::ResourceInformationListStateFlag<version>::done);
							return size;
						}()
					)
				};
			}
			auto information_structure = Structure::Information<version>{};
			information_structure.resource_information.allocate_full(package_manifest.resource.size());
			for (auto & resource_index : SizeRange{package_manifest.resource.size()}) {
				auto & resource_manifest = package_manifest.resource.at(resource_index);
				auto & resource_information_structure = information_structure.resource_information[resource_index];
				auto   resource_path = resource_directory / resource_manifest.key;
				resource_information_structure.path = {resource_manifest.key.to_string(CharacterType::PathSeparator::windows)};
				resource_information_structure.time = cbw<IntegerU64>(resource_manifest.value.time);
				if constexpr (version.compress_resource_data) {
					auto resource_data = FileSystem::read_file(resource_path);
					auto resource_data_stream = IByteStreamView{resource_data};
					auto resource_offset = package_data.position();
					Data::Compression::Deflate::Compress::do_process_whole(resource_data_stream, package_data, 9_sz, 15_sz, 9_sz, Data::Compression::Deflate::Strategy::Constant::default_mode(), Data::Compression::Deflate::Wrapper::Constant::zlib());
					resource_information_structure.size = cbw<IntegerU32>(package_data.position() - resource_offset);
					resource_information_structure.size_original = cbw<IntegerU32>(resource_data.size());
				} else {
					auto resource_size = FileSystem::read_stream_file(resource_path, package_data);
					resource_information_structure.size = cbw<IntegerU32>(resource_size);
				}
			}
			{
				for (auto & element : information_structure.resource_information) {
					information_data.resource_information.write(Structure::ResourceInformationListStateFlag<version>::next);
					information_data.resource_information.write(element);
				}
				information_data.resource_information.write(Structure::ResourceInformationListStateFlag<version>::done);
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

	template <auto version> requires (check_version(version, {}, {}))
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
				information_structure.resource_information.allocate(k_none_size);
				while (k_true) {
					auto flag = package_data.read_of<IntegerU8>();
					if (flag == Structure::ResourceInformationListStateFlag<version>::done) {
						break;
					}
					if (flag == Structure::ResourceInformationListStateFlag<version>::next) {
						information_structure.resource_information.append();
						package_data.read(information_structure.resource_information.last());
						continue;
					}
					assert_fail(R"(flag == /* valid */)");
				}
			}
			package_manifest.resource.allocate_full(information_structure.resource_information.size());
			for (auto & resource_index : SizeRange{information_structure.resource_information.size()}) {
				auto & resource_information_structure = information_structure.resource_information[resource_index];
				auto & resource_manifest = package_manifest.resource.at(resource_index);
				resource_manifest.key = Path{resource_information_structure.path.value};
				resource_manifest.value.time = cbw<Integer>(resource_information_structure.time);
				auto resource_data = package_data.forward_view(cbw<Size>(resource_information_structure.size));
				if constexpr (version.compress_resource_data) {
					auto resource_data_original = ByteArray{cbw<Size>(resource_information_structure.size_original)};
					auto resource_data_stream = IByteStreamView{resource_data};
					auto resource_data_original_stream = OByteStreamView{resource_data_original};
					Data::Compression::Deflate::Uncompress::do_process_whole(resource_data_stream, resource_data_original_stream, 15_sz, Data::Compression::Deflate::Wrapper::Constant::zlib());
					assert_test(resource_data_stream.full() && resource_data_original_stream.full());
					if (resource_directory) {
						FileSystem::write_file(resource_directory.get() / resource_manifest.key, resource_data_original);
					}
				} else {
					if (resource_directory) {
						FileSystem::write_file(resource_directory.get() / resource_manifest.key, resource_data);
					}
				}
			}
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
