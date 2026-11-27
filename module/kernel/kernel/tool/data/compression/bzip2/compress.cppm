module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.bzip2.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.bzip2.common;
import twinning.kernel.dependency.bzip2;

export namespace Twinning::Kernel::Tool::Data::Compression::Bzip2 {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputBinaryStreamView &  raw,
			OutputBinaryStreamView & ripe,
			Container const &        container,
			Integer const &          block_size,
			Integer const &          work_factor
		) -> Void {
			assert_test(Math::between(block_size, 1_i, 9_i));
			assert_test(Math::between(work_factor, 0_i, 250_i));
			auto z_state = int{};
			auto z_stream = Dependency::bzip2::$bz_stream{
				.next_in = unmake_pointer_unsafe<char>(as_variable_pointer(raw.current())),
				.avail_in = unmake_box<unsigned int>(raw.reserve()),
				.total_in_lo32 = 0,
				.total_in_hi32 = 0,
				.next_out = unmake_pointer_unsafe<char>(ripe.current()),
				.avail_out = unmake_box<unsigned int>(ripe.reserve()),
				.total_out_lo32 = 0,
				.total_out_hi32 = 0,
				.state = nullptr,
				.bzalloc = nullptr,
				.bzfree = nullptr,
				.opaque = nullptr,
			};
			z_state = Dependency::bzip2::$BZ2_bzCompressInit(
				&z_stream,
				unmake_box<int>(block_size),
				0,
				unmake_box<int>(work_factor)
			);
			assert_test(z_state == Dependency::bzip2::$BZ_OK);
			z_state = Dependency::bzip2::$BZ2_bzCompress(
				&z_stream,
				Dependency::bzip2::$BZ_FINISH
			);
			assert_test(z_state == Dependency::bzip2::$BZ_STREAM_END);
			z_state = Dependency::bzip2::$BZ2_bzCompressEnd(
				&z_stream
			);
			assert_test(z_state == Dependency::bzip2::$BZ_OK);
			assert_test(z_stream.avail_in == 0);
			raw.forward(cast_box<Size>(Bitwise::infuse(make_box<IntegerU64>(z_stream.total_in_hi32), 2_ix * 32_sz, 32_sz) | Bitwise::infuse(make_box<IntegerU64>(z_stream.total_in_lo32), 1_ix * 32_sz, 32_sz)));
			ripe.forward(cast_box<Size>(Bitwise::infuse(make_box<IntegerU64>(z_stream.total_out_hi32), 2_ix * 32_sz, 32_sz) | Bitwise::infuse(make_box<IntegerU64>(z_stream.total_out_lo32), 1_ix * 32_sz, 32_sz)));
			assert_test(raw.full());
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size const & raw_size,
			Size &       ripe_size_bound,
			Container const &        container
		) -> Void {
			ripe_size_bound = raw_size + Math::compute_padded_unit_count(raw_size, 100_sz) + 600_sz;
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &  raw_,
			OutputBinaryStreamView & ripe_,
			Container const &        container,
			Integer const &          block_size,
			Integer const &          work_factor
		) -> Void {
			M_use_split_stream_of(raw);
			M_use_split_stream_of(ripe);
			return process_whole(raw, ripe, container, block_size, work_factor);
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
