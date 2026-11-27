module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.lz4.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.lz4.common;
import twinning.kernel.dependency.lz4;

export namespace Twinning::Kernel::Tool::Data::Compression::Lz4 {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputBinaryStreamView &  raw,
			OutputBinaryStreamView & ripe,
			Container const &        container
		) -> Void {
			assert_test(raw.reserve() <= make_box<Size>(Dependency::lz4::$LZ4_MAX_INPUT_SIZE));
			auto z_raw_size = unmake_box<int>(raw.reserve());
			auto z_ripe_size = unmake_box<int>(ripe.reserve());
			auto z_size = Dependency::lz4::$LZ4_compress_default(
				unmake_pointer_unsafe<char>(raw.current()),
				unmake_pointer_unsafe<char>(ripe.current()),
				z_raw_size,
				z_ripe_size
			);
			assert_test(z_size > 0);
			raw.forward(make_box<Size>(z_raw_size));
			ripe.forward(make_box<Size>(z_size));
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size const & raw_size,
			Size &       ripe_size_bound,
			Container const &        container
		) -> Void {
			assert_test(raw_size <= make_box<Size>(Dependency::lz4::$LZ4_MAX_INPUT_SIZE));
			ripe_size_bound = make_box<Size>(Dependency::lz4::$LZ4_compressBound(unmake_box<int>(raw_size)));
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &  raw_,
			OutputBinaryStreamView & ripe_,
			Container const &        container
		) -> Void {
			M_use_split_stream_of(raw);
			M_use_split_stream_of(ripe);
			return process_whole(raw, ripe, container);
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
