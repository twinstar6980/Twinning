module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.lzma.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.lzma.common;
import twinning.kernel.third.lzma;

export namespace Twinning::Kernel::Tool::Data::Compression::Lzma {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputByteStreamView &  raw,
			OutputByteStreamView & ripe,
			Integer const &        level
		) -> Void {
			assert_test(Math::between(level, 0_i, 9_i));
			auto property = ripe.forward_view(make_box<Size>(Third::lzma::$LZMA_PROPS_SIZE));
			ripe.write(cast_box<IntegerU64>(raw.reserve()));
			auto lz_raw_size = unmake_box<std::size_t>(raw.reserve());
			auto lz_ripe_size = unmake_box<std::size_t>(ripe.reserve());
			auto lz_property_size = unmake_box<std::size_t>(property.size());
			auto lz_state = int{};
			lz_state = Third::lzma::$LzmaCompress(
				unmake_pointer_unsafe<Third::lzma::$Byte>(ripe.current_pointer()),
				&lz_ripe_size,
				unmake_pointer_unsafe<Third::lzma::$Byte>(raw.current_pointer()),
				lz_raw_size,
				unmake_pointer_unsafe<Third::lzma::$Byte>(property.begin()),
				&lz_property_size,
				unmake_box<int>(level),
				0,
				-1,
				-1,
				-1,
				-1,
				-1
			);
			assert_test(lz_state == Third::lzma::$SZ_OK);
			assert_test(lz_property_size == unmake_box<std::size_t>(property.size()));
			raw.forward(make_box<Size>(lz_raw_size));
			ripe.forward(make_box<Size>(lz_ripe_size));
			return;
		}

		// ----------------

		inline static auto process(
			InputByteStreamView &  raw_,
			OutputByteStreamView & ripe_,
			Integer const &        level
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe, level);
		}

	};

}
