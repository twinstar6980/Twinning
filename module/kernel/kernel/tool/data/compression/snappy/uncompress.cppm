module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.snappy.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.snappy.common;
import twinning.kernel.dependency.snappy;

export namespace Twinning::Kernel::Tool::Data::Compression::Snappy {

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
			auto z_raw_size = std::size_t{};
			auto z_state = Dependency::snappy::$snappy_uncompressed_length(
				unmake_pointer_unsafe<char>(ripe.current()),
				z_ripe_size,
				&z_raw_size
			);
			assert_test(z_state == Dependency::snappy::$SNAPPY_OK);
			assert_test(raw.reserve() >= make_box<Size>(z_raw_size));
			z_state = Dependency::snappy::$snappy_uncompress(
				unmake_pointer_unsafe<char>(ripe.current()),
				z_ripe_size,
				unmake_pointer_unsafe<char>(raw.current()),
				&z_raw_size
			);
			assert_test(z_state == Dependency::snappy::$SNAPPY_OK);
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
