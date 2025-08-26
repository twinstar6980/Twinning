module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/cpp_md5_lib/include/md5.h"

#pragma clang diagnostic pop

export module twinning.kernel.third.cpp_md5_lib;

export namespace Twinning::Kernel::Third::cpp_md5_lib {

	using ::MD5;

}
