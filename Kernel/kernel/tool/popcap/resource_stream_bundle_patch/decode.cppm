module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_bundle_patch.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_bundle_patch.version;
import twinning.kernel.tool.popcap.resource_stream_bundle_patch.common;
import twinning.kernel.tool.popcap.resource_stream_bundle.structure;
import twinning.kernel.tool.data.hash.md5.common;
import twinning.kernel.tool.data.hash.md5.hash;
import twinning.kernel.tool.data.differentiation.vcdiff.common;
import twinning.kernel.tool.data.differentiation.vcdiff.encode;
import twinning.kernel.tool.data.differentiation.vcdiff.decode;

export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamBundlePatch {

	template <auto version> requires (check_version(version, {}))
	struct Decode :
		Common<version> {

		using Common = Common<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::VersionNumber;

		using typename Common::PackageInformation;

		using typename Common::PacketInformation;

		using Common::package_version;

		using Common::read_package_information_structure;

		using Common::packet_version;

		using Common::compress_packet;

		using Common::uncompress_packet;

		using Common::indexing_subgroup_information_by_id;

		// ----------------

		inline static auto test_hash (
			CByteListView const &          data,
			StaticByteArray<16_sz> const & hash
		) -> Void {
			auto hash_test = ByteArray{};
			Data::Hash::MD5::Hash::process(data, hash_test);
			assert_test(hash_test.as_view() == hash.view());
			return;
		}

		inline static auto process_sub (
			CByteListView const & before,
			OByteStreamView &     after,
			IByteStreamView &     patch,
			Size const &          patch_size
		) -> Void {
			auto before_stream = IByteStreamView{before};
			auto patch_stream = IByteStreamView{patch.forward_view(patch_size)};
			Data::Differentiation::VCDiff::Decode::process(before_stream, after, patch_stream, 0x7FFFFFFF_sz);
			return;
		}

		inline static auto process_whole (
			IByteStreamView & before,
			OByteStreamView & after,
			IByteStreamView & patch,
			Boolean const &   use_raw_packet
		) -> Void {
			patch.read_constant(k_magic_identifier);
			patch.read_constant(cbox<VersionNumber>(version.number));
			auto package_information = PackageInformation{};
			patch.read(package_information);
			auto packet_count = cbox<Size>(package_information.packet_count);
			auto information_section_patch_exist = cbox<Boolean>(package_information.patch_exist);
			auto information_section_patch_size = cbox<Size>(package_information.patch_size);
			auto information_section_before_structure = ResourceStreamBundle::Structure::Information<package_version>{};
			auto information_section_after_structure = ResourceStreamBundle::Structure::Information<package_version>{};
			read_package_information_structure(as_lvalue(IByteStreamView{before.view()}), information_section_before_structure);
			auto information_section_before = before.sub_view(k_begin_index, cbox<Size>(information_section_before_structure.header.information_section_size));
			test_hash(information_section_before, package_information.before_hash);
			if (!information_section_patch_exist) {
				assert_test(information_section_patch_size == k_none_size);
				after.write(information_section_before);
			}
			else {
				process_sub(information_section_before, after, patch, information_section_patch_size);
			}
			read_package_information_structure(as_lvalue(IByteStreamView{after.view()}), information_section_after_structure);
			assert_test(packet_count == information_section_after_structure.subgroup_information.size());
			auto packet_before_subgroup_information_index_map = indexing_subgroup_information_by_id(information_section_before_structure.subgroup_information);
			auto packet_before_raw_container = ByteArray{};
			auto packet_after_raw_container = ByteArray{};
			auto before_end_position = cbox<Size>(information_section_before_structure.header.information_section_size);
			for (auto & packet_index : SizeRange{packet_count}) {
				auto & packet_after_subgroup_information = information_section_after_structure.subgroup_information[packet_index];
				auto   packet_information = PacketInformation{};
				patch.read(packet_information);
				auto packet_name = String{packet_information.name.begin(), null_terminated_string_size_of(packet_information.name.begin())};
				auto packet_patch_exist = cbox<Boolean>(packet_information.patch_exist);
				auto packet_patch_size = cbox<Size>(packet_information.patch_size);
				{
					auto packet_name_upper = packet_name;
					packet_name_upper.as_upper_case();
					auto packet_after_subgroup_index = cbox<Size>(information_section_after_structure.subgroup_id[packet_name_upper]);
					assert_test(packet_after_subgroup_index == packet_index);
				}
				auto packet_before = CByteListView{};
				if (auto packet_before_subgroup_information_index = packet_before_subgroup_information_index_map.query_if(packet_name)) {
					auto & packet_before_subgroup_information = information_section_before_structure.subgroup_information[packet_before_subgroup_information_index.get().value];
					auto   packet_before_ripe = IByteStreamView{before.sub_view(cbox<Size>(packet_before_subgroup_information.offset), cbox<Size>(packet_before_subgroup_information.size))};
					if (!use_raw_packet) {
						packet_before = packet_before_ripe.view();
					}
					else {
						auto packet_before_raw_size = cbox<Size>(packet_before_subgroup_information.information_section_size + packet_before_subgroup_information.general_resource_data_section_size_original + packet_before_subgroup_information.texture_resource_data_section_size_original);
						if (packet_before_raw_size > packet_before_raw_container.size()) {
							packet_before_raw_container.allocate(packet_before_raw_size);
						}
						auto packet_before_raw = OByteStreamView{packet_before_raw_container};
						uncompress_packet(packet_before_ripe, packet_before_raw);
						assert_test(packet_before_ripe.full());
						packet_before = packet_before_raw.stream_view();
					}
					before_end_position = maximum(before_end_position, cbox<Size>(packet_before_subgroup_information.offset + packet_before_subgroup_information.size));
				}
				test_hash(packet_before, packet_information.before_hash);
				auto packet_after = CByteListView{};
				if (!packet_patch_exist) {
					assert_test(packet_patch_size == k_none_size);
					packet_after = packet_before;
				}
				else {
					auto packet_after_raw_size = cbox<Size>(packet_after_subgroup_information.information_section_size + packet_after_subgroup_information.general_resource_data_section_size_original + packet_after_subgroup_information.texture_resource_data_section_size_original);
					if (packet_after_raw_size > packet_after_raw_container.size()) {
						packet_after_raw_container.allocate(packet_after_raw_size);
					}
					auto packet_after_raw = OByteStreamView{packet_after_raw_container};
					process_sub(packet_before, packet_after_raw, patch, packet_patch_size);
					packet_after = packet_after_raw.stream_view();
				}
				if (!use_raw_packet) {
					after.write(packet_after);
				}
				else {
					auto packet_after_raw = IByteStreamView{packet_after};
					auto packet_after_ripe = OByteStreamView{after.reserve_view()};
					auto packet_after_information_structure = compress_packet(packet_after_raw, packet_after_ripe);
					assert_test(packet_after_raw.full());
					packet_after_subgroup_information.offset = cbox<IntegerU32>(after.position());
					packet_after_subgroup_information.size = maximum(
						make_initializer_list(
							{
								packet_after_information_structure.header.information_section_size,
								packet_after_information_structure.header.general_resource_data_section_offset + packet_after_information_structure.header.general_resource_data_section_size,
								packet_after_information_structure.header.texture_resource_data_section_offset + packet_after_information_structure.header.texture_resource_data_section_size,
							}
						)
					);
					packet_after_subgroup_information.general_resource_data_section_offset = packet_after_information_structure.header.general_resource_data_section_offset;
					packet_after_subgroup_information.general_resource_data_section_size = packet_after_information_structure.header.general_resource_data_section_size;
					packet_after_subgroup_information.texture_resource_data_section_offset = packet_after_information_structure.header.texture_resource_data_section_offset;
					packet_after_subgroup_information.texture_resource_data_section_size = packet_after_information_structure.header.texture_resource_data_section_size;
					after.forward(packet_after_ripe.position());
				}
			}
			if (use_raw_packet) {
				OByteStreamView{after.sub_view(cbox<Size>(information_section_after_structure.header.subgroup_information_section_offset), bs_size(information_section_after_structure.subgroup_information))}.write(information_section_after_structure.subgroup_information);
			}
			before.set_position(before_end_position);
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView & before_,
			OByteStreamView & after_,
			IByteStreamView & patch_,
			Boolean const &   use_raw_packet
		) -> Void {
			M_use_zps_of(before);
			M_use_zps_of(after);
			M_use_zps_of(patch);
			return process_whole(before, after, patch, use_raw_packet);
		}

	};

}
