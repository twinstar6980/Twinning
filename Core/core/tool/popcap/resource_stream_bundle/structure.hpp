#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/resource_stream_bundle/version.hpp"
#include "core/tool/popcap/resource_stream_bundle/common.hpp"

namespace TwinStar::Core::Tool::PopCap::ResourceStreamBundle::Structure {

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
		&& (check_version(t_version, {3, 4}, {}))
	M_record_of_data(
		M_wrap(Header<t_version>),
		M_wrap(
			(PaddingBlock<4_sz>) unused_1,
			(IntegerU32) information_section_size,
			(IntegerU32) resource_path_section_size,
			(IntegerU32) resource_path_section_offset,
			(PaddingBlock<8_sz>) unused_2,
			(IntegerU32) subgroup_id_section_size,
			(IntegerU32) subgroup_id_section_offset,
			(IntegerU32) subgroup_information_section_block_count,
			(IntegerU32) subgroup_information_section_offset,
			(IntegerU32) subgroup_information_section_block_size,
			(IntegerU32) group_information_section_block_count,
			(IntegerU32) group_information_section_offset,
			(IntegerU32) group_information_section_block_size,
			(IntegerU32) group_id_section_size,
			(IntegerU32) group_id_section_offset,
			(IntegerU32) subgroup_pool_information_section_block_count,
			(IntegerU32) subgroup_pool_information_section_offset,
			(IntegerU32) subgroup_pool_information_section_block_size,
			(IntegerU32) texture_resource_information_section_block_count,
			(IntegerU32) texture_resource_information_section_offset,
			(IntegerU32) texture_resource_information_section_block_size,
			(IntegerU32) description_group_section_offset,
			(IntegerU32) description_resource_section_offset,
			(IntegerU32) description_string_section_offset,
		),
	);

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {4}, {}))
	M_record_of_data(
		M_wrap(Header<t_version>),
		M_wrap(
			(PaddingBlock<4_sz>) unused_1,
			(IntegerU32) information_section_size,
			(IntegerU32) resource_path_section_size,
			(IntegerU32) resource_path_section_offset,
			(PaddingBlock<8_sz>) unused_2,
			(IntegerU32) subgroup_id_section_size,
			(IntegerU32) subgroup_id_section_offset,
			(IntegerU32) subgroup_information_section_block_count,
			(IntegerU32) subgroup_information_section_offset,
			(IntegerU32) subgroup_information_section_block_size,
			(IntegerU32) group_information_section_block_count,
			(IntegerU32) group_information_section_offset,
			(IntegerU32) group_information_section_block_size,
			(IntegerU32) group_id_section_size,
			(IntegerU32) group_id_section_offset,
			(IntegerU32) subgroup_pool_information_section_block_count,
			(IntegerU32) subgroup_pool_information_section_offset,
			(IntegerU32) subgroup_pool_information_section_block_size,
			(IntegerU32) texture_resource_information_section_block_count,
			(IntegerU32) texture_resource_information_section_offset,
			(IntegerU32) texture_resource_information_section_block_size,
			(IntegerU32) description_group_section_offset,
			(IntegerU32) description_resource_section_offset,
			(IntegerU32) description_string_section_offset,
			(IntegerU32) information_without_description_section_size,
		),
	);

	// ----------------

	template <auto t_version>
	struct SimpleSubgroupInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	M_record_of_data(
		M_wrap(SimpleSubgroupInformation<t_version>),
		M_wrap(
			(IntegerU32) index,
			(IntegerU32) resolution,
			(IntegerU32) locale,
			(PaddingBlock<4_sz>) unused_1,
		),
	);

	// ----------------

	template <auto t_version>
	struct GroupInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	M_record_of_data(
		M_wrap(GroupInformation<t_version>),
		M_wrap(
			(StringBlockFixed128) id,
			(StaticArray<SimpleSubgroupInformation<t_version>, 64_sz>) subgroup_information,
			(IntegerU32) subgroup_count,
		),
	);

	// ----------------

	template <auto t_version>
	struct SubgroupInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	M_record_of_data(
		M_wrap(SubgroupInformation<t_version>),
		M_wrap(
			(StringBlockFixed128) id,
			(IntegerU32) offset,
			(IntegerU32) size,
			(IntegerU32) index,
			(IntegerU32) resource_data_section_store_mode,
			(IntegerU32) information_section_size,
			(IntegerU32) generic_resource_data_section_offset,
			(IntegerU32) generic_resource_data_section_size,
			(IntegerU32) generic_resource_data_section_size_original,
			(IntegerU32) generic_resource_data_section_size_original_1,
			(IntegerU32) texture_resource_data_section_offset,
			(IntegerU32) texture_resource_data_section_size,
			(IntegerU32) texture_resource_data_section_size_original,
			(PaddingBlock<20_sz>) unused_1,
			(IntegerU32) texture_resource_count,
			(IntegerU32) texture_resource_begin_index,
		),
	);

	// ----------------

	template <auto t_version>
	struct SubgroupPoolInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	M_record_of_data(
		M_wrap(SubgroupPoolInformation<t_version>),
		M_wrap(
			(StringBlockFixed128) id,
			(IntegerU32) texture_resource_data_section_offset,
			(IntegerU32) texture_resource_data_section_size,
			(IntegerU32) unknown_1,
			(PaddingBlock<12_sz>) unused_1,
		),
	);

	// ----------------

	template <auto t_version>
	struct TextureResourceInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {0}))
	M_record_of_data(
		M_wrap(TextureResourceInformation<t_version>),
		M_wrap(
			(IntegerU32) size_width,
			(IntegerU32) size_height,
			(IntegerU32) row_byte_count,
			(IntegerU32) format,
		),
	);

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {1}))
	M_record_of_data(
		M_wrap(TextureResourceInformation<t_version>),
		M_wrap(
			(IntegerU32) size_width,
			(IntegerU32) size_height,
			(IntegerU32) row_byte_count,
			(IntegerU32) format,
			(IntegerU32) alpha_size,
		),
	);

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {2}))
	M_record_of_data(
		M_wrap(TextureResourceInformation<t_version>),
		M_wrap(
			(IntegerU32) size_width,
			(IntegerU32) size_height,
			(IntegerU32) row_byte_count,
			(IntegerU32) format,
			(IntegerU32) alpha_size,
			(IntegerU32) scale,
		),
	);

	// ----------------

	template <auto t_version>
	struct Information;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	M_record_of_data(
		M_wrap(Information<t_version>),
		M_wrap(
			(Header<t_version>) header,
			(Map<String, IntegerU32>) resource_path,
			(Map<String, IntegerU32>) subgroup_id,
			(List<GroupInformation<t_version>>) group_information,
			(Map<String, IntegerU32> ) group_id,
			(List<SubgroupInformation<t_version>>) subgroup_information,
			(List<SubgroupPoolInformation<t_version>>) subgroup_pool_information,
			(List<TextureResourceInformation<t_version>>) texture_resource_information,
		),
	);

	// ----------------

	template <auto t_version>
	struct ResourceBasicDetailDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	M_record_of_data(
		M_wrap(ResourceBasicDetailDescriptionInformation<t_version>),
		M_wrap(
			(PaddingBlock<4_sz>) unused_1,
			(IntegerU16) type,
			(IntegerU16) header_size,
			(IntegerU32) property_information_offset,
			(IntegerU32) image_property_information_offset,
			(IntegerU32) id_offset,
			(IntegerU32) path_offset,
			(IntegerU32) property_information_count,
		),
	);

	// ----------------

	template <auto t_version>
	struct ResourceImagePropertyDetailDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	M_record_of_data(
		M_wrap(ResourceImagePropertyDetailDescriptionInformation<t_version>),
		M_wrap(
			(IntegerU16) type,
			(IntegerU16) flag,
			(IntegerU16) x,
			(IntegerU16) y,
			(IntegerU16) ax,
			(IntegerU16) ay,
			(IntegerU16) aw,
			(IntegerU16) ah,
			(IntegerU16) rows,
			(IntegerU16) cols,
			(IntegerU32) parent_offset,
		),
	);

	// ----------------

	template <auto t_version>
	struct ResourcePropertyDetailDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
	M_record_of_data(
		M_wrap(ResourcePropertyDetailDescriptionInformation<t_version>),
		M_wrap(
			(IntegerU32) key_offset,
			(PaddingBlock<4_sz>) unused_1, // TODO : value type ?
			(IntegerU32) value_offset,
		),
	);

	// ----------------
	// TODO

	template <auto t_version>
	struct ResourceDetailDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
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
		&& (check_version(t_version, {3}, {}))
	M_record_of_data(
		M_wrap(ResourceBasicDescriptionInformation<t_version>),
		M_wrap(
			(IntegerU32) detail_offset,
		),
	);

	// ----------------
	// TODO

	template <auto t_version>
	struct ResourceDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	struct ResourceDescriptionInformation<t_version> :
		ResourceBasicDescriptionInformation<t_version> {
	};

	// ----------------

	template <auto t_version>
	struct SubgroupBasicDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	M_record_of_data(
		M_wrap(SubgroupBasicDescriptionInformation<t_version>),
		M_wrap(
			(IntegerU32) resolution,
			(IntegerU32) locale,
			(IntegerU32) id_offset,
			(IntegerU32) resource_count,
		),
	);

	// ----------------
	// TODO

	template <auto t_version>
	struct SubgroupDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	struct SubgroupDescriptionInformation<t_version> :
		SubgroupBasicDescriptionInformation<t_version> {
		List<ResourceDescriptionInformation<t_version>> resource_information;
	};

	// ----------------

	template <auto t_version>
	struct GroupBasicDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	M_record_of_data(
		M_wrap(GroupBasicDescriptionInformation<t_version>),
		M_wrap(
			(IntegerU32) id_offset,
			(IntegerU32) subgroup_count,
			(IntegerU32) subgroup_information_size,
		),
	);

	// ----------------
	// TODO

	template <auto t_version>
	struct GroupDescriptionInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	struct GroupDescriptionInformation<t_version> :
		GroupBasicDescriptionInformation<t_version> {
		List<SubgroupDescriptionInformation<t_version>> subgroup_information;
	};

}

