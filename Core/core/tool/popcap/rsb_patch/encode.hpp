#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/rsb_patch/version.hpp"
#include "core/tool/popcap/rsgp/structure.hpp"

namespace TwinStar::Core::Tool::PopCap::RSBPatch {

	template <auto version> requires (check_version(version, {}))
	struct EncodeCommon {

	protected:

		using MagicIdentifier = IntegerU32;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{0x52534250_iu32};

		// ----------------

		using VersionNumber = IntegerU32;

		// ----------------

		inline static constexpr auto package_version = RSB::Version{.number = 4_i, .additional_texture_information_for_pvz_2_chinese_android = 0_i};

		static auto read_package_information_structure (
			IByteStreamView &                              data,
			RSB::Structure::Information<package_version> & information_structure
		) -> Void {
			data.read_constant(RSB::Structure::k_magic_identifier);
			data.read_constant(cbw<RSB::Structure::VersionNumber>(package_version.number));
			data.read(information_structure.header);
			assert_test(cbw<Size>(information_structure.header.group_information_section_block_size) == bs_static_size<RSB::Structure::GroupInformation<package_version>>());
			assert_test(cbw<Size>(information_structure.header.subgroup_information_section_block_size) == bs_static_size<RSB::Structure::SubgroupInformation<package_version>>());
			assert_test(cbw<Size>(information_structure.header.subgroup_pool_information_section_block_size) == bs_static_size<RSB::Structure::SubgroupPoolInformation<package_version>>());
			assert_test(cbw<Size>(information_structure.header.texture_resource_information_section_block_size) == bs_static_size<RSB::Structure::TextureResourceInformation<package_version>>());
			StringMapSection::decode(information_structure.group_id, data.sub_view(cbw<Size>(information_structure.header.group_id_section_offset), cbw<Size>(information_structure.header.group_id_section_size)));
			StringMapSection::decode(information_structure.subgroup_id, data.sub_view(cbw<Size>(information_structure.header.subgroup_id_section_offset), cbw<Size>(information_structure.header.subgroup_id_section_size)));
			StringMapSection::decode(information_structure.resource_path, data.sub_view(cbw<Size>(information_structure.header.resource_path_section_offset), cbw<Size>(information_structure.header.resource_path_section_size)));
			data.set_position(cbw<Size>(information_structure.header.group_information_section_offset));
			data.read(information_structure.group_information, cbw<Size>(information_structure.header.group_information_section_block_count));
			data.set_position(cbw<Size>(information_structure.header.subgroup_information_section_offset));
			data.read(information_structure.subgroup_information, cbw<Size>(information_structure.header.subgroup_information_section_block_count));
			data.set_position(cbw<Size>(information_structure.header.subgroup_pool_information_section_offset));
			data.read(information_structure.subgroup_pool_information, cbw<Size>(information_structure.header.subgroup_pool_information_section_block_count));
			data.set_position(cbw<Size>(information_structure.header.texture_resource_information_section_offset));
			data.read(information_structure.texture_resource_information, cbw<Size>(information_structure.header.texture_resource_information_section_block_count));
			assert_test(information_structure.group_id.size() == cbw<Size>(information_structure.header.group_information_section_block_count));
			assert_test(information_structure.subgroup_id.size() == cbw<Size>(information_structure.header.subgroup_information_section_block_count));
			return;
		}

		// ----------------

		inline static constexpr auto packet_version = RSGP::Version{.number = 4_i};

