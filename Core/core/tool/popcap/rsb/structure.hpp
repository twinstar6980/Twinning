#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/rsb/version.hpp"
#include "core/tool/popcap/rsb/common.hpp"

namespace TwinKleS::Core::Tool::PopCap::RSB::Structure {

	#pragma region magic identifier

	using MagicIdentifier = IntegerU32;

	inline constexpr auto k_magic_identifier = MagicIdentifier{0x72736231_iu32};

	#pragma endregion

	#pragma region version number

	using VersionNumber = IntegerU32;

	#pragma endregion

	template <auto t_version>
	struct Header;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	struct Header<t_version> {
		PaddingBlock<4_szz> unused_1;
		IntegerU32          information_section_size;
		IntegerU32          resource_path_section_size;
		IntegerU32          resource_path_section_offset;
		PaddingBlock<8_szz> unused_2;
		IntegerU32          subgroup_id_section_size;
		IntegerU32          subgroup_id_section_offset;
		IntegerU32          subgroup_information_section_block_count;
		IntegerU32          subgroup_information_section_offset;
		IntegerU32          subgroup_information_section_block_size;
		IntegerU32          group_information_section_block_count;
		IntegerU32          group_information_section_offset;
		IntegerU32          group_information_section_block_size;
		IntegerU32          group_id_section_size;
		IntegerU32          group_id_section_offset;
		IntegerU32          subgroup_pool_information_section_block_count;
		IntegerU32          subgroup_pool_information_section_offset;
		IntegerU32          subgroup_pool_information_section_block_size;
		IntegerU32          texture_resource_information_section_block_count;
		IntegerU32          texture_resource_information_section_offset;
		IntegerU32          texture_resource_information_section_block_size;
		IntegerU32          description_group_section_offset;
		IntegerU32          description_resource_section_offset;
		IntegerU32          description_string_section_offset;
	};

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {4}, {}))
	struct Header<t_version> {
		PaddingBlock<4_szz> unused_1;
		IntegerU32          information_section_size;
		IntegerU32          resource_path_section_size;
		IntegerU32          resource_path_section_offset;
		PaddingBlock<8_szz> unused_2;
		IntegerU32          subgroup_id_section_size;
		IntegerU32          subgroup_id_section_offset;
		IntegerU32          subgroup_information_section_block_count;
		IntegerU32          subgroup_information_section_offset;
		IntegerU32          subgroup_information_section_block_size;
		IntegerU32          group_information_section_block_count;
		IntegerU32          group_information_section_offset;
		IntegerU32          group_information_section_block_size;
		IntegerU32          group_id_section_size;
		IntegerU32          group_id_section_offset;
		IntegerU32          subgroup_pool_information_section_block_count;
		IntegerU32          subgroup_pool_information_section_offset;
		IntegerU32          subgroup_pool_information_section_block_size;
		IntegerU32          texture_resource_information_section_block_count;
		IntegerU32          texture_resource_information_section_offset;
		IntegerU32          texture_resource_information_section_block_size;
		IntegerU32          description_group_section_offset;
		IntegerU32          description_resource_section_offset;
		IntegerU32          description_string_section_offset;
		IntegerU32          information_without_description_section_size;
	};

	// ----------------

	template <auto t_version>
	struct SimpleSubgroupInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct SimpleSubgroupInformation<t_version> {
		IntegerU32          index;
		IntegerU32          resolution;
		IntegerU32          locale;
		PaddingBlock<4_szz> unused_1;
	};

	// ----------------

	template <auto t_version>
	struct GroupInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct GroupInformation<t_version> {
		StringBlockFixed128                                       id;
		StaticArray<SimpleSubgroupInformation<t_version>, 64_szz> subgroup_information;
		IntegerU32                                                subgroup_count;
	};

	// ----------------

	template <auto t_version>
	struct SubgroupInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct SubgroupInformation<t_version> {
		StringBlockFixed128  id;
		IntegerU32           offset;
		IntegerU32           size;
		IntegerU32           index;
		IntegerU32           resource_data_section_store_mode;
		IntegerU32           information_section_size;
		IntegerU32           generic_resource_data_section_offset;
		IntegerU32           generic_resource_data_section_size;
		IntegerU32           generic_resource_data_section_size_original;
		IntegerU32           generic_resource_data_section_size_original_1;
		IntegerU32           texture_resource_data_section_offset;
		IntegerU32           texture_resource_data_section_size;
		IntegerU32           texture_resource_data_section_size_original;
		PaddingBlock<20_szz> unused_1;
		IntegerU32           texture_resource_count;
		IntegerU32           texture_resource_begin_index;
	};

	// ----------------

	template <auto t_version>
	struct SubgroupPoolInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct SubgroupPoolInformation<t_version> {
		StringBlockFixed128  id;
		IntegerU32           texture_resource_data_section_offset;
		IntegerU32           texture_resource_data_section_size;
		IntegerU32           unknown_1;
		PaddingBlock<12_szz> unused_1;
	};

	// ----------------

	template <auto t_version>
	struct TextureResourceInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {0}))
	struct TextureResourceInformation<t_version> {
		IntegerU32 size_width;
		IntegerU32 size_height;
		IntegerU32 bit_per_channel_of_row_division_2;
		IntegerU32 format;
	};

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {1}))
	struct TextureResourceInformation<t_version> {
		IntegerU32 size_width;
		IntegerU32 size_height;
		IntegerU32 bit_per_channel_of_row_division_2;
		IntegerU32 format;
		IntegerU32 alpha_size;
	};

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {2}))
	struct TextureResourceInformation<t_version> {
		IntegerU32 size_width;
		IntegerU32 size_height;
		IntegerU32 bit_per_channel_of_row_division_2;
		IntegerU32 format;
		IntegerU32 alpha_size;
		IntegerU32 scale;
	};

	// ----------------

	template <auto t_version>
	struct Information;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct Information<t_version> {
		Header<t_version>                           header;
		Map<String, IntegerU32>                     resource_path;
		Map<String, IntegerU32>                     subgroup_id;
		List<GroupInformation<t_version>>           group_information;
		Map<String, IntegerU32>                     group_id;
		List<SubgroupInformation<t_version>>        subgroup_information;
		List<SubgroupPoolInformation<t_version>>    subgroup_pool_information;
		List<TextureResourceInformation<t_version>> texture_resource_information;
	};

	// ----------------

	template <auto t_version>
	struct ResourceBasicDetailDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct ResourceBasicDetailDescriptionInformation<t_version> {
		PaddingBlock<4_szz> unused_1;
		IntegerU16          type;
		IntegerU16          header_size;
		IntegerU32          property_information_offset;
		IntegerU32          image_property_information_offset;
		IntegerU32          id_offset;
		IntegerU32          path_offset;
		IntegerU32          property_information_count;
	};

	// ----------------

	template <auto t_version>
	struct ResourceImagePropertyDetailDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct ResourceImagePropertyDetailDescriptionInformation<t_version> {
		IntegerU16 type;
		IntegerU16 flag;
		IntegerU16 x;
		IntegerU16 y;
		IntegerU16 ax;
		IntegerU16 ay;
		IntegerU16 aw;
		IntegerU16 ah;
		IntegerU16 rows;
		IntegerU16 cols;
		IntegerU32 parent_offset;
	};

	// ----------------

	template <auto t_version>
	struct ResourcePropertyDetailDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct ResourcePropertyDetailDescriptionInformation<t_version> {
		IntegerU32          key_offset;
		PaddingBlock<4_szz> unused_1; // NOTE : value type ?
		IntegerU32          value_offset;
	};

	// ----------------

	template <auto t_version>
	struct ResourceDetailDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct ResourceDetailDescriptionInformation<t_version> :
		ResourceBasicDetailDescriptionInformation<t_version> {
		Optional<ResourceImagePropertyDetailDescriptionInformation<t_version>> image_property_information;
		List<ResourcePropertyDetailDescriptionInformation<t_version>>          property_information;
	};

	// ----------------

	template <auto t_version>
	struct ResourceBasicDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct ResourceBasicDescriptionInformation<t_version> {
		IntegerU32 detail_offset;
	};

	// ----------------

	template <auto t_version>
	struct ResourceDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct ResourceDescriptionInformation<t_version> :
		ResourceBasicDescriptionInformation<t_version> {
	};

	// ----------------

	template <auto t_version>
	struct SubgroupBasicDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct SubgroupBasicDescriptionInformation<t_version> {
		IntegerU32 resolution;
		IntegerU32 locale;
		IntegerU32 id_offset;
		IntegerU32 resource_count;
	};

	// ----------------

	template <auto t_version>
	struct SubgroupDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct SubgroupDescriptionInformation<t_version> :
		SubgroupBasicDescriptionInformation<t_version> {
		List<ResourceDescriptionInformation<t_version>> resource_information;
	};

	// ----------------

	template <auto t_version>
	struct GroupBasicDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct GroupBasicDescriptionInformation<t_version> {
		IntegerU32 id_offset;
		IntegerU32 subgroup_count;
		IntegerU32 subgroup_information_size;
	};

	// ----------------

	template <auto t_version>
	struct GroupDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct GroupDescriptionInformation<t_version> :
		GroupBasicDescriptionInformation<t_version> {
		List<SubgroupDescriptionInformation<t_version>> subgroup_information;
	};

}

