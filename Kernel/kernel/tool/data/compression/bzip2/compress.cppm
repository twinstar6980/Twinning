module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.bzip2.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.bzip2.common;
import twinning.kernel.third.bzip2;

export namespace Twinning::Kernel::Tool::Data::Compression::BZip2 {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole (
			IByteStreamView & raw,
			OByteStreamView & ripe,
			Size const &      block_size,
			Size const &      work_factor
		) -> Void {
			assert_test(Math::between(block_size, 1_sz, 9_sz));
			assert_test(Math::between(work_factor, 0_sz, 250_sz));
			auto bz_stream = Third::bzip2::$bz_stream{
				.next_in = cast_pointer<char>(as_variable_pointer(raw.current_pointer())).value,
				.avail_in = static_cast<unsigned int>(raw.reserve().value),
				.total_in_lo32 = 0,
				.total_in_hi32 = 0,
				.next_out = cast_pointer<char>(ripe.current_pointer()).value,
				.avail_out = static_cast<unsigned int>(ripe.reserve().value),
				.total_out_lo32 = 0,
				.total_out_hi32 = 0,
				.state = nullptr,
				.bzalloc = nullptr,
				.bzfree = nullptr,
				.opaque = nullptr,
			};
			auto state = int{};
			state = Third::bzip2::$BZ2_bzCompressInit(
				&bz_stream,
				static_cast<int>(block_size.value),
				0,
				static_cast<int>(work_factor.value)
			);
			assert_test(state == Third::bzip2::$BZ_OK);
			state = Third::bzip2::$BZ2_bzCompress(
				&bz_stream,
				Third::bzip2::$BZ_FINISH
			);
			assert_test(state == Third::bzip2::$BZ_STREAM_END);
			state = Third::bzip2::$BZ2_bzCompressEnd(
				&bz_stream
			);
			assert_test(state == Third::bzip2::$BZ_OK);
			assert_test(bz_stream.avail_in == 0);
			raw.forward(mbox<Size>((static_cast<std::uint64_t>(bz_stream.total_in_hi32) << 32) + static_cast<std::uint64_t>(bz_stream.total_in_lo32)));
			ripe.forward(mbox<Size>((static_cast<std::uint64_t>(bz_stream.total_out_hi32) << 32) + static_cast<std::uint64_t>(bz_stream.total_out_lo32)));
			assert_test(raw.full());
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView & raw_,
			OByteStreamView & ripe_,
			Size const &      block_size,
			Size const &      work_factor
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe, block_size, work_factor);
		}

	};

}