		static auto compress_packet (
			IByteStreamView & raw,
			OByteStreamView & ripe
		) -> RSGP::Structure::Information<packet_version> {
			raw.read_constant(RSGP::Structure::k_magic_identifier);
			raw.read_constant(cbw<RSGP::Structure::VersionNumber>(packet_version.number));
			auto information_structure = RSGP::Structure::Information<packet_version>{};
			{
				raw.read(information_structure.header);
				StringMapSection::decode(information_structure.resource_information, raw.sub_view(cbw<Size>(information_structure.header.resource_information_section_offset), cbw<Size>(information_structure.header.resource_information_section_size)));
			}
			raw.set_position(
				cbw<Size>(
					max(
						{
							information_structure.header.information_section_size,
							information_structure.header.generic_resource_data_section_offset + information_structure.header.generic_resource_data_section_size_original,
							information_structure.header.texture_resource_data_section_offset + information_structure.header.texture_resource_data_section_size_original,
						}
					)
				)
			);
			ripe.write(raw.sub_view(k_begin_index, cbw<Size>(information_structure.header.information_section_size)));
			ripe.write_space(k_null_byte, compute_padding_size(ripe.position(), k_padding_unit_size));
			auto resource_data_section_store_mode = resource_data_section_store_mode_from_data(information_structure.header.resource_data_section_store_mode);
			{
				information_structure.header.generic_resource_data_section_offset = cbw<IntegerU32>(ripe.position());
				auto resource_data_section_view = raw.sub_view(cbw<Size>(information_structure.header.generic_resource_data_section_offset), cbw<Size>(information_structure.header.generic_resource_data_section_size_original));
				if (!resource_data_section_store_mode.compress_generic) {
					ripe.write(resource_data_section_view);
				} else {
					Data::Compression::Deflate::Compress::do_process_whole(as_lvalue(IByteStreamView{resource_data_section_view}), ripe, 9_sz, 15_sz, 9_sz, Data::Compression::Deflate::Strategy::Constant::default_mode(), Data::Compression::Deflate::Wrapper::Constant::zlib());
					ripe.write_space(k_null_byte, compute_padding_size(ripe.position(), k_padding_unit_size));
				}
				information_structure.header.generic_resource_data_section_size = cbw<IntegerU32>(ripe.position()) - information_structure.header.generic_resource_data_section_offset;
			}
			{
				information_structure.header.texture_resource_data_section_offset = cbw<IntegerU32>(ripe.position());
				auto resource_data_section_view = raw.sub_view(cbw<Size>(information_structure.header.texture_resource_data_section_offset), cbw<Size>(information_structure.header.texture_resource_data_section_size_original));
				if (!resource_data_section_store_mode.compress_texture || cbw<Size>(information_structure.header.texture_resource_data_section_size_original) == k_none_size) {
					ripe.write(resource_data_section_view);
				} else {
					Data::Compression::Deflate::Compress::do_process_whole(as_lvalue(IByteStreamView{resource_data_section_view}), ripe, 9_sz, 15_sz, 9_sz, Data::Compression::Deflate::Strategy::Constant::default_mode(), Data::Compression::Deflate::Wrapper::Constant::zlib());
					ripe.write_space(k_null_byte, compute_padding_size(ripe.position(), k_padding_unit_size));
				}
				information_structure.header.texture_resource_data_section_size = cbw<IntegerU32>(ripe.position()) - information_structure.header.texture_resource_data_section_offset;
			}
			OByteStreamView{ripe.sub_view(bs_static_size<RSGP::Structure::MagicIdentifier>() + bs_static_size<RSGP::Structure::VersionNumber>(), bs_size(information_structure.header))}.write(information_structure.header);
			return information_structure;
		}

