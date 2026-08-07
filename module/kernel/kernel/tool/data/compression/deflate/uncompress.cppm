module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.deflate.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.dependency.zlib;

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
			assert_test(Math::between(window_exponent, 8_i, make_box<Integer>(Dependency::zlib::$MAX_WBITS)));
			auto z_state = int{};
			auto z_window_exponent = unmake_box<int>(window_exponent);
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
			auto z_stream = Dependency::zlib::$z_stream{
				.next_in = unmake_pointer_unsafe<Dependency::zlib::$Bytef>(ripe.current_pointer()),
				.avail_in = unmake_box<unsigned>(ripe.reserve()),
				.total_in = 0,
				.next_out = unmake_pointer_unsafe<Dependency::zlib::$Bytef>(raw.current_pointer()),
				.avail_out = unmake_box<unsigned>(raw.reserve()),
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
			z_state = Dependency::zlib::$inflateInit2_(
				&z_stream,
				z_window_exponent,
				Dependency::zlib::$ZLIB_VERSION,
				unmake_box<int>(k_type_size<Dependency::zlib::$z_stream>)
			);
			assert_test(z_state == Dependency::zlib::$Z_OK);
			z_state = Dependency::zlib::$inflate(
				&z_stream,
				Dependency::zlib::$Z_NO_FLUSH
			);
			assert_test(z_state == Dependency::zlib::$Z_STREAM_END);
			z_state = Dependency::zlib::$inflateEnd(
				&z_stream
			);
			assert_test(z_state == Dependency::zlib::$Z_OK);
			ripe.forward(make_box<Size>(z_stream.total_in));
			raw.forward(make_box<Size>(z_stream.total_out));
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
