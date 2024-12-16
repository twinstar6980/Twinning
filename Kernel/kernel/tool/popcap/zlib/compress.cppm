module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.zlib.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.zlib.version;
import twinning.kernel.tool.popcap.zlib.common;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.tool.data.compression.deflate.compress;

export namespace Twinning::Kernel::Tool::PopCap::ZLib {

	template <auto version> requires (check_version(version, {}))
	struct Compress :
		Common<version> {

		using Common = Common<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::IntegerOfPlatform;

		using typename Common::Header;

		// ----------------

		inline static auto process_whole (
			IByteStreamView &                            raw,
			OByteStreamView &                            ripe,
			Size const &                                 level,
			Size const &                                 window_bits,
			Size const &                                 memory_level,
			Data::Compression::Deflate::Strategy const & strategy
		) -> Void {
			ripe.write_constant(k_magic_identifier);
			if constexpr (check_version(version, {true})) {
				ripe.write_constant(0x00000000_iu32);
			}
			auto header = Header{};
			header.raw_size = cbox<IntegerOfPlatform>(raw.reserve());
			ripe.write(header);
			Data::Compression::Deflate::Compress::process(raw, ripe, level, window_bits, memory_level, strategy, Data::Compression::Deflate::Wrapper::Constant::zlib());
			return;
		}

		// ----------------

		inline static auto estimate_whole (
			Size const & raw_size,
			Size &       ripe_size_bound,
			Size const & window_bits,
			Size const & memory_level
		) -> Void {
			ripe_size_bound = k_none_size;
			ripe_size_bound += bs_static_size<MagicIdentifier>();
			if constexpr (check_version(version, {true})) {
				ripe_size_bound += bs_static_size<IntegerU32>();
			}
			ripe_size_bound += bs_static_size<Header>();
			auto ripe_data_size_bound = Size{};
			Data::Compression::Deflate::Compress::estimate(raw_size, ripe_data_size_bound, window_bits, memory_level, Data::Compression::Deflate::Wrapper::Constant::zlib());
			ripe_size_bound += ripe_data_size_bound;
			return;
		}

		// ----------------

		inline static auto process (
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

		inline static auto estimate (
			Size const & raw_size,
			Size &       ripe_size_bound,
			Size const & window_bits,
			Size const & memory_level
		) -> Void {
			restruct(ripe_size_bound);
			return estimate_whole(raw_size, ripe_size_bound, window_bits, memory_level);
		}

	};

}
