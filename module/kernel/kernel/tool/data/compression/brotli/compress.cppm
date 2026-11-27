module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.brotli.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.brotli.common;
import twinning.kernel.dependency.brotli;

export namespace Twinning::Kernel::Tool::Data::Compression::Brotli {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputBinaryStreamView &  raw,
			OutputBinaryStreamView & ripe,
			Container const &        container,
			Integer const &          quality,
			Integer const &          window,
			Mode const &             mode
		) -> Void {
			assert_test(Math::between(quality, make_box<Integer>(Dependency::brotli::$BROTLI_MIN_QUALITY), make_box<Integer>(Dependency::brotli::$BROTLI_MAX_QUALITY)));
			assert_test(Math::between(window, make_box<Integer>(Dependency::brotli::$BROTLI_MIN_WINDOW_BITS), make_box<Integer>(Dependency::brotli::$BROTLI_MAX_WINDOW_BITS)));
			auto z_mode = Dependency::brotli::$BrotliEncoderMode{};
			switch (mode.value) {
				case Mode::Constant::generic().value: {
					z_mode = Dependency::brotli::$BROTLI_MODE_GENERIC;
					break;
				}
				case Mode::Constant::text().value: {
					z_mode = Dependency::brotli::$BROTLI_MODE_TEXT;
					break;
				}
				case Mode::Constant::font().value: {
					z_mode = Dependency::brotli::$BROTLI_MODE_FONT;
					break;
				}
				default: throw UnreachableException{};
			}
			auto z_raw_size = unmake_box<std::size_t>(raw.reserve());
			auto z_ripe_size = unmake_box<std::size_t>(ripe.reserve());
			auto z_state = Dependency::brotli::$BrotliEncoderCompress(
				unmake_box<int>(quality),
				unmake_box<int>(window),
				z_mode,
				z_raw_size,
				unmake_pointer_unsafe<Dependency::brotli::$byte>(raw.current()),
				&z_ripe_size,
				unmake_pointer_unsafe<Dependency::brotli::$byte>(ripe.current())
			);
			assert_test(z_state == Dependency::brotli::$BROTLI_TRUE);
			raw.forward(make_box<Size>(z_raw_size));
			ripe.forward(make_box<Size>(z_ripe_size));
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size const & raw_size,
			Size &       ripe_size_bound,
			Container const &        container
		) -> Void {
			auto brotli_ripe_size_bound = Dependency::brotli::$BrotliEncoderMaxCompressedSize(unmake_box<std::size_t>(raw_size));
			assert_test(brotli_ripe_size_bound != 0);
			ripe_size_bound = make_box<Size>(brotli_ripe_size_bound);
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &  raw_,
			OutputBinaryStreamView & ripe_,
			Container const &        container,
			Integer const &          quality,
			Integer const &          window,
			Mode const &             mode
		) -> Void {
			M_use_split_stream_of(raw);
			M_use_split_stream_of(ripe);
			return process_whole(raw, ripe, container,quality, window, mode);
		}

		inline static auto estimate(
			Size const & raw_size,
			Size &       ripe_size_bound,
			Container const &        container
		) -> Void {
			restruct(ripe_size_bound);
			return estimate_whole(raw_size, ripe_size_bound, container);
		}

	};

}
