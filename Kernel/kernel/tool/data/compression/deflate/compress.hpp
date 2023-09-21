#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/data/compression/deflate/common.hpp"
#include "kernel/third/zlib.hpp"

namespace TwinStar::Kernel::Tool::Data::Compression::Deflate {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		static auto process_whole (
			IByteStreamView & raw,
			OByteStreamView & ripe,
			Size const &      level,
			Size const &      window_bits,
			Size const &      memory_level,
			Strategy const &  strategy,
			Wrapper const &   wrapper
		) -> Void {
			#if defined M_compiler_clang
			#pragma clang diagnostic push
			#pragma clang diagnostic ignored "-Wold-style-cast"
			#endif
			assert_test(Math::between(level, 0_sz, mbw<Size>(Third::zlib::Z_BEST_COMPRESSION_)));
			assert_test(Math::between(window_bits, 8_sz, mbw<Size>(Third::zlib::MAX_WBITS_)));
			assert_test(Math::between(memory_level, 1_sz, mbw<Size>(Third::zlib::MAX_MEM_LEVEL_)));
			assert_test(!(window_bits == 8_sz && wrapper != Wrapper::Constant::zlib()));
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
				.next_in = cast_pointer<Third::zlib::Bytef>(as_variable_pointer(raw.current_pointer())).value,
				.avail_in = static_cast<unsigned>(raw.reserve().value),
				.total_in = 0,
				.next_out = cast_pointer<Third::zlib::Bytef>(ripe.current_pointer()).value,
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
			auto state = int{};
			state = Third::zlib::deflateInit2(
				&z_stream,
				static_cast<int>(level.value),
				Third::zlib::Z_DEFLATED_,
				actual_window_bits,
				static_cast<int>(memory_level.value),
				static_cast<int>(strategy.value)
			);
			assert_test(state == Third::zlib::Z_OK_);
			state = Third::zlib::deflate(
				&z_stream,
				Third::zlib::Z_NO_FLUSH_
			);
			assert_test(state == Third::zlib::Z_OK_);
			state = Third::zlib::deflate(
				&z_stream,
				Third::zlib::Z_FINISH_
			);
			assert_test(state == Third::zlib::Z_STREAM_END_);
			state = Third::zlib::deflateEnd(
				&z_stream
			);
			assert_test(state == Third::zlib::Z_OK_);
			assert_test(z_stream.avail_in == 0);
			raw.forward(mbw<Size>(z_stream.total_in));
			ripe.forward(mbw<Size>(z_stream.total_out));
			assert_test(raw.full());
			return;
			#if defined M_compiler_clang
			#pragma clang diagnostic pop
			#endif
		}

		// ----------------

		static auto estimate_whole (
			Size const &    raw_size,
			Size &          ripe_size_bound,
			Size const &    window_bits,
			Size const &    memory_level,
			Wrapper const & wrapper
		) -> Void {
			auto wrap_size = k_none_size;
			switch (wrapper.value) {
				case Wrapper::Constant::none().value : {
					wrap_size = 0_sz;
					break;
				}
				case Wrapper::Constant::zlib().value : {
					wrap_size = 2_sz + 4_sz + 4_sz;
					break;
				}
				case Wrapper::Constant::gzip().value : {
					wrap_size = 10_sz + 8_sz;
					break;
				}
			}
			if (window_bits == 15_sz && memory_level == 8_sz) {
				ripe_size_bound = wrap_size + (raw_size + (raw_size >> 12_sz) + (raw_size >> 14_sz) + (raw_size >> 25_sz) + 13_sz - 6_sz);
			} else {
				ripe_size_bound = wrap_size + (raw_size + ((raw_size + 7_sz) >> 3_sz) + ((raw_size + 63_sz) >> 6_sz) + 5_sz);
			}
			return;
		}

		// ----------------

		static auto process (
			IByteStreamView & raw_,
			OByteStreamView & ripe_,
			Size const &      level,
			Size const &      window_bits,
			Size const &      memory_level,
			Strategy const &  strategy,
			Wrapper const &   wrapper
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe, level, window_bits, memory_level, strategy, wrapper);
		}

		static auto estimate (
			Size const &    raw_size,
			Size &          ripe_size_bound,
			Size const &    window_bits,
			Size const &    memory_level,
			Wrapper const & wrapper
		) -> Void {
			restruct(ripe_size_bound);
			return estimate_whole(raw_size, ripe_size_bound, window_bits, memory_level, wrapper);
		}

	};

}
