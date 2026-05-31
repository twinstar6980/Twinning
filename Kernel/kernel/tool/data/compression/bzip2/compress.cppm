module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.bzip2.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.bzip2.common;
import twinning.kernel.third.bzip2;

export namespace Twinning::Kernel::Tool::Data::Compression::Bzip2 {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputByteStreamView &  raw,
			OutputByteStreamView & ripe,
			Integer const &        block_size,
			Integer const &        work_factor
		) -> Void {
			assert_test(Math::between(block_size, 1_i, 9_i));
			assert_test(Math::between(work_factor, 0_i, 250_i));
			auto bz_state = int{};
			auto bz_stream = Third::bzip2::$bz_stream{
				.next_in = unmake_pointer_unsafe<char>(as_variable_pointer(raw.current_pointer())),
				.avail_in = unmake_box<unsigned int>(raw.reserve()),
				.total_in_lo32 = 0,
				.total_in_hi32 = 0,
				.next_out = unmake_pointer_unsafe<char>(ripe.current_pointer()),
				.avail_out = unmake_box<unsigned int>(ripe.reserve()),
				.total_out_lo32 = 0,
				.total_out_hi32 = 0,
				.state = nullptr,
				.bzalloc = nullptr,
				.bzfree = nullptr,
				.opaque = nullptr,
			};
			bz_state = Third::bzip2::$BZ2_bzCompressInit(
				&bz_stream,
				unmake_box<int>(block_size),
				0,
				unmake_box<int>(work_factor)
			);
			assert_test(bz_state == Third::bzip2::$BZ_OK);
			bz_state = Third::bzip2::$BZ2_bzCompress(
				&bz_stream,
				Third::bzip2::$BZ_FINISH
			);
			assert_test(bz_state == Third::bzip2::$BZ_STREAM_END);
			bz_state = Third::bzip2::$BZ2_bzCompressEnd(
				&bz_stream
			);
			assert_test(bz_state == Third::bzip2::$BZ_OK);
			assert_test(bz_stream.avail_in == 0);
			raw.forward(cast_box<Size>((make_box<IntegerU64>(bz_stream.total_in_hi32) << 32_sz) | (make_box<IntegerU64>(bz_stream.total_in_lo32) << 0_sz)));
			ripe.forward(cast_box<Size>((make_box<IntegerU64>(bz_stream.total_out_hi32) << 32_sz) | (make_box<IntegerU64>(bz_stream.total_out_lo32) << 0_sz)));
			assert_test(raw.full());
			return;
		}

		// ----------------

		inline static auto process(
			InputByteStreamView &  raw_,
			OutputByteStreamView & ripe_,
			Integer const &        block_size,
			Integer const &        work_factor
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe, block_size, work_factor);
		}

	};

}
