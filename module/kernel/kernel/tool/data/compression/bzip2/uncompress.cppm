module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.bzip2.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.bzip2.common;
import twinning.kernel.dependency.bzip2;

export namespace Twinning::Kernel::Tool::Data::Compression::Bzip2 {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			OutputBinaryStreamView & raw,
			InputBinaryStreamView &  ripe,
			Container const &        container,
			Boolean const &          small
		) -> Void {
			auto z_state = int{};
			auto z_stream = Dependency::bzip2::$bz_stream{
				.next_in = unmake_pointer_unsafe<char>(as_variable_pointer(ripe.current())),
				.avail_in = unmake_box<unsigned int>(ripe.reserve()),
				.total_in_lo32 = 0,
				.total_in_hi32 = 0,
				.next_out = unmake_pointer_unsafe<char>(raw.current()),
				.avail_out = unmake_box<unsigned int>(raw.reserve()),
				.total_out_lo32 = 0,
				.total_out_hi32 = 0,
				.state = nullptr,
				.bzalloc = nullptr,
				.bzfree = nullptr,
				.opaque = nullptr,
			};
			z_state = Dependency::bzip2::$BZ2_bzDecompressInit(
				&z_stream,
				0,
				unmake_box<int>(small)
			);
			assert_test(z_state == Dependency::bzip2::$BZ_OK);
			z_state = Dependency::bzip2::$BZ2_bzDecompress(
				&z_stream
			);
			assert_test(z_state == Dependency::bzip2::$BZ_STREAM_END);
			z_state = Dependency::bzip2::$BZ2_bzDecompressEnd(
				&z_stream
			);
			assert_test(z_state == Dependency::bzip2::$BZ_OK);
			ripe.forward(cast_box<Size>(Bitwise::infuse(make_box<IntegerU64>(z_stream.total_in_hi32), 2_ix * 32_sz, 32_sz) | Bitwise::infuse(make_box<IntegerU64>(z_stream.total_in_lo32), 1_ix * 32_sz, 32_sz)));
			raw.forward(cast_box<Size>(Bitwise::infuse(make_box<IntegerU64>(z_stream.total_out_hi32), 2_ix * 32_sz, 32_sz) | Bitwise::infuse(make_box<IntegerU64>(z_stream.total_out_lo32), 1_ix * 32_sz, 32_sz)));
			return;
		}

		// ----------------

		inline static auto process(
			OutputBinaryStreamView & raw_,
			InputBinaryStreamView &  ripe_,
			Container const &        container,
			Boolean const &          small
		) -> Void {
			M_use_split_stream_of(raw);
			M_use_split_stream_of(ripe);
			return process_whole(raw, ripe, container, small);
		}

	};

}
