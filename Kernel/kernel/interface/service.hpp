#pragma once

#include "kernel/interface/data.hpp"
#include "kernel/interface/proxy.hpp"
#include "kernel/interface/implement.hpp"

namespace TwinStar::Kernel::Interface {

	struct Service {

		Executor * executor{};

		// ----------------

		~Service (
		) {
			ExecutorProxy::destruct(*thiz.executor);
			delete thiz.executor;
		}

		Service (
		) {
			thiz.executor = new Executor{};
			ExecutorProxy::construct(*thiz.executor, ExecutorProxy{&Implement::executor});
		}

	};

	// ----------------

	#if defined M_compiler_msvc
	#pragma warning(push)
	#endif
	#if defined M_compiler_clang
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wextern-initializer"
	#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
	#endif

	#if defined M_compiler_msvc
	__declspec(dllexport)
	#endif
	#if defined M_compiler_clang
	__attribute__((visibility("default")))
	#endif
	extern Service service = Service{};

	#if defined M_compiler_msvc
	#pragma warning(pop)
	#endif
	#if defined M_compiler_clang
	#pragma clang diagnostic pop
	#endif

}
