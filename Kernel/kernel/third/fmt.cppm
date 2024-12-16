module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/fmt/include/fmt/format.h"

#pragma clang diagnostic pop

export module twinning.kernel.third.fmt;

export namespace Twinning::Kernel::Third::fmt {

	using ::fmt::formatter;

	using ::fmt::format;

	using ::fmt::runtime;

}
