module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.zlib.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.zlib.version;
import twinning.kernel.tool.popcap.zlib.common;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.tool.data.compression.deflate.compress;

export namespace Twinning::Kernel::Tool::Popcap::Zlib {

	template <auto t_version> requires (check_version(t_version, {}))
	struct Compress :
		Common<t_version> {

		using Common = Common<t_version>;

		using typename Common::MagicMarker;

		using Common::k_magic_marker;

		using typename Common::IntegerOfPlatform;

		using typename Common::Header;

		// ----------------

		inline static auto process_whole(
			InputBinaryStreamView &                          raw,
			OutputBinaryStreamView &                         ripe,
			Integer const &                                  level,
			Integer const &                                  window_exponent,
			Integer const &                                  memory_level,
			Data::Compression::Deflate::StrategyMode const & strategy
		) -> Void {
			ripe.write_constant(k_magic_marker);
			if constexpr (check_version(t_version, {true})) {
				ripe.write_constant(0x00000000_iu32);
			}
			auto header = Header{};
			header.raw_size = cast_box<IntegerOfPlatform>(raw.reserve());
			ripe.write(header);
			Data::Compression::Deflate::Compress::process(raw, ripe, Data::Compression::Deflate::Container::Constant::zlib(), level, window_exponent, memory_level, strategy);
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size const &    raw_size,
			Size &          ripe_size_bound,
			Integer const & window_exponent,
			Integer const & memory_level
		) -> Void {
			ripe_size_bound = 0_sz;
			ripe_size_bound += binary_static_size_of<MagicMarker>();
			if constexpr (check_version(t_version, {true})) {
				ripe_size_bound += binary_static_size_of<IntegerU32>();
			}
			ripe_size_bound += binary_static_size_of<Header>();
			auto ripe_data_size_bound = Size{};
			Data::Compression::Deflate::Compress::estimate(raw_size, ripe_data_size_bound, Data::Compression::Deflate::Container::Constant::zlib(), window_exponent, memory_level);
			ripe_size_bound += ripe_data_size_bound;
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &                          raw_,
			OutputBinaryStreamView &                         ripe_,
			Integer const &                                  level,
			Integer const &                                  window_exponent,
			Integer const &                                  memory_level,
			Data::Compression::Deflate::StrategyMode const & strategy_mode
		) -> Void {
			M_use_split_stream_of(raw);
			M_use_split_stream_of(ripe);
			return process_whole(raw, ripe, level, window_exponent, memory_level, strategy_mode);
		}

		inline static auto estimate(
			Size const &    raw_size,
			Size &          ripe_size_bound,
			Integer const & window_exponent,
			Integer const & memory_level
		) -> Void {
			restruct(ripe_size_bound);
			return estimate_whole(raw_size, ripe_size_bound, window_exponent, memory_level);
		}

	};

}
