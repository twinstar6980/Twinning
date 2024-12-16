module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_group.structure;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_group.version;
import twinning.kernel.tool.popcap.resource_stream_bundle.shared;

export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamGroup::Structure {

	#pragma region magic identifier

	using MagicIdentifier = IntegerU32;

	inline constexpr auto k_magic_identifier = MagicIdentifier{0x72736770_iu32};

	#pragma endregion

	#pragma region version number

	using VersionNumber = IntegerU32;

	#pragma endregion

	#pragma region information

	template <auto t_version>
	struct Header;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}))
	M_record_of_data(
		M_wrap(Header<t_version>),
		M_wrap(
			(IntegerU32) unknown_1,
			(ConstantBlock<0x00000000_iu32>) unknown_2,
			(IntegerU32) resource_data_section_compression,
			(IntegerU32) information_section_size,
			(IntegerU32) general_resource_data_section_offset,
			(IntegerU32) general_resource_data_section_size,
			(IntegerU32) general_resource_data_section_size_original,
			(ConstantBlock<0x00000000_iu32>) unknown_8,
			(IntegerU32) texture_resource_data_section_offset,
			(IntegerU32) texture_resource_data_section_size,
			(IntegerU32) texture_resource_data_section_size_original,
			(ConstantBlock<0x00000000_iu32>) unknown_12,
			(ConstantBlock<0x00000000_iu32>) unknown_13,
			(ConstantBlock<0x00000000_iu32>) unknown_14,
			(ConstantBlock<0x00000000_iu32>) unknown_15,
			(ConstantBlock<0x00000000_iu32>) unknown_16,
			(IntegerU32) resource_information_section_size,
			(IntegerU32) resource_information_section_offset,
			(ConstantBlock<0x00000000_iu32>) unknown_19,
			(ConstantBlock<0x00000000_iu32>) unknown_20,
			(ConstantBlock<0x00000000_iu32>) unknown_21,
		),
	);

	// ----------------

	template <auto t_version>
	struct ResourceBasicInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}))
	M_record_of_data(
		M_wrap(ResourceBasicInformation<t_version>),
		M_wrap(
			(IntegerU32) type,
			(IntegerU32) offset,
			(IntegerU32) size,
		),
	);

	// ----------------

	template <auto t_version>
	struct GeneralResourceAdditionalInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}))
	M_record_of_data(
		M_wrap(GeneralResourceAdditionalInformation<t_version>),
		M_wrap(
		),
	);

	// ----------------

	template <auto t_version>
	struct TextureResourceAdditionalInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}))
	M_record_of_data(
		M_wrap(TextureResourceAdditionalInformation<t_version>),
		M_wrap(
			(IntegerU32) index,
			(ConstantBlock<0x00000000_iu32>) unknown_2,
			(ConstantBlock<0x00000000_iu32>) unknown_3,
			(IntegerU32) size_width,
			(IntegerU32) size_height,
		),
	);

	// ----------------
	// TODO

	template <auto t_version>
	struct ResourceInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}))
	struct ResourceInformation<t_version> :
		ResourceBasicInformation<t_version> {

		EnumerableVariant<ResourceType, GeneralResourceAdditionalInformation<t_version>, TextureResourceAdditionalInformation<t_version>> additional{};

	};

	// ----------------

	template <auto t_version>
	struct Information;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}))
	M_record_of_data(
		M_wrap(Information<t_version>),
		M_wrap(
			(Header<t_version>) header,
			(Map<String, ResourceInformation<t_version>>) resource_information,
		),
	);

	#pragma endregion

	#pragma region miscellaneous

	template <auto t_version>
	struct ResourceTypeFlag;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}))
	struct ResourceTypeFlag<t_version> {

		inline static constexpr auto general = IntegerU32{0x00000000_iu32};

		inline static constexpr auto texture = IntegerU32{0x00000001_iu32};

	};

	#pragma endregion

}

// NOTE : MORESPACE

export namespace Twinning::Kernel {

	template <auto t_version> requires
		AutoConstraint
	struct ByteStreamAdapter<Tool::PopCap::ResourceStreamGroup::Structure::ResourceInformation<t_version>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Tool::PopCap::ResourceStreamGroup::Structure::ResourceInformation<t_version>;

		// ----------------

		inline static auto size (
			That const & that
		) -> Size {
			using namespace Tool::PopCap::ResourceStreamGroup::Structure;
			auto size = k_none_size;
			size += bs_size(up_cast<ResourceBasicInformation<t_version>>(that));
			if (that.type == ResourceTypeFlag<t_version>::general) {
				size += bs_size(that.additional.template get<GeneralResourceAdditionalInformation<t_version>>());
			}
			else if (that.type == ResourceTypeFlag<t_version>::texture) {
				size += bs_size(that.additional.template get<TextureResourceAdditionalInformation<t_version>>());
			}
			else {
				assert_fail(R"(that.type == /* valid */)");
			}
			return size;
		}

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamGroup::Structure;
			thix.write(up_cast<ResourceBasicInformation<t_version>>(that));
			if (that.type == ResourceTypeFlag<t_version>::general) {
				thix.write(that.additional.template get<GeneralResourceAdditionalInformation<t_version>>());
			}
			else if (that.type == ResourceTypeFlag<t_version>::texture) {
				thix.write(that.additional.template get<TextureResourceAdditionalInformation<t_version>>());
			}
			else {
				assert_fail(R"(that.type == /* valid */)");
			}
			return;
		}

		inline static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using namespace Tool::PopCap::ResourceStreamGroup::Structure;
			thix.read(up_cast<ResourceBasicInformation<t_version>>(that));
			if (that.type == ResourceTypeFlag<t_version>::general) {
				thix.read(that.additional.template set<GeneralResourceAdditionalInformation<t_version>>());
			}
			else if (that.type == ResourceTypeFlag<t_version>::texture) {
				thix.read(that.additional.template set<TextureResourceAdditionalInformation<t_version>>());
			}
			else {
				assert_fail(R"(that.type == /* valid */)");
			}
			return;
		}

	};

}
