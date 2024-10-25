#pragma once

#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dlfcn.h"

#pragma clang diagnostic pop

namespace Twinning::Shell::Third::posix {
}

#endif
