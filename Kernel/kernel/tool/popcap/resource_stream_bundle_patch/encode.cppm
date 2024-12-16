module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_bundle_patch.encode;
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
	struct Encode :
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
			CByteListView const &    data,
			StaticByteArray<16_sz> & hash
		) -> Void {
			auto hash_test = ByteArray{};
			Data::Hash::MD5::Hash::process(data, hash_test);
			Range::assign_from(hash.view(), hash_test.as_view());
			return;
		}

		inline static auto process_sub (
			CByteListView const & before,
			CByteListView const & after,
			OByteStreamView &     patch,
			Size &                patch_size
		) -> Void {
			auto before_stream = IByteStreamView{before};
			auto after_stream = IByteStreamView{after};
			auto patch_begin_position = patch.position();
			Data::Differentiation::VCDiff::Encode::process(before_stream, after_stream, patch, k_true);
			patch_size = patch.position() - patch_begin_position;
			return;
		}

		inline static auto process_whole (
			IByteStreamView & before,
			IByteStreamView & after,
			OByteStreamView & patch,
			Boolean const &   use_raw_packet
		) -> Void {
			patch.write_constant(k_magic_identifier);
			patch.write_constant(cbox<VersionNumber>(version.number));
			auto package_information_stream = OByteStreamView{patch.forward_view(bs_static_size<PackageInformation>())};
			auto package_information = PackageInformation{};
			auto information_section_before_structure = ResourceStreamBundle::Structure::Information<package_version>{};
			auto information_section_after_structure = ResourceStreamBundle::Structure::Information<package_version>{};
			read_package_information_structure(as_lvalue(IByteStreamView{before.view()}), information_section_before_structure);
			read_package_information_structure(as_lvalue(IByteStreamView{after.view()}), information_section_after_structure);
			auto information_section_before = before.sub_view(k_begin_index, cbox<Size>(information_section_before_structure.header.information_section_size));
			auto information_section_after = after.sub_view(k_begin_index, cbox<Size>(information_section_after_structure.header.information_section_size));
			test_hash(information_section_before, package_information.before_hash);
			auto information_section_patch_exist = Boolean{};
			auto information_section_patch_size = Size{};
			information_section_patch_exist = information_section_after != information_section_before;
			if (!information_section_patch_exist) {
				information_section_patch_size = k_none_size;
			}
			else {
				process_sub(information_section_before, information_section_after, patch, information_section_patch_size);
			}
			package_information.patch_exist = cbox<IntegerU32>(information_section_patch_exist);
			package_information.patch_size = cbox<IntegerU32>(information_section_patch_size);
			auto packet_before_subgroup_information_index_map = indexing_subgroup_information_by_id(information_section_before_structure.subgroup_information);
			auto packet_before_raw_container = ByteArray{};
			auto packet_after_raw_container = ByteArray{};
			auto before_end_position = cbox<Size>(information_section_before_structure.header.information_section_size);
			auto after_end_position = cbox<Size>(information_section_after_structure.header.information_section_size);
			auto packet_count = cbox<Size>(information_section_after_structure.header.subgroup_information_section_block_count);
			package_information.packet_count = cbox<IntegerU32>(packet_count);
			for (auto & packet_index : SizeRange{packet_count}) {
				auto & packet_after_subgroup_information = information_section_after_structure.subgroup_information[packet_index];
				auto   packet_information_stream = OByteStreamView{patch.forward_view(bs_static_size<PacketInformation>())};
				auto   packet_information = PacketInformation{};
				auto   packet_name = String{};
				auto   packet_patch_exist = Boolean{};
				auto   packet_patch_size = Size{};
				packet_name = String{packet_after_subgroup_information.id.begin(), null_terminated_string_size_of(packet_after_subgroup_information.id.begin())};
				auto packet_before = CByteListView{};
				if (auto packet_before_subgroup_information_index = packet_before_subgroup_information_index_map.query_if(packet_name)) {
					auto & packet_before_subgroup_information = information_section_before_structure.subgroup_information[cbox<Size>(packet_before_subgroup_information_index.get().value)];
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
				{
					auto packet_after_ripe = IByteStreamView{after.sub_view(cbox<Size>(packet_after_subgroup_information.offset), cbox<Size>(packet_after_subgroup_information.size))};
					if (!use_raw_packet) {
						packet_after = packet_after_ripe.view();
					}
					else {
						auto packet_after_raw_size = cbox<Size>(packet_after_subgroup_information.information_section_size + packet_after_subgroup_information.general_resource_data_section_size_original + packet_after_subgroup_information.texture_resource_data_section_size_original);
						if (packet_after_raw_size > packet_after_raw_container.size()) {
							packet_after_raw_container.allocate(packet_after_raw_size);
						}
						auto packet_after_raw = OByteStreamView{packet_after_raw_container};
						uncompress_packet(packet_after_ripe, packet_after_raw);
						assert_test(packet_after_ripe.full());
						packet_after = packet_after_raw.stream_view();
					}
					after_end_position = maximum(after_end_position, cbox<Size>(packet_after_subgroup_information.offset + packet_after_subgroup_information.size));
				}
				packet_patch_exist = packet_after != packet_before;
				if (!packet_patch_exist) {
					packet_patch_size = k_none_size;
				}
				else {
					process_sub(packet_before, packet_after, patch, packet_patch_size);
				}
				Range::assign_from(packet_information.name.view().head(packet_name.size()), packet_name.view());
				packet_information.patch_exist = cbox<IntegerU32>(packet_patch_exist);
				packet_information.patch_size = cbox<IntegerU32>(packet_patch_size);
				packet_information_stream.write(packet_information);
			}
			package_information.all_after_size = cbox<IntegerU32>(after_end_position);
			package_information.packet_count = cbox<IntegerU32>(packet_count);
			package_information.patch_exist = cbox<IntegerU32>(information_section_patch_exist);
			package_information.patch_size = cbox<IntegerU32>(information_section_patch_size);
			package_information_stream.write(package_information);
			before.set_position(before_end_position);
			after.set_position(after_end_position);
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView & before_,
			IByteStreamView & after_,
			OByteStreamView & patch_,
			Boolean const &   use_raw_packet
		) -> Void {
			M_use_zps_of(before);
			M_use_zps_of(after);
			M_use_zps_of(patch);
			return process_whole(before, after, patch, use_raw_packet);
		}

	};

}
