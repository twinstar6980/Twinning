module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.lzma2.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.data.compression.lzma2.common;
import twinning.kernel.dependency.lzma;

export namespace Twinning::Kernel::Tool::Data::Compression::Lzma2 {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			OutputBinaryStreamView & raw,
			InputBinaryStreamView &  ripe,
			Container const &        container
		) -> Void {
			throw UnimplementedException{};
		}

		// ----------------

		inline static auto process(
			OutputBinaryStreamView & raw_,
			InputBinaryStreamView &  ripe_,
			Container const &        container
		) -> Void {
			throw UnimplementedException{};
		}

	};

}
