module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.lzma2.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.lzma2.common;
import twinning.kernel.dependency.lzma;

export namespace Twinning::Kernel::Tool::Data::Compression::Lzma2 {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputBinaryStreamView &  raw,
			OutputBinaryStreamView & ripe,
			Container const &        container,
			Integer const &          level
		) -> Void {
			throw UnimplementedException{};
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size const & raw_size,
			Size &       ripe_size_bound,
			Container const &        container
		) -> Void {
			throw UnimplementedException{};
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &  raw_,
			OutputBinaryStreamView & ripe_,
			Container const &        container,
			Integer const &          level
		) -> Void {
			M_use_split_stream_of(raw);
			M_use_split_stream_of(ripe);
			return process_whole(raw, ripe, container, level);
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
