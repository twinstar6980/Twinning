#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/data/compression/bzip2/common.hpp"
#include "kernel/third/bzip2.hpp"

namespace TwinStar::Kernel::Tool::Data::Compression::BZip2 {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		static auto process_whole (
			IByteStreamView & ripe,
			OByteStreamView & raw,
			Boolean const &   small
		) -> Void {
			auto bz_stream = Third::bzip2::bz_stream{
				.next_in = cast_pointer<char>(as_variable_pointer(ripe.current_pointer())).value,
				.avail_in = static_cast<unsigned int>(ripe.reserve().value),
				.total_in_lo32 = 0,
				.total_in_hi32 = 0,
				.next_out = cast_pointer<char>(raw.current_pointer()).value,
				.avail_out = static_cast<unsigned int>(raw.reserve().value),
				.total_out_lo32 = 0,
				.total_out_hi32 = 0,
				.state = nullptr,
				.bzalloc = nullptr,
				.bzfree = nullptr,
				.opaque = nullptr,
			};
			auto state = int{};
			state = Third::bzip2::BZ2_bzDecompressInit(
				&bz_stream,
				0,
				static_cast<int>(small.value)
			);
			assert_test(state == Third::bzip2::BZ_OK_);
			state = Third::bzip2::BZ2_bzDecompress(
				&bz_stream
			);
			assert_test(state == Third::bzip2::BZ_STREAM_END_);
			state = Third::bzip2::BZ2_bzDecompressEnd(
				&bz_stream
			);
			assert_test(state == Third::bzip2::BZ_OK_);
			ripe.forward(mbw<Size>((static_cast<uint64_t>(bz_stream.total_in_hi32) << 32) + static_cast<uint64_t>(bz_stream.total_in_lo32)));
			raw.forward(mbw<Size>((static_cast<uint64_t>(bz_stream.total_out_hi32) << 32) + static_cast<uint64_t>(bz_stream.total_out_lo32)));
			return;
		}

		// ----------------

		static auto process (
			IByteStreamView & ripe_,
			OByteStreamView & raw_,
			Boolean const &   small
		) -> Void {
			M_use_zps_of(ripe);
			M_use_zps_of(raw);
			return process_whole(ripe, raw, small);
		}

	};

}