		static auto uncompress_packet (
			IByteStreamView & ripe,
			OByteStreamView & raw
		) -> RSGP::Structure::Information<packet_version> {
			ripe.read_constant(RSGP::Structure::k_magic_identifier);
			ripe.read_constant(cbw<RSGP::Structure::VersionNumber>(packet_version.number));
			auto information_structure = RSGP::Structure::Information<packet_version>{};
			{
				ripe.read(information_structure.header);
				StringMapSection::decode(information_structure.resource_information, ripe.sub_view(cbw<Size>(information_structure.header.resource_information_section_offset), cbw<Size>(information_structure.header.resource_information_section_size)));
			}
			ripe.set_position(
				cbw<Size>(
					max(
						{
							information_structure.header.information_section_size,
							information_structure.header.generic_resource_data_section_offset + information_structure.header.generic_resource_data_section_size,
							information_structure.header.texture_resource_data_section_offset + information_structure.header.texture_resource_data_section_size,
						}
					)
				)
			);
			raw.write(ripe.sub_view(k_begin_index, cbw<Size>(information_structure.header.information_section_size)));
			raw.write_space(k_null_byte, compute_padding_size(raw.position(), k_padding_unit_size));
			auto resource_data_section_store_mode = resource_data_section_store_mode_from_data(information_structure.header.resource_data_section_store_mode);
			{
				auto resource_data_section_view = ripe.sub_view(cbw<Size>(information_structure.header.generic_resource_data_section_offset), cbw<Size>(information_structure.header.generic_resource_data_section_size));
				if (!resource_data_section_store_mode.compress_generic) {
					raw.write(resource_data_section_view);
				} else {
					Data::Compression::Deflate::Uncompress::do_process_whole(as_lvalue(IByteStreamView{resource_data_section_view}), raw, 15_sz, Data::Compression::Deflate::Wrapper::Constant::zlib());
				}
				raw.write_space(k_null_byte, compute_padding_size(raw.position(), k_padding_unit_size));
			}
			{
				auto resource_data_section_view = ripe.sub_view(cbw<Size>(information_structure.header.texture_resource_data_section_offset), cbw<Size>(information_structure.header.texture_resource_data_section_size));
				if (!resource_data_section_store_mode.compress_texture || cbw<Size>(information_structure.header.texture_resource_data_section_size_original) == k_none_size) {
					raw.write(resource_data_section_view);
				} else {
					Data::Compression::Deflate::Uncompress::do_process_whole(as_lvalue(IByteStreamView{resource_data_section_view}), raw, 15_sz, Data::Compression::Deflate::Wrapper::Constant::zlib());
				}
				raw.write_space(k_null_byte, compute_padding_size(raw.position(), k_padding_unit_size));
			}
			return information_structure;
		}

		// ----------------

