module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.deflate.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.third.zlib;

export namespace Twinning::Kernel::Tool::Data::Compression::Deflate {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputByteStreamView &  ripe,
			OutputByteStreamView & raw,
			Integer const &        window_exponent,
			Wrapper const &        wrapper
		) -> Void {
			assert_test(Math::between(window_exponent, 8_i, mbox<Integer>(Third::zlib::$MAX_WBITS)));
			auto z_state = int{};
			auto z_window_exponent = static_cast<int>(window_exponent.value);
			switch (wrapper.value) {
				case Wrapper::Constant::none().value: {
					z_window_exponent = -z_window_exponent;
					break;
				}
				case Wrapper::Constant::zlib().value: {
					z_window_exponent = +z_window_exponent;
					break;
				}
				case Wrapper::Constant::gzip().value: {
					z_window_exponent = +z_window_exponent + 16;
					break;
				}
				default: throw UnreachableException{};
			}
			auto z_stream = Third::zlib::$z_stream{
				.next_in = cast_pointer<Third::zlib::$Bytef>(as_variable_pointer(ripe.current_pointer())).value,
				.avail_in = static_cast<unsigned>(ripe.reserve().value),
				.total_in = 0,
				.next_out = cast_pointer<Third::zlib::$Bytef>(raw.current_pointer()).value,
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
			z_state = Third::zlib::$inflateInit2_(
				&z_stream,
				z_window_exponent,
				Third::zlib::$ZLIB_VERSION,
				static_cast<int>(sizeof(z_stream))
			);
			assert_test(z_state == Third::zlib::$Z_OK);
			z_state = Third::zlib::$inflate(
				&z_stream,
				Third::zlib::$Z_NO_FLUSH
			);
			assert_test(z_state == Third::zlib::$Z_STREAM_END);
			z_state = Third::zlib::$inflateEnd(
				&z_stream
			);
			assert_test(z_state == Third::zlib::$Z_OK);
			ripe.forward(mbox<Size>(z_stream.total_in));
			raw.forward(mbox<Size>(z_stream.total_out));
			return;
		}

		// ----------------

		inline static auto process(
			InputByteStreamView &  ripe_,
			OutputByteStreamView & raw_,
			Integer const &        window_exponent,
			Wrapper const &        wrapper
		) -> Void {
			M_use_zps_of(ripe);
			M_use_zps_of(raw);
			return process_whole(ripe, raw, window_exponent, wrapper);
		}

	};

}
