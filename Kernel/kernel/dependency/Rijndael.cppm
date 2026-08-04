module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dependency/Rijndael/Rijndael.h"

#pragma clang diagnostic pop

export module twinning.kernel.dependency.Rijndael;

export namespace Twinning::Kernel::Dependency::Rijndael {

	using ::CRijndael;

}