namespace TwinKleS::Core {

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3}, {}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSB::Structure::Header<t_version>),
		M_wrap(unused_1, information_section_size, resource_path_section_size, resource_path_section_offset, unused_2, subgroup_id_section_size, subgroup_id_section_offset, subgroup_information_section_block_count, subgroup_information_section_offset, subgroup_information_section_block_size, group_information_section_block_count, group_information_section_offset, group_information_section_block_size, group_id_section_size, group_id_section_offset, subgroup_pool_information_section_block_count, subgroup_pool_information_section_offset, subgroup_pool_information_section_block_size, texture_resource_information_section_block_count, texture_resource_information_section_offset, texture_resource_information_section_block_size, description_group_section_offset, description_resource_section_offset, description_string_section_offset),
	);

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {4}, {}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSB::Structure::Header<t_version>),
		M_wrap(unused_1, information_section_size, resource_path_section_size, resource_path_section_offset, unused_2, subgroup_id_section_size, subgroup_id_section_offset, subgroup_information_section_block_count, subgroup_information_section_offset, subgroup_information_section_block_size, group_information_section_block_count, group_information_section_offset, group_information_section_block_size, group_id_section_size, group_id_section_offset, subgroup_pool_information_section_block_count, subgroup_pool_information_section_offset, subgroup_pool_information_section_block_size, texture_resource_information_section_block_count, texture_resource_information_section_offset, texture_resource_information_section_block_size, description_group_section_offset, description_resource_section_offset, description_string_section_offset, information_without_description_section_size),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSB::Structure::SimpleSubgroupInformation<t_version>),
		M_wrap(index, resolution, locale, unused_1),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSB::Structure::GroupInformation<t_version>),
		M_wrap(id, subgroup_information, subgroup_count),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSB::Structure::SubgroupInformation<t_version>),
		M_wrap(id, offset, size, index, resource_data_section_store_mode, information_section_size, generic_resource_data_section_offset, generic_resource_data_section_size, generic_resource_data_section_size_original, generic_resource_data_section_size_original_1, texture_resource_data_section_offset, texture_resource_data_section_size, texture_resource_data_section_size_original, unused_1, texture_resource_count, texture_resource_begin_index),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSB::Structure::SubgroupPoolInformation<t_version>),
		M_wrap(id, texture_resource_data_section_offset, texture_resource_data_section_size, unknown_1, unused_1),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {0}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSB::Structure::TextureResourceInformation<t_version>),
		M_wrap(size_width, size_height, bit_per_channel_of_row_division_2, format),
	);

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {1}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSB::Structure::TextureResourceInformation<t_version>),
		M_wrap(size_width, size_height, bit_per_channel_of_row_division_2, format, alpha_size),
	);

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {2}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSB::Structure::TextureResourceInformation<t_version>),
		M_wrap(size_width, size_height, bit_per_channel_of_row_division_2, format, alpha_size, scale),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSB::Structure::ResourcePropertyDetailDescriptionInformation<t_version>),
		M_wrap(key_offset, unused_1, value_offset),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSB::Structure::ResourceBasicDetailDescriptionInformation<t_version>),
		M_wrap(unused_1, type, header_size, property_information_offset, image_property_information_offset, id_offset, path_offset, property_information_count),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSB::Structure::ResourceImagePropertyDetailDescriptionInformation<t_version>),
		M_wrap(type, flag, x, y, ax, ay, aw, ah, rows, cols, parent_offset),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
	struct ByteStreamAdapter<Tool::PopCap::RSB::Structure::ResourceDetailDescriptionInformation<t_version>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Tool::PopCap::RSB::Structure::ResourceDetailDescriptionInformation<t_version>;

		// ----------------

		static auto size (
			That const & that
		) -> Size {
			using namespace Tool::PopCap::RSB::Structure;
			auto size = k_none_size;
			size += bs_size(up_cast<ResourceBasicDetailDescriptionInformation<t_version>>(that));
			if (that.image_property_information_offset != 0_iu32) {
				size += bs_size(that.image_property_information.get());
			}
			size += bs_size(that.property_information);
			return size;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using namespace Tool::PopCap::RSB::Structure;
			thix.write(up_cast<ResourceBasicDetailDescriptionInformation<t_version>>(that));
			if (that.image_property_information_offset != 0_iu32) {
				thix.write(that.image_property_information.get());
			}
			thix.write(that.property_information);
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using namespace Tool::PopCap::RSB::Structure;
			thix.read(up_cast<ResourceBasicDetailDescriptionInformation<t_version>>(that));
			if (that.image_property_information_offset != 0_iu32) {
				assert_condition(cbw<Size>(that.image_property_information_offset) == thix.position());
				thix.read(that.image_property_information.set());
			} else {
				that.image_property_information.reset();
			}
			assert_condition(cbw<Size>(that.property_information_offset) == thix.position());
			thix.read(that.property_information, cbw<Size>(that.property_information_count));
			return;
		}

	};

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSB::Structure::ResourceBasicDescriptionInformation<t_version>),
		M_wrap(detail_offset),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
	struct ByteStreamAdapter<Tool::PopCap::RSB::Structure::ResourceDescriptionInformation<t_version>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Tool::PopCap::RSB::Structure::ResourceDescriptionInformation<t_version>;

		// ----------------

		static auto size (
			That const & that
		) -> Size {
			using namespace Tool::PopCap::RSB::Structure;
			auto size = k_none_size;
			size += bs_size(up_cast<ResourceBasicDescriptionInformation<t_version>>(that));
			return size;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using namespace Tool::PopCap::RSB::Structure;
			thix.write(up_cast<ResourceBasicDescriptionInformation<t_version>>(that));
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using namespace Tool::PopCap::RSB::Structure;
			thix.read(up_cast<ResourceBasicDescriptionInformation<t_version>>(that));
			return;
		}

	};

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSB::Structure::SubgroupBasicDescriptionInformation<t_version>),
		M_wrap(resolution, locale, id_offset, resource_count),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
	struct ByteStreamAdapter<Tool::PopCap::RSB::Structure::SubgroupDescriptionInformation<t_version>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Tool::PopCap::RSB::Structure::SubgroupDescriptionInformation<t_version>;

		// ----------------

		static auto size (
			That const & that
		) -> Size {
			using namespace Tool::PopCap::RSB::Structure;
			auto size = k_none_size;
			size += bs_size(up_cast<SubgroupBasicDescriptionInformation<t_version>>(that));
			size += bs_size(that.resource_information);
			return size;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using namespace Tool::PopCap::RSB::Structure;
			thix.write(up_cast<SubgroupBasicDescriptionInformation<t_version>>(that));
			thix.write(that.resource_information);
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using namespace Tool::PopCap::RSB::Structure;
			thix.read(up_cast<SubgroupBasicDescriptionInformation<t_version>>(that));
			thix.read(that.resource_information, cbw<Size>(that.resource_count));
			return;
		}

	};

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSB::Structure::GroupBasicDescriptionInformation<t_version>),
		M_wrap(id_offset, subgroup_count, subgroup_information_size),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
	struct ByteStreamAdapter<Tool::PopCap::RSB::Structure::GroupDescriptionInformation<t_version>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Tool::PopCap::RSB::Structure::GroupDescriptionInformation<t_version>;

		// ----------------

		static auto size (
			That const & that
		) -> Size {
			using namespace Tool::PopCap::RSB::Structure;
			auto size = k_none_size;
			size += bs_size(up_cast<GroupBasicDescriptionInformation<t_version>>(that));
			size += bs_size(that.subgroup_information);
			return size;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using namespace Tool::PopCap::RSB::Structure;
			thix.write(up_cast<GroupBasicDescriptionInformation<t_version>>(that));
			thix.write(that.subgroup_information);
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using namespace Tool::PopCap::RSB::Structure;
			thix.read(up_cast<GroupBasicDescriptionInformation<t_version>>(that));
			thix.read(that.subgroup_information, cbw<Size>(that.subgroup_count));
			return;
		}

	};

}
