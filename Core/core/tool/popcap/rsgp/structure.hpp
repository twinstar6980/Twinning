#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/rsgp/version.hpp"
#include "core/tool/popcap/rsb/common.hpp"

namespace TwinKleS::Core::Tool::PopCap::RSGP::Structure {

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
		&& (check_version(t_version, {3, 4}))
	struct Header<t_version> {
		PaddingBlock<8_szz>  unused_1;
		IntegerU32           resource_data_section_store_mode;
		IntegerU32           information_section_size;
		IntegerU32           generic_resource_data_section_offset;
		IntegerU32           generic_resource_data_section_size;
		IntegerU32           generic_resource_data_section_size_original;
		PaddingBlock<4_szz>  unused_2;
		IntegerU32           texture_resource_data_section_offset;
		IntegerU32           texture_resource_data_section_size;
		IntegerU32           texture_resource_data_section_size_original;
		PaddingBlock<20_szz> unused_3;
		IntegerU32           resource_information_section_size;
		IntegerU32           resource_information_section_offset;
		PaddingBlock<12_szz> unused_4;
	};

	// ----------------

	template <auto t_version>
	struct ResourceBasicInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}))
	struct ResourceBasicInformation<t_version> {
		IntegerU32 type;
		IntegerU32 offset;
		IntegerU32 size;
	};

	// ----------------

	template <auto t_version>
	struct GenericResourceAdditionalInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}))
	struct GenericResourceAdditionalInformation<t_version> {
	};

	// ----------------

	template <auto t_version>
	struct TextureResourceAdditionalInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}))
	struct TextureResourceAdditionalInformation<t_version> {
		IntegerU32          index;
		PaddingBlock<8_szz> unused_1;
		IntegerU32          size_width;
		IntegerU32          size_height;

	};

	// ----------------

	template <auto t_version>
	struct ResourceInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}))
	struct ResourceInformation<t_version> :
		ResourceBasicInformation<t_version> {
		EnumerableVariant<ResourceType, GenericResourceAdditionalInformation<t_version>, TextureResourceAdditionalInformation<t_version>> additional;
	};

	// ----------------

	template <auto t_version>
	struct Information;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}))
	struct Information<t_version> {
		Header<t_version>                           header;
		Map<String, ResourceInformation<t_version>> resource_information;
	};

	#pragma endregion

	#pragma region misc

	template <auto t_version>
	struct ResourceTypeFlag;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}))
	struct ResourceTypeFlag<t_version> {
		inline static constexpr auto generic = IntegerU32{0x00000000_iu32};
		inline static constexpr auto texture = IntegerU32{0x00000001_iu32};
	};

	#pragma endregion

}

namespace TwinKleS::Core {

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSGP::check_version(t_version, {3, 4}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSGP::Structure::Header<t_version>),
		M_wrap(unused_1, resource_data_section_store_mode, information_section_size, generic_resource_data_section_offset, generic_resource_data_section_size, generic_resource_data_section_size_original, unused_2, texture_resource_data_section_offset, texture_resource_data_section_size, texture_resource_data_section_size_original, unused_3, resource_information_section_size, resource_information_section_offset, unused_4),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSGP::check_version(t_version, {3, 4}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSGP::Structure::ResourceBasicInformation<t_version>),
		M_wrap(type, offset, size),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSGP::check_version(t_version, {3, 4}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSGP::Structure::GenericResourceAdditionalInformation<t_version>),
		M_wrap(),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSGP::check_version(t_version, {3, 4}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::RSGP::Structure::TextureResourceAdditionalInformation<t_version>),
		M_wrap(index, unused_1, size_width, size_height),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
	struct ByteStreamAdapter<Tool::PopCap::RSGP::Structure::ResourceInformation<t_version>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Tool::PopCap::RSGP::Structure::ResourceInformation<t_version>;

		// ----------------

		static auto size (
			That const & that
		) -> Size {
			using namespace Tool::PopCap::RSGP::Structure;
			auto size = k_none_size;
			size += bs_size(up_cast<ResourceBasicInformation<t_version>>(that));
			if (that.type == ResourceTypeFlag<t_version>::generic) {
				size += bs_size(that.additional.template get<GenericResourceAdditionalInformation<t_version>>());
			} else if (that.type == ResourceTypeFlag<t_version>::texture) {
				size += bs_size(that.additional.template get<TextureResourceAdditionalInformation<t_version>>());
			} else {
				assert_failed(R"(that.type == /* valid */)");
			}
			return size;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using namespace Tool::PopCap::RSGP::Structure;
			thix.write(up_cast<ResourceBasicInformation<t_version>>(that));
			if (that.type == ResourceTypeFlag<t_version>::generic) {
				thix.write(that.additional.template get<GenericResourceAdditionalInformation<t_version>>());
			} else if (that.type == ResourceTypeFlag<t_version>::texture) {
				thix.write(that.additional.template get<TextureResourceAdditionalInformation<t_version>>());
			} else {
				assert_failed(R"(that.type == /* valid */)");
			}
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using namespace Tool::PopCap::RSGP::Structure;
			thix.read(up_cast<ResourceBasicInformation<t_version>>(that));
			if (that.type == ResourceTypeFlag<t_version>::generic) {
				thix.read(that.additional.template set<GenericResourceAdditionalInformation<t_version>>());
			} else if (that.type == ResourceTypeFlag<t_version>::texture) {
				thix.read(that.additional.template set<TextureResourceAdditionalInformation<t_version>>());
			} else {
				assert_failed(R"(that.type == /* valid */)");
			}
			return;
		}

	};

}
