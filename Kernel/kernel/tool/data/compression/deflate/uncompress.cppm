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
			OutputByteStreamView & raw,
			InputByteStreamView &  ripe,
			Integer const &        window_exponent,
			WrapperType const &    wrapper_type
		) -> Void {
			assert_test(Math::between(window_exponent, 8_i, mbox<Integer>(Third::zlib::$MAX_WBITS)));
			auto z_state = int{};
			auto z_window_exponent = ubox<int>(window_exponent);
			switch (wrapper_type.value) {
				case WrapperType::Constant::none().value: {
					z_window_exponent = -z_window_exponent;
					break;
				}
				case WrapperType::Constant::zlib().value: {
					z_window_exponent = +z_window_exponent;
					break;
				}
				case WrapperType::Constant::gzip().value: {
					z_window_exponent = +z_window_exponent + 16;
					break;
				}
				default: throw UnreachableException{};
			}
			auto z_stream = Third::zlib::$z_stream{
				.next_in = rubox<Third::zlib::$Bytef const *>(ripe.current_pointer()),
				.avail_in = ubox<unsigned>(ripe.reserve()),
				.total_in = 0,
				.next_out = rubox<Third::zlib::$Bytef *>(raw.current_pointer()),
				.avail_out = ubox<unsigned>(raw.reserve()),
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
				ubox<int>(k_type_size<Third::zlib::$z_stream>)
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
			OutputByteStreamView & raw_,
			InputByteStreamView &  ripe_,
			Integer const &        window_exponent,
			WrapperType const &    wrapper_type
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe, window_exponent, wrapper_type);
		}

	};

}
