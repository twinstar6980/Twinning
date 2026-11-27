module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_bundle_patch.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_bundle_patch.version;
import twinning.kernel.tool.popcap.resource_stream_bundle_patch.common;
import twinning.kernel.tool.popcap.resource_stream_bundle.structure;
import twinning.kernel.tool.data.hashing.md.common;
import twinning.kernel.tool.data.hashing.md.hash;
import twinning.kernel.tool.data.differentiation.vcdiff.common;
import twinning.kernel.tool.data.differentiation.vcdiff.encode;
import twinning.kernel.tool.data.differentiation.vcdiff.decode;

export namespace Twinning::Kernel::Tool::Popcap::ResourceStreamBundlePatch {

	template <auto t_version> requires (check_version(t_version, {}))
	struct Decode :
		Common<t_version> {

		using Common = Common<t_version>;

		using typename Common::MagicMarker;

		using Common::k_magic_marker;

		using typename Common::VersionNumber;

		using typename Common::PackageInformation;

		using typename Common::PacketInformation;

		using Common::package_version;

		using Common::read_package_information_structure;

		using Common::packet_version;

		using Common::compress_packet;

		using Common::uncompress_packet;

		using Common::indexing_subgroup_information_by_identifier;

		// ----------------

		inline static auto test_hash(
			ConstantBinaryView const &       data,
			StaticArray<Byte, 16_sz> const & hash
		) -> Void {
			auto hash_test = Binary{};
			Data::Hashing::Md::Hash::process(data, hash_test, Data::Hashing::Md::Mode::Constant::v5(), 128_sz);
			assert_test(hash_test.as_view() == hash.view());
			return;
		}

		inline static auto process_sub(
			InputBinaryStreamView &  before,
			OutputBinaryStreamView & after,
			InputBinaryStreamView &  patch,
			Size const &             patch_size
		) -> Void {
			auto patch_stream = patch.split_forward(patch_size);
			Data::Differentiation::Vcdiff::Decode::process(before, after, patch_stream, 0x7FFFFFFF_i);
			return;
		}

		inline static auto process_whole(
			InputBinaryStreamView &  before,
			OutputBinaryStreamView & after,
			InputBinaryStreamView &  patch,
			Boolean const &          use_raw_packet
		) -> Void {
			patch.read_constant(k_magic_marker);
			patch.read_constant(cast_box<VersionNumber>(t_version.number));
			auto package_information = PackageInformation{};
			patch.read(package_information);
			auto packet_count = cast_box<Size>(package_information.packet_count);
			auto information_section_patch_exist = cast_box<Boolean>(package_information.patch_exist);
			auto information_section_patch_size = cast_box<Size>(package_information.patch_size);
			auto information_section_before_structure = ResourceStreamBundle::Structure::Information<package_version>{};
			auto information_section_after_structure = ResourceStreamBundle::Structure::Information<package_version>{};
			read_package_information_structure(as_left(before.split_copy()), information_section_before_structure);
			auto information_section_before = before.split_sub(0_sz, cast_box<Size>(information_section_before_structure.header.information_section_size));
			test_hash(information_section_before.view(), package_information.before_hash);
			if (!information_section_patch_exist) {
				assert_test(information_section_patch_size == 0_sz);
				after.write(information_section_before.view());
			}
			else {
				process_sub(information_section_before, after, patch, information_section_patch_size);
			}
			read_package_information_structure(as_left(unsafe_cast<InputBinaryStreamView>(after.split_copy())), information_section_after_structure);
			assert_test(packet_count == information_section_after_structure.subgroup_information.size());
			auto packet_before_subgroup_information_index_if_map = indexing_subgroup_information_by_identifier(information_section_before_structure.subgroup_information);
			auto packet_before_raw_container = Binary{};
			auto packet_after_raw_container = Binary{};
			auto before_end_position = cast_box<Size>(information_section_before_structure.header.information_section_size);
			for (auto & packet_index : SizeRange{packet_count}) {
				auto & packet_after_subgroup_information = information_section_after_structure.subgroup_information[packet_index];
				auto   packet_information = PacketInformation{};
				patch.read(packet_information);
				auto packet_name = make_string_view_from_indeterminate(packet_information.name.data(), packet_information.name.size());
				auto packet_patch_exist = cast_box<Boolean>(packet_information.patch_exist);
				auto packet_patch_size = cast_box<Size>(packet_information.patch_size);
				{
					auto packet_name_upper = String{packet_name};
					StringUtility::as_upper_case(packet_name_upper);
					auto packet_after_subgroup_index = cast_box<Size>(information_section_after_structure.subgroup_identifier[packet_name_upper]);
					assert_test(packet_after_subgroup_index == packet_index);
				}
				auto packet_before = InputBinaryStreamView{};
				auto packet_before_subgroup_information_index_if = packet_before_subgroup_information_index_if_map.query_if(packet_name);
				if (packet_before_subgroup_information_index_if.has()) {
					auto & packet_before_subgroup_information = information_section_before_structure.subgroup_information[packet_before_subgroup_information_index_if.get().dereference().value];
					auto   packet_before_ripe = before.split_sub(cast_box<Size>(packet_before_subgroup_information.offset), cast_box<Size>(packet_before_subgroup_information.size));
					if (!use_raw_packet) {
						packet_before = packet_before_ripe;
					}
					else {
						auto packet_before_raw_size = cast_box<Size>(packet_before_subgroup_information.information_section_size + packet_before_subgroup_information.general_resource_data_section_size_original + packet_before_subgroup_information.texture_resource_data_section_size_original);
						auto packet_before_raw = like_binary_stream<StreamMode::Constant::output()>(before, use_binary_buffer(packet_before_raw_container, packet_before_raw_size));
						uncompress_packet(packet_before_ripe, packet_before_raw);
						assert_test(packet_before_ripe.full());
						packet_before = unsafe_cast<InputBinaryStreamView>(packet_before_raw.split_stream());
					}
					before_end_position = Math::maximum(before_end_position, cast_box<Size>(packet_before_subgroup_information.offset + packet_before_subgroup_information.size));
				}
				test_hash(packet_before.view(), packet_information.before_hash);
				auto packet_after = InputBinaryStreamView{};
				if (!packet_patch_exist) {
					assert_test(packet_patch_size == 0_sz);
					packet_after = packet_before;
				}
				else {
					auto packet_after_raw_size = cast_box<Size>(packet_after_subgroup_information.information_section_size + packet_after_subgroup_information.general_resource_data_section_size_original + packet_after_subgroup_information.texture_resource_data_section_size_original);
					auto packet_after_raw = like_binary_stream<StreamMode::Constant::output()>(after, use_binary_buffer(packet_after_raw_container, packet_after_raw_size));
					process_sub(packet_before, packet_after_raw, patch, packet_patch_size);
					packet_after = unsafe_cast<InputBinaryStreamView>(packet_after_raw.split_stream());
				}
				if (!use_raw_packet) {
					after.write(packet_after.view());
				}
				else {
					auto packet_after_ripe = after.split_reserve();
					auto packet_after_information_structure = compress_packet(packet_after, packet_after_ripe);
					assert_test(packet_after.full());
					packet_after_subgroup_information.offset = cast_box<IntegerU32>(after.position());
					packet_after_subgroup_information.size = Range::maximum(
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
				after.split_sub(cast_box<Size>(information_section_after_structure.header.subgroup_information_section_offset), binary_size_of(information_section_after_structure.subgroup_information)).write(information_section_after_structure.subgroup_information);
			}
			before.set_position(before_end_position);
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &  before_,
			OutputBinaryStreamView & after_,
			InputBinaryStreamView &  patch_,
			Boolean const &          use_raw_packet
		) -> Void {
			M_use_split_stream_of(before);
			M_use_split_stream_of(after);
			M_use_split_stream_of(patch);
			return process_whole(before, after, patch, use_raw_packet);
		}

	};

}
