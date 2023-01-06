#pragma once

#include "core/utility/utility.hpp"
#include "core/third_party/zlib.hpp"

namespace TwinStar::Core::Tool::Data::Compress::Deflate {

	M_enumeration(
		M_wrap(Strategy),
		M_wrap(
			default_mode,
			filtered,
			huffman_only,
			rle,
			fixed,
		),
	);

	M_enumeration(
		M_wrap(Wrapper),
		M_wrap(
			none,
			zlib,
			gzip,
		),
	);

	// ----------------

	struct CompressCommon {

	protected:

	};

	struct Compress :
		CompressCommon {

	protected:

		static auto compute_size_bound (
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

		static auto process_whole (
			IByteStreamView & raw,
			OByteStreamView & ripe,
			Size const &      level,
			Size const &      window_bits,
			Size const &      memory_level,
			Strategy const &  strategy,
			Wrapper const &   wrapper
		) -> Void {
			assert_condition(Math::between(level, 0_sz, mbw<Size>(ThirdParty::zlib::Z_BEST_COMPRESSION_)));
			assert_condition(Math::between(window_bits, 8_sz, mbw<Size>(ThirdParty::zlib::MAX_WBITS_)));
			assert_condition(Math::between(memory_level, 1_sz, mbw<Size>(ThirdParty::zlib::MAX_MEM_LEVEL_)));
			assert_condition(!(window_bits == 8_sz && wrapper != Wrapper::Constant::zlib()));
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
			auto z_stream = ThirdParty::zlib::z_stream{
				.next_in = cast_pointer<ThirdParty::zlib::Bytef>(raw.current_pointer()).value,
				.avail_in = static_cast<unsigned>(raw.reserve().value),
				.total_in = 0,
				.next_out = cast_pointer<ThirdParty::zlib::Bytef>(ripe.current_pointer()).value,
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
			state = ThirdParty::zlib::deflateInit2(
				&z_stream,
				static_cast<int>(level.value),
				ThirdParty::zlib::Z_DEFLATED_,
				actual_window_bits,
				static_cast<int>(memory_level.value),
				static_cast<int>(strategy.value)
			);
			assert_condition(state == ThirdParty::zlib::Z_OK_);
			state = ThirdParty::zlib::deflate(
				&z_stream,
				ThirdParty::zlib::Z_NO_FLUSH_
			);
			assert_condition(state == ThirdParty::zlib::Z_OK_);
			state = ThirdParty::zlib::deflate(
				&z_stream,
				ThirdParty::zlib::Z_FINISH_
			);
			assert_condition(state == ThirdParty::zlib::Z_STREAM_END_);
			state = ThirdParty::zlib::deflateEnd(
				&z_stream
			);
			assert_condition(state == ThirdParty::zlib::Z_OK_);
			assert_condition(z_stream.avail_in == 0);
			raw.forward(mbw<Size>(z_stream.total_in));
			ripe.forward(mbw<Size>(z_stream.total_out));
			assert_condition(raw.full());
			return;
		}

	public:

		static auto do_compute_size_bound (
			Size const &    raw_size,
			Size &          ripe_size_bound,
			Size const &    window_bits,
			Size const &    memory_level,
			Wrapper const & wrapper
		) -> Void {
			restruct(ripe_size_bound);
			return compute_size_bound(raw_size, ripe_size_bound, window_bits, memory_level, wrapper);
		}

		// ----------------

		static auto do_process_whole (
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

	};

	struct Uncompress :
		CompressCommon {

	protected:

		static auto process_whole (
			IByteStreamView & ripe,
			OByteStreamView & raw,
			Size const &      window_bits,
			Wrapper const &   wrapper
		) -> Void {
			assert_condition(Math::between(window_bits, 8_sz, mbw<Size>(ThirdParty::zlib::MAX_WBITS_)));
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
			auto z_stream = ThirdParty::zlib::z_stream{
				.next_in = cast_pointer<ThirdParty::zlib::Bytef>(ripe.current_pointer()).value,
				.avail_in = static_cast<unsigned>(ripe.reserve().value),
				.total_in = 0,
				.next_out = cast_pointer<ThirdParty::zlib::Bytef>(raw.current_pointer()).value,
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
			state = ThirdParty::zlib::inflateInit2(
				&z_stream,
				actual_window_bits
			);
			assert_condition(state == ThirdParty::zlib::Z_OK_);
			state = ThirdParty::zlib::inflate(
				&z_stream,
				ThirdParty::zlib::Z_NO_FLUSH_
			);
			assert_condition(state == ThirdParty::zlib::Z_STREAM_END_);
			state = ThirdParty::zlib::inflateEnd(
				&z_stream
			);
			assert_condition(state == ThirdParty::zlib::Z_OK_);
			ripe.forward(mbw<Size>(z_stream.total_in));
			raw.forward(mbw<Size>(z_stream.total_out));
			return;
		}

	public:

		static auto do_process_whole (
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
