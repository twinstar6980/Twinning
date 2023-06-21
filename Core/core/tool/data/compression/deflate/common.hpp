#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Tool::Data::Compression::Deflate {

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

	struct Common {

	};

}