		static auto indexing_subgroup_information_by_id (
			List<RSB::Structure::SubgroupInformation<package_version>> const & list
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

	template <auto version> requires (check_version(version, {}))
	struct Encode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::VersionNumber;

		using Common::package_version;

		using Common::read_package_information_structure;

		using Common::packet_version;

		using Common::compress_packet;

		using Common::uncompress_packet;

		using Common::indexing_subgroup_information_by_id;

		// ----------------

		static auto exchange_constant_32 (
			OByteStreamView &  data,
			IntegerU32 const & value
		) -> Void {
			data.write_constant(value);
			return;
		}

		static auto exchange_boolean_32 (
			OByteStreamView & data,
			Boolean const &   value
		) -> Void {
			auto raw_value = IntegerU32{};
			raw_value = cbw<IntegerU32>(value);
			data.write(raw_value);
			return;
		}

		static auto exchange_size_32 (
			OByteStreamView & data,
			Size const &      value
		) -> Void {
			auto raw_value = IntegerU32{};
			raw_value = cbw<IntegerU32>(value);
			data.write(raw_value);
			return;
		}

		static auto exchange_hash_128 (
			OByteStreamView & data,
			ByteArray const & value
		) -> Void {
			data.write(value);
			return;
		}

		static auto exchange_string_128 (
			OByteStreamView & data,
			String const &    value
		) -> Void {
			auto raw_value = StaticCharacterArray<128_sz>{};
			Range::assign_from(raw_value.view().head(value.size()), value.view());
			data.write(raw_value);
			return;
		}

		// ----------------

		static auto process_hash_validate (
			CByteListView const & data,
			ByteArray &           hash
		) -> Void {
			Data::Hash::MD5::Hash::do_process_whole(data, hash);
			return;
		}

		static auto process_sub_patch (
			CByteListView const & before,
			CByteListView const & after,
			OByteStreamView &     patch,
			Size &                patch_size
		) -> Void {
			auto before_stream = IByteStreamView{before};
			auto after_stream = IByteStreamView{after};
			auto patch_begin_position = patch.position();
			Data::Differentiation::VCDiff::Encode::do_process_whole(before_stream, after_stream, patch, k_true);
			patch_size = patch.position() - patch_begin_position;
			return;
		}

		// ----------------

		static auto process_whole (
			IByteStreamView & before,
			IByteStreamView & after,
			OByteStreamView & patch,
			Boolean const &   use_raw_packet
		) -> Void {
			patch.write_constant(k_magic_identifier);
			patch.write_constant(cbw<VersionNumber>(version.number));
			auto after_size = Size{};
			auto information_section_before_hash = ByteArray{};
			auto information_section_patch_exist = Boolean{};
			auto information_section_patch_size = Size{};
			auto packet_patch_count = Size{};
			auto information_section_before_structure = RSB::Structure::Information<package_version>{};
			auto information_section_after_structure = RSB::Structure::Information<package_version>{};
			after_size = after.reserve();
			exchange_constant_32(patch, 0x00000002_iu32);
			exchange_size_32(patch, after_size);
			exchange_constant_32(patch, 0x00000000_iu32);
			read_package_information_structure(as_lvalue(IByteStreamView{before.view()}), information_section_before_structure);
			read_package_information_structure(as_lvalue(IByteStreamView{after.view()}), information_section_after_structure);
			auto information_section_patch_header = OByteStreamView{patch.forward_view(4_sz + 4_sz + 4_sz + 16_sz)};
			packet_patch_count = cbw<Size>(information_section_after_structure.header.subgroup_information_section_block_count);
			auto information_section_before = before.sub_view(k_begin_index, cbw<Size>(information_section_before_structure.header.information_section_size));
			process_hash_validate(information_section_before, information_section_before_hash);
			auto information_section_after = after.sub_view(k_begin_index, cbw<Size>(information_section_after_structure.header.information_section_size));
			information_section_patch_exist = information_section_after != information_section_before;
			if (!information_section_patch_exist) {
				information_section_patch_size = k_none_size;
			} else {
				process_sub_patch(information_section_before, information_section_after, patch, information_section_patch_size);
			}
			exchange_size_32(information_section_patch_header, information_section_patch_size);
			exchange_hash_128(information_section_patch_header, information_section_before_hash);
			exchange_size_32(information_section_patch_header, packet_patch_count);
			exchange_boolean_32(information_section_patch_header, information_section_patch_exist);
			auto packet_before_subgroup_information_index_map = indexing_subgroup_information_by_id(information_section_before_structure.subgroup_information);
			auto packet_before_raw_container = ByteArray{};
			auto packet_after_raw_container = ByteArray{};
			auto before_end_position = cbw<Size>(information_section_before_structure.header.information_section_size);
			auto after_end_position = cbw<Size>(information_section_after_structure.header.information_section_size);
			for (auto & packet_index : SizeRange{packet_patch_count}) {
				auto & packet_after_subgroup_information = information_section_after_structure.subgroup_information[packet_index];
				auto   packet_name = String{};
				auto   packet_before_hash = ByteArray{};
				auto   packet_patch_exist = Boolean{};
				auto   packet_patch_size = Size{};
				packet_name = String{packet_after_subgroup_information.id.begin(), null_terminated_string_size_of(packet_after_subgroup_information.id.begin())};
				auto packet_before = CByteListView{};
				if (auto packet_before_subgroup_information_index = packet_before_subgroup_information_index_map.query_if(packet_name)) {
					auto & packet_before_subgroup_information = information_section_before_structure.subgroup_information[cbw<Size>(packet_before_subgroup_information_index.get().value)];
					auto   packet_before_ripe = IByteStreamView{before.sub_view(cbw<Size>(packet_before_subgroup_information.offset), cbw<Size>(packet_before_subgroup_information.size))};
					if (!use_raw_packet) {
						packet_before = packet_before_ripe.view();
					} else {
						auto packet_before_raw_size = cbw<Size>(packet_before_subgroup_information.information_section_size + packet_before_subgroup_information.generic_resource_data_section_size_original + packet_before_subgroup_information.texture_resource_data_section_size_original);
						if (packet_before_raw_size > packet_before_raw_container.size()) {
							packet_before_raw_container.allocate(packet_before_raw_size);
						}
						auto packet_before_raw = OByteStreamView{packet_before_raw_container};
						uncompress_packet(packet_before_ripe, packet_before_raw);
						assert_test(packet_before_ripe.full());
						packet_before = packet_before_raw.stream_view();
					}
					before_end_position = max(before_end_position, cbw<Size>(packet_before_subgroup_information.offset + packet_before_subgroup_information.size));
				}
				process_hash_validate(packet_before, packet_before_hash);
				auto packet_after = CByteListView{};
				{
					auto packet_after_ripe = IByteStreamView{after.sub_view(cbw<Size>(packet_after_subgroup_information.offset), cbw<Size>(packet_after_subgroup_information.size))};
					if (!use_raw_packet) {
						packet_after = packet_after_ripe.view();
					} else {
						auto packet_after_raw_size = cbw<Size>(packet_after_subgroup_information.information_section_size + packet_after_subgroup_information.generic_resource_data_section_size_original + packet_after_subgroup_information.texture_resource_data_section_size_original);
						if (packet_after_raw_size > packet_after_raw_container.size()) {
							packet_after_raw_container.allocate(packet_after_raw_size);
						}
						auto packet_after_raw = OByteStreamView{packet_after_raw_container};
						uncompress_packet(packet_after_ripe, packet_after_raw);
						assert_test(packet_after_ripe.full());
						packet_after = packet_after_raw.stream_view();
					}
					after_end_position = max(after_end_position, cbw<Size>(packet_after_subgroup_information.offset + packet_after_subgroup_information.size));
				}
				auto packet_patch_header = OByteStreamView{patch.forward_view(4_sz + 4_sz + 128_sz + 16_sz)};
				packet_patch_exist = packet_after != packet_before;
				if (!packet_patch_exist) {
					packet_patch_size = k_none_size;
				} else {
					process_sub_patch(packet_before, packet_after, patch, packet_patch_size);
				}
				exchange_boolean_32(packet_patch_header, packet_patch_exist);
				exchange_size_32(packet_patch_header, packet_patch_size);
				exchange_string_128(packet_patch_header, packet_name);
				exchange_hash_128(packet_patch_header, packet_before_hash);
			}
			before.set_position(before_end_position);
			after.set_position(after_end_position);
			return;
		}

	public:

		static auto do_process_whole (
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

	template <auto version> requires (check_version(version, {}))
	struct Decode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::VersionNumber;

		using Common::package_version;

		using Common::read_package_information_structure;

		using Common::packet_version;

		using Common::compress_packet;

		using Common::uncompress_packet;

		using Common::indexing_subgroup_information_by_id;

		// ----------------

		static auto exchange_constant_32 (
			IByteStreamView &  data,
			IntegerU32 const & value
		) -> Void {
			data.read_constant(value);
			return;
		}

		static auto exchange_boolean_32 (
			IByteStreamView & data,
			Boolean &         value
		) -> Void {
			auto raw_value = IntegerU32{};
			data.read(raw_value);
			value = cbw<Boolean>(raw_value);
			return;
		}

		static auto exchange_size_32 (
			IByteStreamView & data,
			Size &            value
		) -> Void {
			auto raw_value = IntegerU32{};
			data.read(raw_value);
			value = cbw<Size>(raw_value);
			return;
		}

		static auto exchange_hash_128 (
			IByteStreamView & data,
			ByteArray &       value
		) -> Void {
			data.read(value, 16_sz);
			return;
		}

		static auto exchange_string_128 (
			IByteStreamView & data,
			String &          value
		) -> Void {
			auto raw_value = StaticCharacterArray<128_sz>{};
			data.read(raw_value);
			value = String{raw_value.begin(), null_terminated_string_size_of(raw_value.begin())};
			return;
		}

		// ----------------

		static auto process_hash_validate (
			CByteListView const & data,
			ByteArray const &     hash
		) -> Void {
			auto hash_test = ByteArray{};
			Data::Hash::MD5::Hash::do_process_whole(data, hash_test);
			assert_test(hash_test == hash);
			return;
		}

		static auto process_sub_patch (
			CByteListView const & before,
			OByteStreamView &     after,
			IByteStreamView &     patch,
			Size const &          patch_size
		) -> Void {
			auto before_stream = IByteStreamView{before};
			auto patch_stream = IByteStreamView{patch.forward_view(patch_size)};
			Data::Differentiation::VCDiff::Decode::do_process_whole(before_stream, after, patch_stream, 0x7FFFFFFF_sz);
			return;
		}

		// ----------------

		static auto process_whole (
			IByteStreamView & before,
			OByteStreamView & after,
			IByteStreamView & patch,
			Boolean const &   use_raw_packet
		) -> Void {
			patch.read_constant(k_magic_identifier);
			patch.read_constant(cbw<VersionNumber>(version.number));
			auto after_size = Size{};
			auto information_section_before_hash = ByteArray{};
			auto information_section_patch_exist = Boolean{};
			auto information_section_patch_size = Size{};
			auto packet_patch_count = Size{};
			exchange_constant_32(patch, 0x00000002_iu32);
			exchange_size_32(patch, after_size);
			exchange_constant_32(patch, 0x00000000_iu32);
			exchange_size_32(patch, information_section_patch_size);
			exchange_hash_128(patch, information_section_before_hash);
			exchange_size_32(patch, packet_patch_count);
			exchange_boolean_32(patch, information_section_patch_exist);
			auto information_section_before_structure = RSB::Structure::Information<package_version>{};
			auto information_section_after_structure = RSB::Structure::Information<package_version>{};
			read_package_information_structure(as_lvalue(IByteStreamView{before.view()}), information_section_before_structure);
			auto information_section_before = before.sub_view(k_begin_index, cbw<Size>(information_section_before_structure.header.information_section_size));
			process_hash_validate(information_section_before, information_section_before_hash);
			if (!information_section_patch_exist) {
				assert_test(information_section_patch_size == k_none_size);
				after.write(information_section_before);
			} else {
				process_sub_patch(information_section_before, after, patch, information_section_patch_size);
			}
			read_package_information_structure(as_lvalue(IByteStreamView{after.view()}), information_section_after_structure);
			assert_test(packet_patch_count == information_section_after_structure.subgroup_information.size());
			auto packet_before_subgroup_information_index_map = indexing_subgroup_information_by_id(information_section_before_structure.subgroup_information);
			auto packet_before_raw_container = ByteArray{};
			auto packet_after_raw_container = ByteArray{};
			auto before_end_position = cbw<Size>(information_section_before_structure.header.information_section_size);
			for (auto & packet_index : SizeRange{packet_patch_count}) {
				auto & packet_after_subgroup_information = information_section_after_structure.subgroup_information[packet_index];
				auto   packet_name = String{};
				auto   packet_before_hash = ByteArray{};
				auto   packet_patch_exist = Boolean{};
				auto   packet_patch_size = Size{};
				exchange_boolean_32(patch, packet_patch_exist);
				exchange_size_32(patch, packet_patch_size);
				exchange_string_128(patch, packet_name);
				exchange_hash_128(patch, packet_before_hash);
				{
					auto packet_name_upper = packet_name;
					packet_name_upper.as_upper_case();
					auto packet_after_subgroup_index = cbw<Size>(information_section_after_structure.subgroup_id[packet_name_upper]);
					assert_test(packet_after_subgroup_index == packet_index);
				}
				auto packet_before = CByteListView{};
				if (auto packet_before_subgroup_information_index = packet_before_subgroup_information_index_map.query_if(packet_name)) {
					auto & packet_before_subgroup_information = information_section_before_structure.subgroup_information[packet_before_subgroup_information_index.get().value];
					auto   packet_before_ripe = IByteStreamView{before.sub_view(cbw<Size>(packet_before_subgroup_information.offset), cbw<Size>(packet_before_subgroup_information.size))};
					if (!use_raw_packet) {
						packet_before = packet_before_ripe.view();
					} else {
						auto packet_before_raw_size = cbw<Size>(packet_before_subgroup_information.information_section_size + packet_before_subgroup_information.generic_resource_data_section_size_original + packet_before_subgroup_information.texture_resource_data_section_size_original);
						if (packet_before_raw_size > packet_before_raw_container.size()) {
							packet_before_raw_container.allocate(packet_before_raw_size);
						}
						auto packet_before_raw = OByteStreamView{packet_before_raw_container};
						uncompress_packet(packet_before_ripe, packet_before_raw);
						assert_test(packet_before_ripe.full());
						packet_before = packet_before_raw.stream_view();
					}
					before_end_position = max(before_end_position, cbw<Size>(packet_before_subgroup_information.offset + packet_before_subgroup_information.size));
				}
				process_hash_validate(packet_before, packet_before_hash);
				auto packet_after = CByteListView{};
				if (!packet_patch_exist) {
					assert_test(packet_patch_size == k_none_size);
					packet_after = packet_before;
				} else {
					auto packet_after_raw_size = cbw<Size>(packet_after_subgroup_information.information_section_size + packet_after_subgroup_information.generic_resource_data_section_size_original + packet_after_subgroup_information.texture_resource_data_section_size_original);
					if (packet_after_raw_size > packet_after_raw_container.size()) {
						packet_after_raw_container.allocate(packet_after_raw_size);
					}
					auto packet_after_raw = OByteStreamView{packet_after_raw_container};
					process_sub_patch(packet_before, packet_after_raw, patch, packet_patch_size);
					packet_after = packet_after_raw.stream_view();
				}
				if (!use_raw_packet) {
					after.write(packet_after);
				} else {
					auto packet_after_raw = IByteStreamView{packet_after};
					auto packet_after_ripe = OByteStreamView{after.reserve_view()};
					auto packet_after_information_structure = compress_packet(packet_after_raw, packet_after_ripe);
					assert_test(packet_after_raw.full());
					packet_after_subgroup_information.offset = cbw<IntegerU32>(after.position());
					packet_after_subgroup_information.size = max(
						{
							packet_after_information_structure.header.information_section_size,
							packet_after_information_structure.header.generic_resource_data_section_offset + packet_after_information_structure.header.generic_resource_data_section_size,
							packet_after_information_structure.header.texture_resource_data_section_offset + packet_after_information_structure.header.texture_resource_data_section_size,
						}
					);
					packet_after_subgroup_information.generic_resource_data_section_offset = packet_after_information_structure.header.generic_resource_data_section_offset;
					packet_after_subgroup_information.generic_resource_data_section_size = packet_after_information_structure.header.generic_resource_data_section_size;
					packet_after_subgroup_information.texture_resource_data_section_offset = packet_after_information_structure.header.texture_resource_data_section_offset;
					packet_after_subgroup_information.texture_resource_data_section_size = packet_after_information_structure.header.texture_resource_data_section_size;
					after.forward(packet_after_ripe.position());
				}
			}
			if (use_raw_packet) {
				OByteStreamView{after.sub_view(cbw<Size>(information_section_after_structure.header.subgroup_information_section_offset), bs_size(information_section_after_structure.subgroup_information))}.write(information_section_after_structure.subgroup_information);
			}
			before.set_position(before_end_position);
			return;
		}

	public:

		static auto do_process_whole (
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
