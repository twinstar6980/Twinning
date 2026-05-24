module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.lzma.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.lzma.common;
import twinning.kernel.third.lzma;

export namespace Twinning::Kernel::Tool::Data::Compression::Lzma {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			OutputByteStreamView & raw,
			InputByteStreamView &  ripe
		) -> Void {
			auto property = ripe.forward_view(mbox<Size>(Third::lzma::$LZMA_PROPS_SIZE));
			auto raw_size_recorded = ripe.read_of<IntegerU64>();
			assert_test(raw_size_recorded != ~0_iu64);
			assert_test(raw.reserve() >= cbox<Size>(raw_size_recorded));
			auto lz_ripe_size = ubox<std::size_t>(ripe.reserve());
			auto lz_raw_size = ubox<std::size_t>(raw_size_recorded);
			auto lz_property_size = ubox<std::size_t>(property.size());
			auto lz_state = int{};
			lz_state = Third::lzma::$LzmaUncompress(
				rubox<Third::lzma::$Byte *>(raw.current_pointer()),
				&lz_raw_size,
				rubox<Third::lzma::$Byte const *>(ripe.current_pointer()),
				&lz_ripe_size,
				rubox<Third::lzma::$Byte const *>(property.begin()),
				lz_property_size
			);
			assert_test(lz_state == Third::lzma::$SZ_OK);
			assert_test(lz_raw_size == ubox<std::size_t>(raw_size_recorded));
			ripe.forward(mbox<Size>(lz_ripe_size));
			raw.forward(mbox<Size>(lz_raw_size));
			return;
		}

		// ----------------

		inline static auto process(
			OutputByteStreamView & raw_,
			InputByteStreamView &  ripe_
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe);
		}

	};

}
