#pragma once

#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "unistd.h"
#include "fcntl.h"
#include "spawn.h"
#include "sys/wait.h"

#if defined M_system_macintosh || defined M_system_iphone
extern char * * environ; // NOTE : not declared in unistd.h of xcode sdk
#endif

#pragma clang diagnostic pop

namespace Twinning::Kernel::Third::posix {
}

#endif
