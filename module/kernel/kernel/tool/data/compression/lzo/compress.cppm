module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.lzo.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.lzo.common;
import twinning.kernel.dependency.lzo;

export namespace Twinning::Kernel::Tool::Data::Compression::Lzo {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputBinaryStreamView &  raw,
			OutputBinaryStreamView & ripe,
			Container const &        container
		) -> Void {
			auto lzo_state = Dependency::lzo::$lzo_init();
			assert_test(lzo_state == Dependency::lzo::$LZO_E_OK);
			auto z_work = Binary{};
			z_work.allocate_full(make_box<Size>(Dependency::lzo::$LZO1X_1_MEM_COMPRESS));
			auto z_raw_size = unmake_box<Dependency::lzo::$lzo_uint>(raw.reserve());
			auto z_ripe_size = unmake_box<Dependency::lzo::$lzo_uint>(ripe.reserve());
			lzo_state = Dependency::lzo::$lzo1x_1_compress(
				unmake_pointer_unsafe<Dependency::lzo::$lzo_byte>(raw.current()),
				z_raw_size,
				unmake_pointer_unsafe<Dependency::lzo::$lzo_byte>(ripe.current()),
				&z_ripe_size,
				unmake_pointer_unsafe<Dependency::lzo::$lzo_byte>(z_work.data())
			);
			assert_test(lzo_state == Dependency::lzo::$LZO_E_OK);
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
			ripe_size_bound = raw_size + raw_size / 16_sz + 64_sz + 3_sz;
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
