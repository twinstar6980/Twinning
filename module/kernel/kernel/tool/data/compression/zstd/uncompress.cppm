module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.zstd.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.zstd.common;
import twinning.kernel.dependency.zstd;

export namespace Twinning::Kernel::Tool::Data::Compression::Zstd {

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
			auto z_raw_size_recorded = Dependency::zstd::$ZSTD_getFrameContentSize(
				unmake_pointer_unsafe<char>(ripe.current()),
				z_ripe_size
			);
			assert_test(z_raw_size_recorded != Dependency::zstd::$ZSTD_CONTENTSIZE_ERROR);
			if (z_raw_size_recorded != Dependency::zstd::$ZSTD_CONTENTSIZE_UNKNOWN) {
				assert_test(raw.reserve() >= make_box<Size>(z_raw_size_recorded));
			}
			auto z_raw_size = Dependency::zstd::$ZSTD_decompress(
				unmake_pointer_unsafe<char>(raw.current()),
				unmake_box<std::size_t>(raw.reserve()),
				unmake_pointer_unsafe<char>(ripe.current()),
				z_ripe_size
			);
			assert_test(Dependency::zstd::$ZSTD_isError(z_raw_size) == 0);
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
