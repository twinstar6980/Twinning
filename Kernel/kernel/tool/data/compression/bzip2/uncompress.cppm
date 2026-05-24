module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.bzip2.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.bzip2.common;
import twinning.kernel.third.bzip2;

export namespace Twinning::Kernel::Tool::Data::Compression::Bzip2 {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			OutputByteStreamView & raw,
			InputByteStreamView &  ripe,
			Boolean const &        small
		) -> Void {
			auto bz_state = int{};
			auto bz_stream = Third::bzip2::$bz_stream{
				.next_in = rubox<char *>(as_variable_pointer(ripe.current_pointer())),
				.avail_in = ubox<unsigned int>(ripe.reserve()),
				.total_in_lo32 = 0,
				.total_in_hi32 = 0,
				.next_out = rubox<char *>(raw.current_pointer()),
				.avail_out = ubox<unsigned int>(raw.reserve()),
				.total_out_lo32 = 0,
				.total_out_hi32 = 0,
				.state = nullptr,
				.bzalloc = nullptr,
				.bzfree = nullptr,
				.opaque = nullptr,
			};
			bz_state = Third::bzip2::$BZ2_bzDecompressInit(
				&bz_stream,
				0,
				ubox<int>(small)
			);
			assert_test(bz_state == Third::bzip2::$BZ_OK);
			bz_state = Third::bzip2::$BZ2_bzDecompress(
				&bz_stream
			);
			assert_test(bz_state == Third::bzip2::$BZ_STREAM_END);
			bz_state = Third::bzip2::$BZ2_bzDecompressEnd(
				&bz_stream
			);
			assert_test(bz_state == Third::bzip2::$BZ_OK);
			ripe.forward(mbox<Size>((static_cast<std::uint64_t>(bz_stream.total_in_hi32) << 32) + static_cast<std::uint64_t>(bz_stream.total_in_lo32)));
			raw.forward(mbox<Size>((static_cast<std::uint64_t>(bz_stream.total_out_hi32) << 32) + static_cast<std::uint64_t>(bz_stream.total_out_lo32)));
			return;
		}

		// ----------------

		inline static auto process(
			OutputByteStreamView & raw_,
			InputByteStreamView &  ripe_,
			Boolean const &        small
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe, small);
		}

	};

}
