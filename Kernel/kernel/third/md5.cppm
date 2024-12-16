module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/md5/md5.h"

#pragma clang diagnostic pop

export module twinning.kernel.third.md5;

export namespace Twinning::Kernel::Third::md5 {

	using ::byte;

	using ::MD5;

}
