#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/zlib/version.hpp"
#include "core/tool/data/compression/deflate.hpp"

namespace TwinStar::Core::Tool::PopCap::ZLib {

	template <auto version> requires (check_version(version, {}))
	struct CompressCommon {

	protected:

		using MagicIdentifier = IntegerU32;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{0xDEADFED4_iu32};

	};

	template <auto version> requires (check_version(version, {}))
	struct Compress :
		CompressCommon<version> {

	protected:

		using Common = CompressCommon<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		// ----------------

		static auto compute_size_bound (
			Size const & raw_size,
			Size &       ripe_size_bound,
			Size const & window_bits,
			Size const & memory_level
		) -> Void {
			ripe_size_bound = k_none_size;
			ripe_size_bound += bs_static_size<MagicIdentifier>();
			if constexpr (version.variant_64) {
				ripe_size_bound += bs_static_size<IntegerU32>();
			}
			if constexpr (!version.variant_64) {
				ripe_size_bound += bs_static_size<IntegerU32>();
			} else {
				ripe_size_bound += bs_static_size<IntegerU64>();
			}
			auto ripe_data_size_bound = Size{};
			Data::Compression::Deflate::Compress::do_compute_size_bound(raw_size, ripe_data_size_bound, window_bits, memory_level, Data::Compression::Deflate::Wrapper::Constant::zlib());
			ripe_size_bound += ripe_data_size_bound;
			return;
		}

		// ----------------

		static auto process_whole (
			IByteStreamView &                            raw,
			OByteStreamView &                            ripe,
			Size const &                                 level,
			Size const &                                 window_bits,
			Size const &                                 memory_level,
			Data::Compression::Deflate::Strategy const & strategy
		) -> Void {
			ripe.write_constant(k_magic_identifier);
			if constexpr (version.variant_64) {
				ripe.write_constant(0x00000000_iu32);
			}
			if constexpr (!version.variant_64) {
				ripe.write(cbw<IntegerU32>(raw.reserve()));
			} else {
				ripe.write(cbw<IntegerU64>(raw.reserve()));
			}
			Data::Compression::Deflate::Compress::do_process_whole(raw, ripe, level, window_bits, memory_level, strategy, Data::Compression::Deflate::Wrapper::Constant::zlib());
			return;
		}

	public:

		static auto do_compute_size_bound (
			Size const & raw_size,
			Size &       ripe_size_bound,
			Size const & window_bits,
			Size const & memory_level
		) -> Void {
			restruct(ripe_size_bound);
			return compute_size_bound(raw_size, ripe_size_bound, window_bits, memory_level);
		}

		static auto do_process_whole (
			IByteStreamView &                            raw_,
			OByteStreamView &                            ripe_,
			Size const &                                 level,
			Size const &                                 window_bits,
			Size const &                                 memory_level,
			Data::Compression::Deflate::Strategy const & strategy
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe, level, window_bits, memory_level, strategy);
		}

	};

	template <auto version> requires (check_version(version, {}))
	struct Uncompress :
		CompressCommon<version> {

	protected:

		using Common = CompressCommon<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		// ----------------

		static auto compute_size (
			CByteListView const & ripe,
			Size &                raw_size
		) -> Void {
			raw_size = k_none_size;
			auto ripe_stream = IByteStreamView{ripe};
			ripe_stream.read_constant(k_magic_identifier);
			if constexpr (version.variant_64) {
				ripe_stream.read_constant(0x00000000_iu32);
			}
			if constexpr (!version.variant_64) {
				raw_size += cbw<Size>(ripe_stream.read_of<IntegerU32>());
			} else {
				raw_size += cbw<Size>(ripe_stream.read_of<IntegerU64>());
			}
			return;
		}

		// ----------------

		static auto process_whole (
			IByteStreamView & ripe,
			OByteStreamView & raw,
			Size const &      window_bits
		) -> Void {
			ripe.read_constant(k_magic_identifier);
			if constexpr (version.variant_64) {
				ripe.read_constant(0x00000000_iu32);
			}
			auto size = Size{};
			if constexpr (!version.variant_64) {
				size = cbw<Size>(ripe.read_of<IntegerU32>());
			} else {
				size = cbw<Size>(ripe.read_of<IntegerU64>());
			}
			auto raw_begin = raw.position();
			Data::Compression::Deflate::Uncompress::do_process_whole(ripe, raw, window_bits, Data::Compression::Deflate::Wrapper::Constant::zlib());
			assert_test(raw.position() - raw_begin == size);
			return;
		}

	public:

		static auto do_compute_size (
			CByteListView const & ripe,
			Size &                raw_size
		) -> Void {
			restruct(raw_size);
			return compute_size(ripe, raw_size);
		}

		static auto do_process_whole (
			IByteStreamView & ripe_,
			OByteStreamView & raw_,
			Size const &      window_bits
		) -> Void {
			M_use_zps_of(ripe);
			M_use_zps_of(raw);
			return process_whole(ripe, raw, window_bits);
		}

	};

}
