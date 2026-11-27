module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.deflate.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.dependency.zlib;

export namespace Twinning::Kernel::Tool::Data::Compression::Deflate {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputBinaryStreamView &  raw,
			OutputBinaryStreamView & ripe,
			Container const &        container,
			Integer const &          level,
			Integer const &          window_exponent,
			Integer const &          memory_level,
			StrategyMode const &     strategy_mode
		) -> Void {
			assert_test(Math::between(level, 0_i, make_box<Integer>(Dependency::zlib::$Z_BEST_COMPRESSION)));
			assert_test(Math::between(window_exponent, 8_i, make_box<Integer>(Dependency::zlib::$MAX_WBITS)));
			assert_test(Math::between(memory_level, 1_i, make_box<Integer>(Dependency::zlib::$MAX_MEM_LEVEL)));
			assert_test(!(window_exponent == 8_i && container != Container::Constant::zlib()));
			auto z_state = int{};
			auto z_window_exponent = unmake_box<int>(window_exponent);
			switch (container.value) {
				case Container::Constant::none().value: {
					z_window_exponent = -z_window_exponent;
					break;
				}
				case Container::Constant::zlib().value: {
					z_window_exponent = +z_window_exponent;
					break;
				}
				case Container::Constant::gzip().value: {
					z_window_exponent = +z_window_exponent + 16;
					break;
				}
				default: throw UnreachableException{};
			}
			auto z_stream = Dependency::zlib::$z_stream{
				.next_in = unmake_pointer_unsafe<Dependency::zlib::$Bytef>(raw.current()),
				.avail_in = unmake_box<unsigned>(raw.reserve()),
				.total_in = 0,
				.next_out = unmake_pointer_unsafe<Dependency::zlib::$Bytef>(ripe.current()),
				.avail_out = unmake_box<unsigned>(ripe.reserve()),
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
			z_state = Dependency::zlib::$deflateInit2_(
				&z_stream,
				unmake_box<int>(level),
				Dependency::zlib::$Z_DEFLATED,
				z_window_exponent,
				unmake_box<int>(memory_level),
				unmake_box<int>(strategy_mode),
				Dependency::zlib::$ZLIB_VERSION,
				unmake_box<int>(type_byte_size_of<Dependency::zlib::$z_stream>())
			);
			assert_test(z_state == Dependency::zlib::$Z_OK);
			z_state = Dependency::zlib::$deflate(
				&z_stream,
				Dependency::zlib::$Z_NO_FLUSH
			);
			assert_test(z_state == Dependency::zlib::$Z_OK);
			z_state = Dependency::zlib::$deflate(
				&z_stream,
				Dependency::zlib::$Z_FINISH
			);
			assert_test(z_state == Dependency::zlib::$Z_STREAM_END);
			z_state = Dependency::zlib::$deflateEnd(
				&z_stream
			);
			assert_test(z_state == Dependency::zlib::$Z_OK);
			assert_test(z_stream.avail_in == 0);
			raw.forward(make_box<Size>(z_stream.total_in));
			ripe.forward(make_box<Size>(z_stream.total_out));
			assert_test(raw.full());
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size const &        raw_size,
			Size &              ripe_size_bound,
			Container const &        container,
			Integer const &     window_exponent,
			Integer const &     memory_level
		) -> Void {
			auto container_size = 0_sz;
			switch (container.value) {
				case Container::Constant::none().value: {
					container_size = 0_sz;
					break;
				}
				case Container::Constant::zlib().value: {
					container_size = 2_sz + 4_sz + 4_sz;
					break;
				}
				case Container::Constant::gzip().value: {
					container_size = 10_sz + 8_sz;
					break;
				}
				default: throw UnreachableException{};
			}
			if (window_exponent == 15_i && memory_level == 8_i) {
				ripe_size_bound = container_size + (raw_size + (raw_size >> 12_sz) + (raw_size >> 14_sz) + (raw_size >> 25_sz) + 13_sz - 6_sz);
			}
			else {
				ripe_size_bound = container_size + (raw_size + ((raw_size + 7_sz) >> 3_sz) + ((raw_size + 63_sz) >> 6_sz) + 5_sz);
			}
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &  raw_,
			OutputBinaryStreamView & ripe_,
			Container const &        container,
			Integer const &          level,
			Integer const &          window_exponent,
			Integer const &          memory_level,
			StrategyMode const &     strategy_mode
		) -> Void {
			M_use_split_stream_of(raw);
			M_use_split_stream_of(ripe);
			return process_whole(raw, ripe, container, level, window_exponent, memory_level, strategy_mode);
		}

		inline static auto estimate(
			Size const &        raw_size,
			Size &              ripe_size_bound,
			Container const &        container,
			Integer const &     window_exponent,
			Integer const &     memory_level
		) -> Void {
			restruct(ripe_size_bound);
			return estimate_whole(raw_size, ripe_size_bound, container, window_exponent, memory_level);
		}

	};

}
