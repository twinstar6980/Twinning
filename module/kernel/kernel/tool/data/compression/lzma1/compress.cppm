module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.lzma1.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.lzma1.common;
import twinning.kernel.dependency.lzma;

export namespace Twinning::Kernel::Tool::Data::Compression::Lzma1 {

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
			assert_test(Math::between(level, 0_i, 9_i));
			auto property = ripe.split_forward(make_box<Size>(Dependency::lzma::$LZMA_PROPS_SIZE)).view();
			ripe.write(cast_box<IntegerU64>(raw.reserve()));
			auto z_raw_size = unmake_box<std::size_t>(raw.reserve());
			auto z_ripe_size = unmake_box<std::size_t>(ripe.reserve());
			auto z_property_size = unmake_box<std::size_t>(property.size());
			auto z_state = int{};
			z_state = Dependency::lzma::$LzmaCompress(
				unmake_pointer_unsafe<Dependency::lzma::$Byte>(ripe.current()),
				&z_ripe_size,
				unmake_pointer_unsafe<Dependency::lzma::$Byte>(raw.current()),
				z_raw_size,
				unmake_pointer_unsafe<Dependency::lzma::$Byte>(property.data()),
				&z_property_size,
				unmake_box<int>(level),
				0,
				-1,
				-1,
				-1,
				-1,
				-1
			);
			assert_test(z_state == Dependency::lzma::$SZ_OK);
			assert_test(z_property_size == unmake_box<std::size_t>(property.size()));
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
			ripe_size_bound = 5_sz + cast_box<Size>(Math::ceil(cast_box<Floater>(raw_size) * 1.1_f)) + 1024_sz * 16_sz;
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
