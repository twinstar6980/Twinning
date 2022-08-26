#pragma once

#include "core/utility/utility.hpp"
#include "core/third_party/zlib.hpp"

namespace TwinKleS::Core::Tool::Data::Compress::Deflate {

	inline namespace Common {

		namespace Detail {

			#pragma region parameter

			enum class Strategy : ZIntegerU8 {
				default_mode = ThirdParty::zlib::Z_DEFAULT_STRATEGY_,
				filtered = ThirdParty::zlib::Z_FILTERED_,
				huffman_only = ThirdParty::zlib::Z_HUFFMAN_ONLY_,
				rle = ThirdParty::zlib::Z_RLE_,
				fixed = ThirdParty::zlib::Z_FIXED_,
			};

			enum class Wrapper : ZIntegerU8 {
				none,
				zlib,
				gzip,
			};

			#pragma endregion

		}

		using Detail::Strategy;

		using Detail::Wrapper;

	}

	namespace Compress {

		namespace Detail {

			#pragma region using

			using namespace Common::Detail;

			#pragma endregion

			#pragma region compute size

			inline auto compute_size_bound (
				Size const &    raw_size,
				Size const &    window_bits,
				Size const &    memory_level,
				Wrapper const & wrapper
			) -> Size {
				auto wrap_size = k_none_size;
				switch (wrapper) {
					case Wrapper::none : {
						wrap_size = 0_sz;
						break;
					}
					case Wrapper::zlib : {
						wrap_size = 2_sz + 4_sz + 4_sz;
						break;
					}
					case Wrapper::gzip : {
						wrap_size = 10_sz + 8_sz;
						break;
					}
				}
				if (window_bits == 15_sz && memory_level == 8_sz) {
					return wrap_size + (raw_size + (raw_size >> 12_sz) + (raw_size >> 14_sz) + (raw_size >> 25_sz) + 13_sz - 6_sz);
				} else {
					return wrap_size + (raw_size + ((raw_size + 7_sz) >> 3_sz) + ((raw_size + 63_sz) >> 6_sz) + 5_sz);
				}
			}

			#pragma endregion

			#pragma region process

			inline auto process (
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
				assert_condition(!(window_bits == 8_sz && wrapper != Wrapper::zlib));
				auto actual_window_bits = static_cast<int>(window_bits.value);
				switch (wrapper) {
					case Wrapper::none : {
						actual_window_bits = -actual_window_bits;
						break;
					}
					case Wrapper::zlib : {
						actual_window_bits = actual_window_bits;
						break;
					}
					case Wrapper::gzip : {
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
					static_cast<int>(strategy)
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

			#pragma endregion

		}

		using Detail::compute_size_bound;

		using Detail::process;

	}

	namespace Uncompress {

		namespace Detail {

			#pragma region using

			using namespace Common::Detail;

			#pragma endregion

			#pragma region process

			inline auto process (
				IByteStreamView & ripe,
				OByteStreamView & raw,
				Size const &      window_bits,
				Wrapper const &   wrapper
			) -> Void {
				assert_condition(Math::between(window_bits, 8_sz, mbw<Size>(ThirdParty::zlib::MAX_WBITS_)));
				auto actual_window_bits = static_cast<int>(window_bits.value);
				switch (wrapper) {
					case Wrapper::none : {
						actual_window_bits = -actual_window_bits;
						break;
					}
					case Wrapper::zlib : {
						actual_window_bits = actual_window_bits;
						break;
					}
					case Wrapper::gzip : {
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

			#pragma endregion

		}

		using Detail::process;

	}

}

namespace TwinKleS::Core::Trait::Reflection {

	M_reflection_enum(
		M_wrap(Tool::Data::Compress::Deflate::Strategy),
		M_wrap(Strategy),
		M_wrap(default_mode, filtered, huffman_only, rle, fixed),
	);

	M_reflection_enum(
		M_wrap(Tool::Data::Compress::Deflate::Wrapper),
		M_wrap(Wrapper),
		M_wrap(none, zlib, gzip),
	);

}
