module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_bundle.structure;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_bundle.version;
import twinning.kernel.tool.popcap.resource_stream_bundle.shared;

export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamBundle::Structure {

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
		&& (check_version(t_version, {1, 4}, {}))
	M_record_of_data(
		M_wrap(Header<t_version>),
		M_wrap(
			(IntegerU32) unknown_1,
			(IntegerU32) information_section_size,
			(IntegerU32) resource_path_section_size,
			(IntegerU32) resource_path_section_offset,
			(ConstantBlock<0x00000000_iu32>) unknown_5,
			(ConstantBlock<0x00000000_iu32>) unknown_6,
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
			(IntegerU32) pool_information_section_block_count,
			(IntegerU32) pool_information_section_offset,
			(IntegerU32) pool_information_section_block_size,
			(IntegerU32) texture_resource_information_section_block_count,
			(IntegerU32) texture_resource_information_section_offset,
			(IntegerU32) texture_resource_information_section_block_size,
			(IntegerU32) group_manifest_information_section_offset,
			(IntegerU32) resource_manifest_information_section_offset,
			(IntegerU32) string_manifest_information_section_offset,
		),
	);

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {4}, {}))
	M_record_of_data(
		M_wrap(Header<t_version>),
		M_wrap(
			(IntegerU32) unknown_1,
			(IntegerU32) information_section_size,
			(IntegerU32) resource_path_section_size,
			(IntegerU32) resource_path_section_offset,
			(ConstantBlock<0x00000000_iu32>) unknown_5,
			(ConstantBlock<0x00000000_iu32>) unknown_6,
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
			(IntegerU32) pool_information_section_block_count,
			(IntegerU32) pool_information_section_offset,
			(IntegerU32) pool_information_section_block_size,
			(IntegerU32) texture_resource_information_section_block_count,
			(IntegerU32) texture_resource_information_section_offset,
			(IntegerU32) texture_resource_information_section_block_size,
			(IntegerU32) group_manifest_information_section_offset,
			(IntegerU32) resource_manifest_information_section_offset,
			(IntegerU32) string_manifest_information_section_offset,
			(IntegerU32) information_without_manifest_section_size,
		),
	);

	// ----------------

	template <auto t_version>
	struct SimpleSubgroupInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1, 3}, {}))
	M_record_of_data(
		M_wrap(SimpleSubgroupInformation<t_version>),
		M_wrap(
			(IntegerU32) index,
			(IntegerU32) resolution,
		),
	);

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	M_record_of_data(
		M_wrap(SimpleSubgroupInformation<t_version>),
		M_wrap(
			(IntegerU32) index,
			(IntegerU32) resolution,
			(IntegerU32) locale,
			(ConstantBlock<0x00000000_iu32>) unknown_4,
		),
	);

	// ----------------

	template <auto t_version>
	struct GroupInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}, {}))
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
		&& (check_version(t_version, {1, 3}, {}))
	M_record_of_data(
		M_wrap(SubgroupInformation<t_version>),
		M_wrap(
			(StringBlockFixed128) id,
			(IntegerU32) offset,
			(IntegerU32) size,
			(IntegerU32) pool,
			(IntegerU32) resource_data_section_compression,
			(IntegerU32) information_section_size,
			(IntegerU32) general_resource_data_section_offset,
			(IntegerU32) general_resource_data_section_size,
			(IntegerU32) general_resource_data_section_size_original,
			(IntegerU32) general_resource_data_section_size_pool,
			(IntegerU32) texture_resource_data_section_offset,
			(IntegerU32) texture_resource_data_section_size,
			(IntegerU32) texture_resource_data_section_size_original,
			(IntegerU32) texture_resource_data_section_size_pool,
			(ConstantBlock<0x00000000_iu32>) unknown_15,
			(ConstantBlock<0x00000000_iu32>) unknown_16,
			(ConstantBlock<0x00000000_iu32>) unknown_17,
			(ConstantBlock<0x00000000_iu32>) unknown_18,
		),
	);

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	M_record_of_data(
		M_wrap(SubgroupInformation<t_version>),
		M_wrap(
			(StringBlockFixed128) id,
			(IntegerU32) offset,
			(IntegerU32) size,
			(IntegerU32) pool,
			(IntegerU32) resource_data_section_compression,
			(IntegerU32) information_section_size,
			(IntegerU32) general_resource_data_section_offset,
			(IntegerU32) general_resource_data_section_size,
			(IntegerU32) general_resource_data_section_size_original,
			(IntegerU32) general_resource_data_section_size_pool,
			(IntegerU32) texture_resource_data_section_offset,
			(IntegerU32) texture_resource_data_section_size,
			(IntegerU32) texture_resource_data_section_size_original,
			(IntegerU32) texture_resource_data_section_size_pool,
			(ConstantBlock<0x00000000_iu32>) unknown_15,
			(ConstantBlock<0x00000000_iu32>) unknown_16,
			(ConstantBlock<0x00000000_iu32>) unknown_17,
			(ConstantBlock<0x00000000_iu32>) unknown_18,
			(IntegerU32) texture_resource_count,
			(IntegerU32) texture_resource_begin,
		),
	);

	// ----------------

	template <auto t_version>
	struct PoolInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}, {}))
	M_record_of_data(
		M_wrap(PoolInformation<t_version>),
		M_wrap(
			(StringBlockFixed128) id,
			(IntegerU32) texture_resource_data_section_offset,
			(IntegerU32) texture_resource_data_section_size,
			(IntegerU32) instance_count,
			(IntegerU32) flag,
			(IntegerU32) texture_resource_count,
			(IntegerU32) texture_resource_begin,
		),
	);

	// ----------------

	template <auto t_version>
	struct TextureResourceInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}, {0, 1}))
	M_record_of_data(
		M_wrap(TextureResourceInformation<t_version>),
		M_wrap(
			(IntegerU32) size_width,
			(IntegerU32) size_height,
			(IntegerU32) pitch,
			(IntegerU32) format,
		),
	);

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {4}, {1, 2}))
	M_record_of_data(
		M_wrap(TextureResourceInformation<t_version>),
		M_wrap(
			(IntegerU32) size_width,
			(IntegerU32) size_height,
			(IntegerU32) pitch,
			(IntegerU32) format,
			(IntegerU32) additional_byte_count,
		),
	);

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {4}, {2, 3}))
	M_record_of_data(
		M_wrap(TextureResourceInformation<t_version>),
		M_wrap(
			(IntegerU32) size_width,
			(IntegerU32) size_height,
			(IntegerU32) pitch,
			(IntegerU32) format,
			(IntegerU32) additional_byte_count,
			(IntegerU32) scale,
		),
	);

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {4}, {3}))
	M_record_of_data(
		M_wrap(TextureResourceInformation<t_version>),
		M_wrap(
			(IntegerU32) size_width,
			(IntegerU32) size_height,
			(IntegerU32) pitch,
			(IntegerU32) format,
			(IntegerU32) scale,
			(IntegerU32) additional_byte_count,
		),
	);

	// ----------------

	template <auto t_version>
	struct Information;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}, {}))
	M_record_of_data(
		M_wrap(Information<t_version>),
		M_wrap(
			(Header<t_version>) header,
			(Map<String, IntegerU32>) resource_path,
			(Map<String, IntegerU32>) subgroup_id,
			(List<GroupInformation<t_version>>) group_information,
			(Map<String, IntegerU32> ) group_id,
			(List<SubgroupInformation<t_version>>) subgroup_information,
			(List<PoolInformation<t_version>>) pool_information,
			(List<TextureResourceInformation<t_version>>) texture_resource_information,
		),
	);

	// ----------------

	template <auto t_version>
	struct ResourceBasicDetailManifestInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}, {}))
	M_record_of_data(
		M_wrap(ResourceBasicDetailManifestInformation<t_version>),
		M_wrap(
			(ConstantBlock<0x00000000_iu32>) unknown_1,
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
	struct ResourceImagePropertyDetailManifestInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}, {}))
	M_record_of_data(
		M_wrap(ResourceImagePropertyDetailManifestInformation<t_version>),
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
	struct ResourcePropertyDetailManifestInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}, {}))
	M_record_of_data(
		M_wrap(ResourcePropertyDetailManifestInformation<t_version>),
		M_wrap(
			(IntegerU32) key_offset,
			(ConstantBlock<0x00000000_iu32>) unknown_2, // TODO : value type ?
			(IntegerU32) value_offset,
		),
	);

	// ----------------
	// TODO

	template <auto t_version>
	struct ResourceDetailManifestInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}, {}))
	struct ResourceDetailManifestInformation<t_version> :
		ResourceBasicDetailManifestInformation<t_version> {

		Optional<ResourceImagePropertyDetailManifestInformation<t_version>> image_property_information{};

		List<ResourcePropertyDetailManifestInformation<t_version>> property_information{};

	};

	// ----------------

	template <auto t_version>
	struct ResourceBasicManifestInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}, {}))
	M_record_of_data(
		M_wrap(ResourceBasicManifestInformation<t_version>),
		M_wrap(
			(IntegerU32) detail_offset,
		),
	);

	// ----------------
	// TODO

	template <auto t_version>
	struct ResourceManifestInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}, {}))
	struct ResourceManifestInformation<t_version> :
		ResourceBasicManifestInformation<t_version> {

	};

	// ----------------

	template <auto t_version>
	struct SubgroupBasicManifestInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1, 3}, {}))
	M_record_of_data(
		M_wrap(SubgroupBasicManifestInformation<t_version>),
		M_wrap(
			(IntegerU32) resolution,
			(IntegerU32) id_offset,
			(IntegerU32) resource_count,
		),
	);

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3}, {}))
	M_record_of_data(
		M_wrap(SubgroupBasicManifestInformation<t_version>),
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
	struct SubgroupManifestInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}, {}))
	struct SubgroupManifestInformation<t_version> :
		SubgroupBasicManifestInformation<t_version> {

		List<ResourceManifestInformation<t_version>> resource_information{};

	};

	// ----------------

	template <auto t_version>
	struct GroupBasicManifestInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}, {}))
	M_record_of_data(
		M_wrap(GroupBasicManifestInformation<t_version>),
		M_wrap(
			(IntegerU32) id_offset,
			(IntegerU32) subgroup_count,
			(IntegerU32) subgroup_information_size,
		),
	);

	// ----------------
	// TODO

	template <auto t_version>
	struct GroupManifestInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}, {}))
	struct GroupManifestInformation<t_version> :
		GroupBasicManifestInformation<t_version> {

		List<SubgroupManifestInformation<t_version>> subgroup_information{};

	};

}

