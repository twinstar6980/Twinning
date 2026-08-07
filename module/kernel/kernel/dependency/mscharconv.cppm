module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dependency/mscharconv/include/msstl/charconv.hpp"

#pragma clang diagnostic pop

export module twinning.kernel.dependency.mscharconv;

export namespace Twinning::Kernel::Dependency::mscharconv {

	using ::msstl::chars_format;

	using ::msstl::from_chars_result;

	using ::msstl::to_chars;

	using ::msstl::from_chars;

}
