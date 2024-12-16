module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/mscharconv/include/msstl/charconv.hpp"

#pragma clang diagnostic pop

export module twinning.kernel.third.mscharconv;

export namespace Twinning::Kernel::Third::mscharconv {

	using ::msstl::chars_format;

	using ::msstl::from_chars_result;

	using ::msstl::to_chars;

	using ::msstl::from_chars;

}
