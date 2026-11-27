module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.snappy.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.snappy.common;
import twinning.kernel.dependency.snappy;

export namespace Twinning::Kernel::Tool::Data::Compression::Snappy {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputBinaryStreamView &  raw,
			OutputBinaryStreamView & ripe,
			Container const &        container
		) -> Void {
			auto z_raw_size = unmake_box<std::size_t>(raw.reserve());
			auto z_ripe_size = unmake_box<std::size_t>(ripe.reserve());
			auto z_state = Dependency::snappy::$snappy_compress(
				unmake_pointer_unsafe<char>(raw.current()),
				z_raw_size,
				unmake_pointer_unsafe<char>(ripe.current()),
				&z_ripe_size
			);
			assert_test(z_state == Dependency::snappy::$SNAPPY_OK);
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
			ripe_size_bound = make_box<Size>(Dependency::snappy::$snappy_max_compressed_length(unmake_box<std::size_t>(raw_size)));
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
