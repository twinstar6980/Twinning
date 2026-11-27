module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.brotli.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.brotli.common;
import twinning.kernel.dependency.brotli;

export namespace Twinning::Kernel::Tool::Data::Compression::Brotli {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			OutputBinaryStreamView & raw,
			InputBinaryStreamView &  ripe,
			Container const &        container
		) -> Void {
			auto z_ripe_size = unmake_box<std::size_t>(ripe.reserve());
			auto z_raw_size = unmake_box<std::size_t>(raw.reserve());
			auto z_state = Dependency::brotli::$BrotliDecoderDecompress(
				z_ripe_size,
				unmake_pointer_unsafe<Dependency::brotli::$byte>(ripe.current()),
				&z_raw_size,
				unmake_pointer_unsafe<Dependency::brotli::$byte>(raw.current())
			);
			assert_test(z_state == Dependency::brotli::$BROTLI_DECODER_RESULT_SUCCESS);
			ripe.forward(make_box<Size>(z_ripe_size));
			raw.forward(make_box<Size>(z_raw_size));
			return;
		}

		// ----------------

		inline static auto process(
			OutputBinaryStreamView & raw_,
			InputBinaryStreamView &  ripe_,
			Container const &        container
		) -> Void {
			M_use_split_stream_of(raw);
			M_use_split_stream_of(ripe);
			return process_whole(raw, ripe, container);
		}

	};

}