// NOTE : MORESPACE

export namespace Twinning::Kernel {

	template <auto t_version> requires
		AutoConstraint
	struct ByteStreamAdapter<Tool::PopCap::ResourceStreamBundle::Structure::ResourceDetailManifestInformation<t_version>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Tool::PopCap::ResourceStreamBundle::Structure::ResourceDetailManifestInformation<t_version>;

		// ----------------

		inline static auto size (
			That const & that
		) -> Size {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			auto size = k_none_size;
			size += bs_size(up_cast<ResourceBasicDetailManifestInformation<t_version>>(that));
			if (that.image_property_information_offset != 0_iu32) {
				size += bs_size(that.image_property_information.get());
			}
			size += bs_size(that.property_information);
			return size;
		}

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			thix.write(up_cast<ResourceBasicDetailManifestInformation<t_version>>(that));
			if (that.image_property_information_offset != 0_iu32) {
				thix.write(that.image_property_information.get());
			}
			thix.write(that.property_information);
			return;
		}

		inline static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			thix.read(up_cast<ResourceBasicDetailManifestInformation<t_version>>(that));
			if (that.image_property_information_offset != 0_iu32) {
				assert_test(cbox<Size>(that.image_property_information_offset) == thix.position());
				thix.read(that.image_property_information.set());
			}
			else {
				that.image_property_information.reset();
			}
			assert_test(cbox<Size>(that.property_information_offset) == thix.position());
			thix.read(that.property_information, cbox<Size>(that.property_information_count));
			return;
		}

	};

	// ----------------

	template <auto t_version> requires
		AutoConstraint
	struct ByteStreamAdapter<Tool::PopCap::ResourceStreamBundle::Structure::ResourceManifestInformation<t_version>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Tool::PopCap::ResourceStreamBundle::Structure::ResourceManifestInformation<t_version>;

		// ----------------

		inline static auto size (
			That const & that
		) -> Size {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			auto size = k_none_size;
			size += bs_size(up_cast<ResourceBasicManifestInformation<t_version>>(that));
			return size;
		}

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			thix.write(up_cast<ResourceBasicManifestInformation<t_version>>(that));
			return;
		}

		inline static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			thix.read(up_cast<ResourceBasicManifestInformation<t_version>>(that));
			return;
		}

	};

	// ----------------

	template <auto t_version> requires
		AutoConstraint
	struct ByteStreamAdapter<Tool::PopCap::ResourceStreamBundle::Structure::SubgroupManifestInformation<t_version>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Tool::PopCap::ResourceStreamBundle::Structure::SubgroupManifestInformation<t_version>;

		// ----------------

		inline static auto size (
			That const & that
		) -> Size {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			auto size = k_none_size;
			size += bs_size(up_cast<SubgroupBasicManifestInformation<t_version>>(that));
			size += bs_size(that.resource_information);
			return size;
		}

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			thix.write(up_cast<SubgroupBasicManifestInformation<t_version>>(that));
			thix.write(that.resource_information);
			return;
		}

		inline static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			thix.read(up_cast<SubgroupBasicManifestInformation<t_version>>(that));
			thix.read(that.resource_information, cbox<Size>(that.resource_count));
			return;
		}

	};

	// ----------------

	template <auto t_version> requires
		AutoConstraint
	struct ByteStreamAdapter<Tool::PopCap::ResourceStreamBundle::Structure::GroupManifestInformation<t_version>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Tool::PopCap::ResourceStreamBundle::Structure::GroupManifestInformation<t_version>;

		// ----------------

		inline static auto size (
			That const & that
		) -> Size {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			auto size = k_none_size;
			size += bs_size(up_cast<GroupBasicManifestInformation<t_version>>(that));
			size += bs_size(that.subgroup_information);
			return size;
		}

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			thix.write(up_cast<GroupBasicManifestInformation<t_version>>(that));
			thix.write(that.subgroup_information);
			return;
		}

		inline static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamBundle::Structure;
			thix.read(up_cast<GroupBasicManifestInformation<t_version>>(that));
			thix.read(that.subgroup_information, cbox<Size>(that.subgroup_count));
			return;
		}

	};

}
