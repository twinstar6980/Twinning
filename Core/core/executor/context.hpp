#pragma once

#include "core/utility/utility.hpp"
#include "core/executor/callback.hpp"

namespace TwinStar::Core::Executor {

	#pragma region type

	class Context {

	protected:

		JavaScript::Runtime m_runtime;
		JavaScript::Context m_context;
		Callback            m_callback;
		Boolean             m_busy;
		Optional<Path>      m_module_home;

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
			Callback const & callback
		) :
			m_runtime{JavaScript::Runtime::new_instance()},
			m_context{m_runtime.new_context()},
			m_callback{callback},
			m_busy{k_false} {
			thiz.m_runtime.set_module_loader(thiz.m_module_home);
		}

		Context (
			JavaScript::Runtime & runtime,
			Callback const &      callback
		) :
			m_runtime{JavaScript::Runtime::new_reference(runtime._runtime())},
			m_context{m_runtime.new_context()},
			m_callback{callback},
			m_busy{k_false} {
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
			auto guard = std::lock_guard{JavaScript::g_mutex};
			return thiz.m_context.evaluate(script, name, is_module);
		}

		auto callback (
			List<String> const & argument
		) -> List<String> {
			auto guard = std::lock_guard{JavaScript::g_mutex};
			auto argument_pointer = ZPointer<List<String>>{&as_variable(argument)};
			auto result_pointer = ZPointer<List<String>>{};
			auto exception_pointer = thiz.m_callback(&argument_pointer, &result_pointer);
			if (exception_pointer != nullptr) {
				auto & exception = *exception_pointer;
				throw InvocationException{mss("<callback>"_sv), mss(exception)};
			}
			auto & result = *result_pointer;
			return result;
		}

		#pragma endregion

		#pragma region extension

		auto spawn (
		) -> Context {
			auto guard = std::lock_guard{JavaScript::g_mutex};
			return Context{thiz.m_runtime, thiz.m_callback};
		}

		// ----------------

		auto busy (
		) -> Boolean {
			return thiz.m_busy;
		}

		auto execute (
			Thread &            thread,
			JavaScript::Value & executor
		) -> Void {
			assert_test(!thiz.busy());
			thiz.m_busy = k_true;
			auto guard = std::lock_guard{JavaScript::g_mutex};
			thread.run(
				[&, executor] {
					auto guard = std::lock_guard{JavaScript::g_mutex};
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
