module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait;
export import twinning.kernel.utility.trait.base;
export import twinning.kernel.utility.trait.category;
export import twinning.kernel.utility.trait.template_instance;
export import twinning.kernel.utility.trait.object;
export import twinning.kernel.utility.trait.builtin;
export import twinning.kernel.utility.trait.package.type_package;
export import twinning.kernel.utility.trait.package.value_package;
export import twinning.kernel.utility.trait.reflection.string;
export import twinning.kernel.utility.trait.reflection.field;
export import twinning.kernel.utility.trait.generalization;
export import twinning.kernel.utility.trait.range;
export import twinning.kernel.utility.trait.callable;

// NOTE : USING

export namespace Twinning::Kernel {
	using namespace Trait;
}
