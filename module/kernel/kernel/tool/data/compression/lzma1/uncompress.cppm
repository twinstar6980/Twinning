module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.lzma1.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.lzma1.common;
import twinning.kernel.dependency.lzma;

export namespace Twinning::Kernel::Tool::Data::Compression::Lzma1 {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			OutputBinaryStreamView & raw,
			InputBinaryStreamView &  ripe,
			Container const &        container
		) -> Void {
			auto property = ripe.split_forward(make_box<Size>(Dependency::lzma::$LZMA_PROPS_SIZE)).view();
			auto raw_size_recorded = ripe.read_of<IntegerU64>();
			assert_test(raw_size_recorded != ~0_iu64);
			assert_test(raw.reserve() >= cast_box<Size>(raw_size_recorded));
			auto z_ripe_size = unmake_box<std::size_t>(ripe.reserve());
			auto z_raw_size = unmake_box<std::size_t>(raw_size_recorded);
			auto z_property_size = unmake_box<std::size_t>(property.size());
			auto z_state = int{};
			z_state = Dependency::lzma::$LzmaUncompress(
				unmake_pointer_unsafe<Dependency::lzma::$Byte>(raw.current()),
				&z_raw_size,
				unmake_pointer_unsafe<Dependency::lzma::$Byte>(ripe.current()),
				&z_ripe_size,
				unmake_pointer_unsafe<Dependency::lzma::$Byte>(property.data()),
				z_property_size
			);
			assert_test(z_state == Dependency::lzma::$SZ_OK);
			assert_test(z_raw_size == unmake_box<std::size_t>(raw_size_recorded));
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
