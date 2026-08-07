module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dependency/avir/avir.h"

#pragma clang diagnostic pop

export module twinning.kernel.dependency.avir;

export namespace Twinning::Kernel::Dependency::avir {

	using ::avir::CImageResizer;

}
