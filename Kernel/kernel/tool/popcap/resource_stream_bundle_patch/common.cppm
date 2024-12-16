module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_bundle_patch.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_bundle_patch.version;
import twinning.kernel.tool.popcap.resource_stream_group.version;
import twinning.kernel.tool.popcap.resource_stream_group.structure;
import twinning.kernel.tool.popcap.resource_stream_bundle.version;
import twinning.kernel.tool.popcap.resource_stream_bundle.structure;
import twinning.kernel.tool.popcap.resource_stream_bundle.shared;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.tool.data.compression.deflate.compress;
import twinning.kernel.tool.data.compression.deflate.uncompress;

export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamBundlePatch {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using MagicIdentifier = IntegerU32;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{0x52534250_iu32};

		// ----------------

		using VersionNumber = IntegerU32;

		// ----------------

		M_record_of_data(
			M_wrap(PackageInformation),
			M_wrap(
				(ConstantBlock<0x00000002_iu32>) unknown_1,
				(IntegerU32) all_after_size,
				(ConstantBlock<0x00000000_iu32>) unknown_3,
				(IntegerU32) patch_size,
				(StaticByteArray<16_sz>) before_hash,
				(IntegerU32) packet_count,
				(IntegerU32) patch_exist,
			),
		);

		M_record_of_data(
			M_wrap(PacketInformation),
			M_wrap(
				(IntegerU32) patch_exist,
				(IntegerU32) patch_size,
				(StaticCharacterArray<128_sz>) name,
				(StaticByteArray<16_sz>) before_hash,
			),
		);

		// ----------------

		inline static constexpr auto package_version = ResourceStreamBundle::Version{.number = 4_i, .extended_texture_information_for_pvz2_cn = 0_i};

		inline static auto read_package_information_structure (
			IByteStreamView &                                               data,
			ResourceStreamBundle::Structure::Information<package_version> & information_structure
		) -> Void {
			data.read_constant(ResourceStreamBundle::Structure::k_magic_identifier);
			data.read_constant(cbox<ResourceStreamBundle::Structure::VersionNumber>(package_version.number));
			data.read(information_structure.header);
			assert_test(cbox<Size>(information_structure.header.group_information_section_block_size) == bs_static_size<ResourceStreamBundle::Structure::GroupInformation<package_version>>());
			assert_test(cbox<Size>(information_structure.header.subgroup_information_section_block_size) == bs_static_size<ResourceStreamBundle::Structure::SubgroupInformation<package_version>>());
			assert_test(cbox<Size>(information_structure.header.pool_information_section_block_size) == bs_static_size<ResourceStreamBundle::Structure::PoolInformation<package_version>>());
			assert_test(cbox<Size>(information_structure.header.texture_resource_information_section_block_size) == bs_static_size<ResourceStreamBundle::Structure::TextureResourceInformation<package_version>>());
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
			assert_test(information_structure.group_id.size() == cbox<Size>(information_structure.header.group_information_section_block_count));
			assert_test(information_structure.subgroup_id.size() == cbox<Size>(information_structure.header.subgroup_information_section_block_count));
			return;
		}

		// ----------------

		inline static constexpr auto packet_version = ResourceStreamGroup::Version{.number = 4_i};

		inline static auto compress_packet (
			IByteStreamView & raw,
			OByteStreamView & ripe
		) -> ResourceStreamGroup::Structure::Information<packet_version> {
			raw.read_constant(ResourceStreamGroup::Structure::k_magic_identifier);
			raw.read_constant(cbox<ResourceStreamGroup::Structure::VersionNumber>(packet_version.number));
			auto information_structure = ResourceStreamGroup::Structure::Information<packet_version>{};
			{
				raw.read(information_structure.header);
				CompiledMapData::decode(information_structure.resource_information, as_lvalue(IByteStreamView{raw.sub_view(cbox<Size>(information_structure.header.resource_information_section_offset), cbox<Size>(information_structure.header.resource_information_section_size))}));
			}
			raw.set_position(
				cbox<Size>(
					maximum(
						make_initializer_list(
							{
								information_structure.header.information_section_size,
								information_structure.header.general_resource_data_section_offset + information_structure.header.general_resource_data_section_size_original,
								information_structure.header.texture_resource_data_section_offset + information_structure.header.texture_resource_data_section_size_original,
							}
						)
					)
				)
			);
			ripe.write(raw.sub_view(k_begin_index, cbox<Size>(information_structure.header.information_section_size)));
			ripe.write_space(k_null_byte, compute_padding_size(ripe.position(), k_padding_unit_size));
			auto resource_data_section_compression = packet_compression_from_data(information_structure.header.resource_data_section_compression);
			{
				information_structure.header.general_resource_data_section_offset = cbox<IntegerU32>(ripe.position());
				auto resource_data_section_view = raw.sub_view(cbox<Size>(information_structure.header.general_resource_data_section_offset), cbox<Size>(information_structure.header.general_resource_data_section_size_original));
				if (!resource_data_section_compression.general) {
					ripe.write(resource_data_section_view);
				}
				else {
					Data::Compression::Deflate::Compress::process(as_lvalue(IByteStreamView{resource_data_section_view}), ripe, 9_sz, 15_sz, 9_sz, Data::Compression::Deflate::Strategy::Constant::default_mode(), Data::Compression::Deflate::Wrapper::Constant::zlib());
					ripe.write_space(k_null_byte, compute_padding_size(ripe.position(), k_padding_unit_size));
				}
				information_structure.header.general_resource_data_section_size = cbox<IntegerU32>(ripe.position()) - information_structure.header.general_resource_data_section_offset;
			}
			{
				information_structure.header.texture_resource_data_section_offset = cbox<IntegerU32>(ripe.position());
				auto resource_data_section_view = raw.sub_view(cbox<Size>(information_structure.header.texture_resource_data_section_offset), cbox<Size>(information_structure.header.texture_resource_data_section_size_original));
				if (!resource_data_section_compression.texture || cbox<Size>(information_structure.header.texture_resource_data_section_size_original) == k_none_size) {
					ripe.write(resource_data_section_view);
				}
				else {
					Data::Compression::Deflate::Compress::process(as_lvalue(IByteStreamView{resource_data_section_view}), ripe, 9_sz, 15_sz, 9_sz, Data::Compression::Deflate::Strategy::Constant::default_mode(), Data::Compression::Deflate::Wrapper::Constant::zlib());
					ripe.write_space(k_null_byte, compute_padding_size(ripe.position(), k_padding_unit_size));
				}
				information_structure.header.texture_resource_data_section_size = cbox<IntegerU32>(ripe.position()) - information_structure.header.texture_resource_data_section_offset;
			}
			OByteStreamView{ripe.sub_view(bs_static_size<ResourceStreamGroup::Structure::MagicIdentifier>() + bs_static_size<ResourceStreamGroup::Structure::VersionNumber>(), bs_size(information_structure.header))}.write(information_structure.header);
			return information_structure;
		}

		inline static auto uncompress_packet (
			IByteStreamView & ripe,
			OByteStreamView & raw
		) -> ResourceStreamGroup::Structure::Information<packet_version> {
			ripe.read_constant(ResourceStreamGroup::Structure::k_magic_identifier);
			ripe.read_constant(cbox<ResourceStreamGroup::Structure::VersionNumber>(packet_version.number));
			auto information_structure = ResourceStreamGroup::Structure::Information<packet_version>{};
			{
				ripe.read(information_structure.header);
				CompiledMapData::decode(information_structure.resource_information, as_lvalue(IByteStreamView{ripe.sub_view(cbox<Size>(information_structure.header.resource_information_section_offset), cbox<Size>(information_structure.header.resource_information_section_size))}));
			}
			ripe.set_position(
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
			raw.write(ripe.sub_view(k_begin_index, cbox<Size>(information_structure.header.information_section_size)));
			raw.write_space(k_null_byte, compute_padding_size(raw.position(), k_padding_unit_size));
			auto resource_data_section_compression = packet_compression_from_data(information_structure.header.resource_data_section_compression);
			{
				auto resource_data_section_view = ripe.sub_view(cbox<Size>(information_structure.header.general_resource_data_section_offset), cbox<Size>(information_structure.header.general_resource_data_section_size));
				if (!resource_data_section_compression.general) {
					raw.write(resource_data_section_view);
				}
				else {
					Data::Compression::Deflate::Uncompress::process(as_lvalue(IByteStreamView{resource_data_section_view}), raw, 15_sz, Data::Compression::Deflate::Wrapper::Constant::zlib());
				}
				raw.write_space(k_null_byte, compute_padding_size(raw.position(), k_padding_unit_size));
			}
			{
				auto resource_data_section_view = ripe.sub_view(cbox<Size>(information_structure.header.texture_resource_data_section_offset), cbox<Size>(information_structure.header.texture_resource_data_section_size));
				if (!resource_data_section_compression.texture || cbox<Size>(information_structure.header.texture_resource_data_section_size_original) == k_none_size) {
					raw.write(resource_data_section_view);
				}
				else {
					Data::Compression::Deflate::Uncompress::process(as_lvalue(IByteStreamView{resource_data_section_view}), raw, 15_sz, Data::Compression::Deflate::Wrapper::Constant::zlib());
				}
				raw.write_space(k_null_byte, compute_padding_size(raw.position(), k_padding_unit_size));
			}
			return information_structure;
		}

		// ----------------

		inline static auto indexing_subgroup_information_by_id (
			List<ResourceStreamBundle::Structure::SubgroupInformation<package_version>> const & list
		) -> Map<String, Size> {
			auto index_map = Map<String, Size>{};
			index_map.allocate(list.size());
			for (auto & index : SizeRange{list.size()}) {
				auto & item = list[index];
				index_map.append(String{item.id.begin(), null_terminated_string_size_of(item.id.begin())}, index);
			}
			return index_map;
		}

	};

}
