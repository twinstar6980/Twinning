#pragma once

#if defined M_compiler_msvc
#pragma warning(push)
#pragma warning(disable:4365)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#endif

#include "third_party/Rijndael/Rijndael.h"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace TwinKleS::Core::ThirdParty::Rijndael {

	using CRijndael = ::CRijndael;

}