namespace TwinStar::Core {

	template <auto t_version> requires
		AutoConstraint
	struct ByteStreamAdapter<Tool::PopCap::ResourceStreamBundle::Structure::ResourceDetailDescriptionInformation<t_version>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Tool::PopCap::ResourceStreamBundle::Structure::ResourceDetailDescriptionInformation<t_version>;

		// ----------------

		static auto size (
			That const & that
		) -> Size {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
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
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
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
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			thix.read(up_cast<ResourceBasicDetailDescriptionInformation<t_version>>(that));
			if (that.image_property_information_offset != 0_iu32) {
				assert_test(cbw<Size>(that.image_property_information_offset) == thix.position());
				thix.read(that.image_property_information.set());
			} else {
				that.image_property_information.reset();
			}
			assert_test(cbw<Size>(that.property_information_offset) == thix.position());
			thix.read(that.property_information, cbw<Size>(that.property_information_count));
			return;
		}

	};

	// ----------------

	template <auto t_version> requires
		AutoConstraint
	struct ByteStreamAdapter<Tool::PopCap::ResourceStreamBundle::Structure::ResourceDescriptionInformation<t_version>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Tool::PopCap::ResourceStreamBundle::Structure::ResourceDescriptionInformation<t_version>;

		// ----------------

		static auto size (
			That const & that
		) -> Size {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			auto size = k_none_size;
			size += bs_size(up_cast<ResourceBasicDescriptionInformation<t_version>>(that));
			return size;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			thix.write(up_cast<ResourceBasicDescriptionInformation<t_version>>(that));
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			thix.read(up_cast<ResourceBasicDescriptionInformation<t_version>>(that));
			return;
		}

	};

	// ----------------

	template <auto t_version> requires
		AutoConstraint
	struct ByteStreamAdapter<Tool::PopCap::ResourceStreamBundle::Structure::SubgroupDescriptionInformation<t_version>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Tool::PopCap::ResourceStreamBundle::Structure::SubgroupDescriptionInformation<t_version>;

		// ----------------

		static auto size (
			That const & that
		) -> Size {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			auto size = k_none_size;
			size += bs_size(up_cast<SubgroupBasicDescriptionInformation<t_version>>(that));
			size += bs_size(that.resource_information);
			return size;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			thix.write(up_cast<SubgroupBasicDescriptionInformation<t_version>>(that));
			thix.write(that.resource_information);
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			thix.read(up_cast<SubgroupBasicDescriptionInformation<t_version>>(that));
			thix.read(that.resource_information, cbw<Size>(that.resource_count));
			return;
		}

	};

	// ----------------

	template <auto t_version> requires
		AutoConstraint
	struct ByteStreamAdapter<Tool::PopCap::ResourceStreamBundle::Structure::GroupDescriptionInformation<t_version>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Tool::PopCap::ResourceStreamBundle::Structure::GroupDescriptionInformation<t_version>;

		// ----------------

		static auto size (
			That const & that
		) -> Size {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			auto size = k_none_size;
			size += bs_size(up_cast<GroupBasicDescriptionInformation<t_version>>(that));
			size += bs_size(that.subgroup_information);
			return size;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			thix.write(up_cast<GroupBasicDescriptionInformation<t_version>>(that));
			thix.write(that.subgroup_information);
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			thix.read(up_cast<GroupBasicDescriptionInformation<t_version>>(that));
			thix.read(that.subgroup_information, cbw<Size>(that.subgroup_count));
			return;
		}

	};

}
