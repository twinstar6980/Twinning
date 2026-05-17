module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.deflate.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.third.zlib;

export namespace Twinning::Kernel::Tool::Data::Compression::Deflate {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputByteStreamView &  raw,
			OutputByteStreamView & ripe,
			Integer const &        level,
			Integer const &        window_exponent,
			Integer const &        memory_level,
			Strategy const &       strategy,
			Wrapper const &        wrapper
		) -> Void {
			assert_test(Math::between(level, 0_i, mbox<Integer>(Third::zlib::$Z_BEST_COMPRESSION)));
			assert_test(Math::between(window_exponent, 8_i, mbox<Integer>(Third::zlib::$MAX_WBITS)));
			assert_test(Math::between(memory_level, 1_i, mbox<Integer>(Third::zlib::$MAX_MEM_LEVEL)));
			assert_test(!(window_exponent == 8_i && wrapper != Wrapper::Constant::zlib()));
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
				.next_in = cast_pointer<Third::zlib::$Bytef>(as_variable_pointer(raw.current_pointer())).value,
				.avail_in = static_cast<unsigned>(raw.reserve().value),
				.total_in = 0,
				.next_out = cast_pointer<Third::zlib::$Bytef>(ripe.current_pointer()).value,
				.avail_out = static_cast<unsigned>(ripe.reserve().value),
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
			z_state = Third::zlib::$deflateInit2_(
				&z_stream,
				static_cast<int>(level.value),
				Third::zlib::$Z_DEFLATED,
				z_window_exponent,
				static_cast<int>(memory_level.value),
				static_cast<int>(strategy.value),
				Third::zlib::$ZLIB_VERSION,
				static_cast<int>(sizeof(z_stream))
			);
			assert_test(z_state == Third::zlib::$Z_OK);
			z_state = Third::zlib::$deflate(
				&z_stream,
				Third::zlib::$Z_NO_FLUSH
			);
			assert_test(z_state == Third::zlib::$Z_OK);
			z_state = Third::zlib::$deflate(
				&z_stream,
				Third::zlib::$Z_FINISH
			);
			assert_test(z_state == Third::zlib::$Z_STREAM_END);
			z_state = Third::zlib::$deflateEnd(
				&z_stream
			);
			assert_test(z_state == Third::zlib::$Z_OK);
			assert_test(z_stream.avail_in == 0);
			raw.forward(mbox<Size>(z_stream.total_in));
			ripe.forward(mbox<Size>(z_stream.total_out));
			assert_test(raw.full());
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size const &    raw_size,
			Size &          ripe_size_bound,
			Integer const & window_exponent,
			Integer const & memory_level,
			Wrapper const & wrapper
		) -> Void {
			auto wrapper_size = 0_sz;
			switch (wrapper.value) {
				case Wrapper::Constant::none().value: {
					wrapper_size = 0_sz;
					break;
				}
				case Wrapper::Constant::zlib().value: {
					wrapper_size = 2_sz + 4_sz + 4_sz;
					break;
				}
				case Wrapper::Constant::gzip().value: {
					wrapper_size = 10_sz + 8_sz;
					break;
				}
				default: throw UnreachableException{};
			}
			if (window_exponent == 15_i && memory_level == 8_i) {
				ripe_size_bound = wrapper_size + (raw_size + (raw_size >> 12_sz) + (raw_size >> 14_sz) + (raw_size >> 25_sz) + 13_sz - 6_sz);
			}
			else {
				ripe_size_bound = wrapper_size + (raw_size + ((raw_size + 7_sz) >> 3_sz) + ((raw_size + 63_sz) >> 6_sz) + 5_sz);
			}
			return;
		}

		// ----------------

		inline static auto process(
			InputByteStreamView &  raw_,
			OutputByteStreamView & ripe_,
			Integer const &        level,
			Integer const &        window_exponent,
			Integer const &        memory_level,
			Strategy const &       strategy,
			Wrapper const &        wrapper
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe, level, window_exponent, memory_level, strategy, wrapper);
		}

		inline static auto estimate(
			Size const &    raw_size,
			Size &          ripe_size_bound,
			Integer const & window_exponent,
			Integer const & memory_level,
			Wrapper const & wrapper
		) -> Void {
			restruct(ripe_size_bound);
			return estimate_whole(raw_size, ripe_size_bound, window_exponent, memory_level, wrapper);
		}

	};

}
