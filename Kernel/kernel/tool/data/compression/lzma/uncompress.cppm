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

		inline static auto process_whole (
			IByteStreamView & ripe,
			OByteStreamView & raw
		) -> Void {
			auto property = ripe.forward_view(mbox<Size>(Third::lzma::$LZMA_PROPS_SIZE));
			auto raw_size_recorded = ripe.read_of<IntegerU64>();
			assert_test(raw_size_recorded != ~0_iu64);
			assert_test(raw.reserve() >= cbox<Size>(raw_size_recorded));
			auto ripe_size = ripe.reserve().value;
			auto raw_size = cbox<Size>(raw_size_recorded).value;
			auto property_size = property.size().value;
			auto state = Third::lzma::$LzmaUncompress(
				cast_pointer<Third::lzma::$Byte>(raw.current_pointer()).value,
				&raw_size,
				cast_pointer<Third::lzma::$Byte>(ripe.current_pointer()).value,
				&ripe_size,
				cast_pointer<Third::lzma::$Byte>(property.begin()).value,
				property_size
			);
			assert_test(state == Third::lzma::$SZ_OK);
			assert_test(raw_size == cbox<Size>(raw_size_recorded).value);
			ripe.forward(mbox<Size>(ripe_size));
			raw.forward(mbox<Size>(raw_size));
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView & ripe_,
			OByteStreamView & raw_
		) -> Void {
			M_use_zps_of(ripe);
			M_use_zps_of(raw);
			return process_whole(ripe, raw);
		}

	};

}
