module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dependency/tinyxml2/tinyxml2.h"

#pragma clang diagnostic pop

export module twinning.kernel.dependency.tinyxml2;

export namespace Twinning::Kernel::Dependency::tinyxml2 {

	using ::tinyxml2::XMLNode;

	using ::tinyxml2::XMLDocument;

	using ::tinyxml2::XMLPrinter;

}
