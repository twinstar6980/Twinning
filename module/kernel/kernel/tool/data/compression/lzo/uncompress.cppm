module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.lzo.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.lzo.common;
import twinning.kernel.dependency.lzo;

export namespace Twinning::Kernel::Tool::Data::Compression::Lzo {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			OutputBinaryStreamView & raw,
			InputBinaryStreamView &  ripe,
			Container const &        container
		) -> Void {
			auto z_state = Dependency::lzo::$lzo_init();
			assert_test(z_state == Dependency::lzo::$LZO_E_OK);
			auto z_ripe_size = unmake_box<Dependency::lzo::$lzo_uint>(ripe.reserve());
			auto z_raw_size = unmake_box<Dependency::lzo::$lzo_uint>(raw.reserve());
			z_state = Dependency::lzo::$lzo1x_decompress_safe(
				unmake_pointer_unsafe<Dependency::lzo::$lzo_byte>(ripe.current()),
				z_ripe_size,
				unmake_pointer_unsafe<Dependency::lzo::$lzo_byte>(raw.current()),
				&z_raw_size,
				nullptr
			);
			assert_test(z_state == Dependency::lzo::$LZO_E_OK);
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
