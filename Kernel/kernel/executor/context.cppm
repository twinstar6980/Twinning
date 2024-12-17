module;

#include "kernel/common.hpp"

export module twinning.kernel.executor.context;
import twinning.kernel.utility;
import twinning.kernel.interface.proxy;

export namespace Twinning::Kernel::Executor {

	#pragma region type

	class Context {

	protected:

		JavaScript::Runtime m_runtime;

		JavaScript::Context m_context;

		Interface::ExecutorProxy m_callback;

		Boolean m_busy;

		Optional<Path> m_module_home;

	public:

		#pragma region structor

		~Context (
		) = default;

		// ----------------

		Context (
		) = delete;

		Context (
			Context const & that
		) = delete;

		Context (
			Context && that
		) = default;

		// ----------------

		Context (
			Interface::ExecutorProxy const & callback
		) :
			m_runtime{JavaScript::Runtime::new_instance()},
			m_context{m_runtime.new_context()},
			m_callback{callback},
			m_busy{k_false},
			m_module_home{k_null_optional} {
			thiz.m_runtime.enable_module_loader(thiz.m_module_home);
		}

		Context (
			JavaScript::Runtime &            runtime,
			Interface::ExecutorProxy const & callback
		) :
			m_runtime{JavaScript::Runtime::new_reference(runtime._runtime())},
			m_context{m_runtime.new_context()},
			m_callback{callback},
			m_busy{k_false},
			m_module_home{k_null_optional} {
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			Context const & that
		) -> Context & = delete;

		auto operator = (
			Context && that
		) -> Context & = default;

		#pragma endregion

		#pragma region runtime & context

		auto runtime (
		) -> JavaScript::Runtime & {
			return thiz.m_runtime;
		}

		auto context (
		) -> JavaScript::Context & {
			return thiz.m_context;
		}

		#pragma endregion

		#pragma region basic

		auto evaluate (
			CStringView const & script,
			String const &      name,
			Boolean const &     is_module
		) -> JavaScript::Value {
			auto locker = Thread::Locker{JavaScript::g_mutex};
			return thiz.m_context.evaluate(script, name, is_module);
		}

		auto callback (
			List<String> const & argument
		) -> List<String> {
			auto result = List<String>{};
			#if defined M_build_release
			try
			#endif
			{
				thiz.m_callback.value(
					Interface::ExecutorProxy{},
					self_cast<Interface::MessageProxy>(argument),
					self_cast<Interface::MessageProxy>(result)
				);
			}
			#if defined M_build_release
			catch (String & exception) {
				throw InvocationException{mss("<callback>"_sv), mss(exception)};
			}
			#endif
			return result;
		}

		#pragma endregion

		#pragma region extension

		auto spawn (
		) -> Context {
			auto locker = Thread::Locker{JavaScript::g_mutex};
			return Context{thiz.m_runtime, thiz.m_callback};
		}

		// ----------------

		auto busy (
		) -> Boolean {
			return thiz.m_busy;
		}

		auto execute (
			Thread::Thread &    thread,
			JavaScript::Value & executor
		) -> Void {
			assert_test(!thiz.busy());
			thiz.m_busy = k_true;
			auto locker = Thread::Locker{JavaScript::g_mutex};
			thread.run(
				[&, executor] {
					auto locker = Thread::Locker{JavaScript::g_mutex};
					JavaScript::Value::new_reference(thiz.m_context._context(), as_variable(executor)._value()).call(make_list<JavaScript::Value>());
					as_variable(executor).set_undefined();
					thiz.m_busy = k_false;
					return;
				}
			);
			return;
		}

		#pragma endregion

		#pragma region environment

		auto query_module_home (
		) -> Optional<Path> & {
			return thiz.m_module_home;
		}

		// ----------------

		auto query_byte_stream_use_big_endian (
		) -> Boolean & {
			return g_byte_stream_use_big_endian;
		}

		#pragma endregion

	};

	#pragma endregion

}
