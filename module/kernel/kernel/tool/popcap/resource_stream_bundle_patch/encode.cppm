module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_bundle_patch.encode;
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
	struct Encode :
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
			ConstantBinaryView const & data,
			StaticArray<Byte, 16_sz> & hash
		) -> Void {
			auto hash_test = Binary{};
			Data::Hashing::Md::Hash::process(data, hash_test, Data::Hashing::Md::Mode::Constant::v5(), 128_sz);
			Range::assign_from(hash.view(), hash_test);
			return;
		}

		inline static auto process_sub(
			InputBinaryStreamView &  before,
			InputBinaryStreamView &  after,
			OutputBinaryStreamView & patch,
			Size &                   patch_size
		) -> Void {
			auto patch_begin_position = patch.position();
			Data::Differentiation::Vcdiff::Encode::process(before, after, patch, k_true);
			patch_size = patch.position() - patch_begin_position;
			return;
		}

		inline static auto process_whole(
			InputBinaryStreamView &  before,
			InputBinaryStreamView &  after,
			OutputBinaryStreamView & patch,
			Boolean const &          use_raw_packet
		) -> Void {
			patch.write_constant(k_magic_marker);
			patch.write_constant(cast_box<VersionNumber>(t_version.number));
			auto package_information_stream = patch.split_forward(binary_static_size_of<PackageInformation>());
			auto package_information = PackageInformation{};
			auto information_section_before_structure = ResourceStreamBundle::Structure::Information<package_version>{};
			auto information_section_after_structure = ResourceStreamBundle::Structure::Information<package_version>{};
			read_package_information_structure(as_left(before.split_copy()), information_section_before_structure);
			read_package_information_structure(as_left(after.split_copy()), information_section_after_structure);
			auto information_section_before = before.split_sub(0_sz, cast_box<Size>(information_section_before_structure.header.information_section_size));
			auto information_section_after = after.split_sub(0_sz, cast_box<Size>(information_section_after_structure.header.information_section_size));
			test_hash(information_section_before.view(), package_information.before_hash);
			auto information_section_patch_exist = Boolean{};
			auto information_section_patch_size = Size{};
			information_section_patch_exist = information_section_after.view() != information_section_before.view();
			if (!information_section_patch_exist) {
				information_section_patch_size = 0_sz;
			}
			else {
				process_sub(information_section_before, information_section_after, patch, information_section_patch_size);
			}
			package_information.patch_exist = cast_box<IntegerU32>(information_section_patch_exist);
			package_information.patch_size = cast_box<IntegerU32>(information_section_patch_size);
			auto packet_before_subgroup_information_index_if_map = indexing_subgroup_information_by_identifier(information_section_before_structure.subgroup_information);
			auto packet_before_raw_container = Binary{};
			auto packet_after_raw_container = Binary{};
			auto before_end_position = cast_box<Size>(information_section_before_structure.header.information_section_size);
			auto after_end_position = cast_box<Size>(information_section_after_structure.header.information_section_size);
			auto packet_count = cast_box<Size>(information_section_after_structure.header.subgroup_information_section_block_count);
			package_information.packet_count = cast_box<IntegerU32>(packet_count);
			for (auto & packet_index : SizeRange{packet_count}) {
				auto & packet_after_subgroup_information = information_section_after_structure.subgroup_information[packet_index];
				auto   packet_information_stream = patch.split_forward(binary_static_size_of<PacketInformation>());
				auto   packet_information = PacketInformation{};
				auto   packet_name = String{};
				auto   packet_patch_exist = Boolean{};
				auto   packet_patch_size = Size{};
				packet_name = make_string_view_from_indeterminate(packet_after_subgroup_information.identifier.data(), packet_after_subgroup_information.identifier.size());
				auto packet_before = InputBinaryStreamView{};
				auto packet_before_subgroup_information_index_if = packet_before_subgroup_information_index_if_map.query_if(packet_name);
				if (packet_before_subgroup_information_index_if.has()) {
					auto & packet_before_subgroup_information = information_section_before_structure.subgroup_information[cast_box<Size>(packet_before_subgroup_information_index_if.get().dereference().value)];
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
				{
					auto packet_after_ripe = after.split_sub(cast_box<Size>(packet_after_subgroup_information.offset), cast_box<Size>(packet_after_subgroup_information.size));
					if (!use_raw_packet) {
						packet_after = packet_after_ripe;
					}
					else {
						auto packet_after_raw_size = cast_box<Size>(packet_after_subgroup_information.information_section_size + packet_after_subgroup_information.general_resource_data_section_size_original + packet_after_subgroup_information.texture_resource_data_section_size_original);
						auto packet_after_raw = like_binary_stream<StreamMode::Constant::output()>(after, use_binary_buffer(packet_after_raw_container, packet_after_raw_size));
						uncompress_packet(packet_after_ripe, packet_after_raw);
						assert_test(packet_after_ripe.full());
						packet_after = unsafe_cast<InputBinaryStreamView>(packet_after_raw.split_stream());
					}
					after_end_position = Math::maximum(after_end_position, cast_box<Size>(packet_after_subgroup_information.offset + packet_after_subgroup_information.size));
				}
				packet_patch_exist = packet_after.view() != packet_before.view();
				if (!packet_patch_exist) {
					packet_patch_size = 0_sz;
				}
				else {
					process_sub(packet_before, packet_after, patch, packet_patch_size);
				}
				Range::assign_from(packet_information.name.view().head(packet_name.size()), packet_name);
				packet_information.patch_exist = cast_box<IntegerU32>(packet_patch_exist);
				packet_information.patch_size = cast_box<IntegerU32>(packet_patch_size);
				packet_information_stream.write(packet_information);
			}
			package_information.all_after_size = cast_box<IntegerU32>(after_end_position);
			package_information.packet_count = cast_box<IntegerU32>(packet_count);
			package_information.patch_exist = cast_box<IntegerU32>(information_section_patch_exist);
			package_information.patch_size = cast_box<IntegerU32>(information_section_patch_size);
			package_information_stream.write(package_information);
			before.set_position(before_end_position);
			after.set_position(after_end_position);
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &  before_,
			InputBinaryStreamView &  after_,
			OutputBinaryStreamView & patch_,
			Boolean const &          use_raw_packet
		) -> Void {
			M_use_split_stream_of(before);
			M_use_split_stream_of(after);
			M_use_split_stream_of(patch);
			return process_whole(before, after, patch, use_raw_packet);
		}

	};

}
