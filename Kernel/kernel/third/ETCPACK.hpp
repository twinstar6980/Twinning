#pragma once

#if defined M_compiler_msvc
#pragma warning(push)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#endif

#include "third/ETCPACK/_etcpack.hpp"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace TwinStar::Kernel::Third::ETCPACK {

	using uint8 = ::uint8;

	inline constexpr auto decompressBlockETC2c = ::decompressBlockETC2c;

	inline constexpr auto decompressBlockAlphaC = ::decompressBlockAlphaC;

}
