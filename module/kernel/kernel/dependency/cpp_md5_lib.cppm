module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dependency/cpp_md5_lib/include/md5.h"

#pragma clang diagnostic pop

export module twinning.kernel.dependency.cpp_md5_lib;

export namespace Twinning::Kernel::Dependency::cpp_md5_lib {

	using ::MD5;

}
