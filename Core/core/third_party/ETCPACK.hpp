#pragma once

#if defined M_compiler_msvc
#pragma warning(push)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#endif

#include "third_party/ETCPACK/_etcpack.hpp"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace TwinStar::Core::ThirdParty::ETCPACK {

	using uint8 = ::uint8;

	inline constexpr auto compressBlockETC1Exhaustive = ::compressBlockETC1Exhaustive;

	inline constexpr auto decompressBlockETC2 = ::decompressBlockETC2;

}
