module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.zstd.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.zstd.common;
import twinning.kernel.dependency.zstd;

export namespace Twinning::Kernel::Tool::Data::Compression::Zstd {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputBinaryStreamView &  raw,
			OutputBinaryStreamView & ripe,
			Container const &        container,
			Integer const &          level
		) -> Void {
			assert_test(Math::between(level, make_box<Integer>(Dependency::zstd::$ZSTD_minCLevel()), make_box<Integer>(Dependency::zstd::$ZSTD_maxCLevel())));
			auto z_raw_size = unmake_box<std::size_t>(raw.reserve());
			auto z_ripe_size = Dependency::zstd::$ZSTD_compress(
				unmake_pointer_unsafe<char>(ripe.current()),
				unmake_box<std::size_t>(ripe.reserve()),
				unmake_pointer_unsafe<char>(raw.current()),
				z_raw_size,
				unmake_box<int>(level)
			);
			assert_test(Dependency::zstd::$ZSTD_isError(z_ripe_size) == 0);
			raw.forward(make_box<Size>(z_raw_size));
			ripe.forward(make_box<Size>(z_ripe_size));
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size const & raw_size,
			Size &       ripe_size_bound,
			Container const &        container
		) -> Void {
			ripe_size_bound = make_box<Size>(Dependency::zstd::$ZSTD_compressBound(unmake_box<std::size_t>(raw_size)));
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &  raw_,
			OutputBinaryStreamView & ripe_,
			Container const &        container,
			Integer const &          level
		) -> Void {
			M_use_split_stream_of(raw);
			M_use_split_stream_of(ripe);
			return process_whole(raw, ripe, container, level);
		}

		inline static auto estimate(
			Size const & raw_size,
			Size &       ripe_size_bound,
			Container const &        container
		) -> Void {
			restruct(ripe_size_bound);
			return estimate_whole(raw_size, ripe_size_bound, container);
		}

	};

}
