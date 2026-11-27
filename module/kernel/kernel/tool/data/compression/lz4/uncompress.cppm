module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.lz4.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.lz4.common;
import twinning.kernel.dependency.lz4;

export namespace Twinning::Kernel::Tool::Data::Compression::Lz4 {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			OutputBinaryStreamView & raw,
			InputBinaryStreamView &  ripe,
			Container const &        container
		) -> Void {
			assert_test(ripe.reserve() <= make_box<Size>(Dependency::lz4::$LZ4_MAX_INPUT_SIZE));
			auto z_ripe_size = unmake_box<int>(ripe.reserve());
			auto z_raw_size = unmake_box<int>(raw.reserve());
			auto z_size = Dependency::lz4::$LZ4_decompress_safe(
				unmake_pointer_unsafe<char>(ripe.current()),
				unmake_pointer_unsafe<char>(raw.current()),
				z_ripe_size,
				z_raw_size
			);
			assert_test(z_size >= 0);
			ripe.forward(make_box<Size>(z_ripe_size));
			raw.forward(make_box<Size>(z_size));
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
