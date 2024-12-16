module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/Rijndael/Rijndael.h"

#pragma clang diagnostic pop

export module twinning.kernel.third.Rijndael;

export namespace Twinning::Kernel::Third::Rijndael {

	using ::CRijndael;

}
