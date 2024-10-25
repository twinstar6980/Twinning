#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/data/compression/deflate/common.hpp"
#include "kernel/third/zlib.hpp"

namespace Twinning::Kernel::Tool::Data::Compression::Deflate {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		static auto process_whole (
			IByteStreamView & ripe,
			OByteStreamView & raw,
			Size const &      window_bits,
			Wrapper const &   wrapper
		) -> Void {
			#pragma clang diagnostic push
			#pragma clang diagnostic ignored "-Wold-style-cast"
			assert_test(Math::between(window_bits, 8_sz, mbw<Size>(Third::zlib::MAX_WBITS_)));
			auto actual_window_bits = static_cast<int>(window_bits.value);
			switch (wrapper.value) {
				case Wrapper::Constant::none().value : {
					actual_window_bits = -actual_window_bits;
					break;
				}
				case Wrapper::Constant::zlib().value : {
					actual_window_bits = actual_window_bits;
					break;
				}
				case Wrapper::Constant::gzip().value : {
					actual_window_bits = actual_window_bits + 16;
					break;
				}
			}
			auto z_stream = Third::zlib::z_stream{
				.next_in = cast_pointer<Third::zlib::Bytef>(as_variable_pointer(ripe.current_pointer())).value,
				.avail_in = static_cast<unsigned>(ripe.reserve().value),
				.total_in = 0,
				.next_out = cast_pointer<Third::zlib::Bytef>(raw.current_pointer()).value,
				.avail_out = static_cast<unsigned>(raw.reserve().value),
				.total_out = 0,
				.msg = nullptr,
				.state = nullptr,
				.zalloc = nullptr,
				.zfree = nullptr,
				.opaque = nullptr,
				.data_type = 0,
				.adler = 0,
				.reserved = 0,
			};
			auto state = int{};
			state = Third::zlib::inflateInit2(
				&z_stream,
				actual_window_bits
			);
			assert_test(state == Third::zlib::Z_OK_);
			state = Third::zlib::inflate(
				&z_stream,
				Third::zlib::Z_NO_FLUSH_
			);
			assert_test(state == Third::zlib::Z_STREAM_END_);
			state = Third::zlib::inflateEnd(
				&z_stream
			);
			assert_test(state == Third::zlib::Z_OK_);
			ripe.forward(mbw<Size>(z_stream.total_in));
			raw.forward(mbw<Size>(z_stream.total_out));
			return;
			#pragma clang diagnostic pop
		}

		// ----------------

		static auto process (
			IByteStreamView & ripe_,
			OByteStreamView & raw_,
			Size const &      window_bits,
			Wrapper const &   wrapper
		) -> Void {
			M_use_zps_of(ripe);
			M_use_zps_of(raw);
			return process_whole(ripe, raw, window_bits, wrapper);
		}

	};

}
