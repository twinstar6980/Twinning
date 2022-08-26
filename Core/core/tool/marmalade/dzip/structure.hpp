#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/marmalade/dzip/version.hpp"
#include "core/tool/data/compress/lzma.hpp"

namespace TwinKleS::Core::Tool::Marmalade::DZip::Structure {

	#pragma region magic identifier

	using MagicIdentifier = StaticByteArray<4_szz>;

	inline constexpr auto k_magic_identifier = MagicIdentifier{{'D'_b, 'T'_b, 'R'_b, 'Z'_b}};

	#pragma endregion

	#pragma region version number

	using VersionNumber = IntegerU8;

	#pragma endregion

	#pragma region information

	template <auto t_version>
	struct ArchiveSetting;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}))
	struct ArchiveSetting<t_version> {
		IntegerU16    resource_file_count;
		IntegerU16    resource_directory_count;
		VersionNumber version;
	};

	// ----------------

	template <auto t_version>
	struct ResourceInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}))
	struct ResourceInformation<t_version> {
		IntegerU16       directory_index;
		List<IntegerU16> chunk_index;
	};

	// ----------------

	template <auto t_version>
	struct ChunkSetting;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}))
	struct ChunkSetting<t_version> {
		IntegerU16 archive_resource_count;
		IntegerU16 chunk_count;
	};

	// ----------------

	template <auto t_version>
	struct ChunkInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}))
	struct ChunkInformation<t_version> {
		IntegerU32 offset;
		IntegerU32 size_compressed;
		IntegerU32 size_uncompressed;
		IntegerU16 flag;
		IntegerU16 file;
	};

	// ----------------

	// todo
	template <auto t_version>
	struct ArchiveResourceInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}))
	struct ArchiveResourceInformation<t_version> {
	};

	// ----------------

	template <auto t_version>
	struct Information;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}))
	struct Information<t_version> {
		ArchiveSetting<t_version>                   archive_setting;
		List<String>                                resource_file;
		List<String>                                resource_directory;
		List<ResourceInformation<t_version>>        resource_information;
		ChunkSetting<t_version>                     chunk_setting;
		List<ChunkInformation<t_version>>           chunk_information;
		List<ArchiveResourceInformation<t_version>> archive_resource_information;
	};

	#pragma endregion

	#pragma region misc

	template <auto t_version>
	struct ChunkFlag;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}))
	struct ChunkFlag<t_version> {
		inline static constexpr auto k_count = Size{11_sz};
		inline static constexpr auto combuf = Size{1_ix};
		inline static constexpr auto unused_2 = Size{2_ix};
		inline static constexpr auto dzip = Size{3_ix};
		inline static constexpr auto zlib = Size{4_ix};
		inline static constexpr auto bzip2 = Size{5_ix};
		inline static constexpr auto mp3 = Size{6_ix};
		inline static constexpr auto jpeg = Size{7_ix};
		inline static constexpr auto zerod_out = Size{8_ix};
		inline static constexpr auto copy_coded = Size{9_ix};
		inline static constexpr auto lzma = Size{10_ix};
		inline static constexpr auto random_access = Size{11_ix};
	};

	// ----------------

	template <auto t_version>
	struct ChunkHeaderLzma;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}))
	struct ChunkHeaderLzma<t_version> {
		StaticByteArray<Data::Compress::Lzma::k_props_size.value> props;
		IntegerU32                                                size;
		PaddingBlock<4_szz>                                       unused_1;
	};

	#pragma endregion

}

namespace TwinKleS::Core {

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Marmalade::DZip::check_version(t_version, {0}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::Marmalade::DZip::Structure::ArchiveSetting<t_version>),
		M_wrap(resource_file_count, resource_directory_count, version),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
	struct ByteStreamAdapter<Tool::Marmalade::DZip::Structure::ResourceInformation<t_version>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Tool::Marmalade::DZip::Structure::ResourceInformation<t_version>;

		// ----------------

		static auto size (
			That const & that
		) -> Size {
			using namespace Tool::Marmalade::DZip::Structure;
			auto size = k_none_size;
			size += bs_size(that.directory_index);
			size += bs_size(that.chunk_index);
			size += bs_static_size<IntegerU16>();
			return size;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using namespace Tool::Marmalade::DZip::Structure;
			thix.write(that.directory_index);
			thix.write(that.chunk_index);
			thix.write(~0_iu16); // TODO
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using namespace Tool::Marmalade::DZip::Structure;
			thix.read(that.directory_index);
			that.chunk_index.reset();
			while (k_true) {
				auto chunk_index = thix.read_of<IntegerU16>();
				if (chunk_index == ~0_iu16) {
					break;
				}
				that.chunk_index.append(chunk_index);
			}
			return;
		}

	};

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Marmalade::DZip::check_version(t_version, {0}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::Marmalade::DZip::Structure::ChunkSetting<t_version>),
		M_wrap(archive_resource_count, chunk_count),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Marmalade::DZip::check_version(t_version, {0}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::Marmalade::DZip::Structure::ChunkInformation<t_version>),
		M_wrap(offset, size_compressed, size_uncompressed, flag, file),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Marmalade::DZip::check_version(t_version, {0}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::Marmalade::DZip::Structure::ArchiveResourceInformation<t_version>),
		M_wrap(),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Marmalade::DZip::check_version(t_version, {0}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::Marmalade::DZip::Structure::ChunkHeaderLzma<t_version>),
		M_wrap(props, size, unused_1),
	);

}
