#pragma once

#include "core/utility/utility.hpp"
#include "core/executor/shell_callback.hpp"

namespace TwinStar::Core::Executor {

	#pragma region type

	class Context {

	protected:

		JS::Runtime   m_runtime;
		JS::Context   m_context;
		ShellCallback m_shell_callback;
		Boolean       m_busy;

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
			ShellCallback const & shell_callback
		) :
			m_runtime{JS::Runtime::new_instance()},
			m_context{m_runtime.new_context()},
			m_shell_callback{shell_callback},
			m_busy{k_false} {
		}

		Context (
			JS::Runtime &         runtime,
			ShellCallback const & shell_callback
		) :
			m_runtime{JS::Runtime::new_reference(runtime._runtime())},
			m_context{m_runtime.new_context()},
			m_shell_callback{shell_callback},
			m_busy{k_false} {
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			Context const & that
		) -> Context& = delete;

		auto operator = (
			Context && that
		) -> Context& = default;

		#pragma endregion

		#pragma region runtime & context

		auto runtime (
		) -> JS::Runtime& {
			return thiz.m_runtime;
		}

		auto context (
		) -> JS::Context& {
			return thiz.m_context;
		}

		#pragma endregion

		#pragma region basic

		auto evaluate (
			CStringView const & script,
			String const &      name
		) -> JS::Value {
			auto guard = std::lock_guard{JS::g_mutex};
			return thiz.m_context.evaluate(script, name);
		}

		auto shell_callback (
			List<String> const & argument
		) -> List<String> {
			auto guard = std::lock_guard{JS::g_mutex};
			return thiz.m_shell_callback(argument);
		}

		#pragma endregion

		#pragma region extension

		auto spawn (
		) -> Context {
			auto guard = std::lock_guard{JS::g_mutex};
			return Context{thiz.m_runtime, thiz.m_shell_callback};
		}

		// ----------------

		auto busy (
		) -> Boolean {
			return thiz.m_busy;
		}

		auto execute (
			JS::Value & executor,
			Thread &    thread
		) -> Void {
			assert_condition(!thiz.busy());
			thiz.m_busy = k_true;
			auto guard = std::lock_guard{JS::g_mutex};
			thread.run(
				[&, executor] {
					auto guard = std::lock_guard{JS::g_mutex};
					//M_log("----> {}"_sf(fmt::ptr(&thiz)));
					JS::Value::new_reference(thiz.m_context._context(), as_variable(executor)._value()).call(make_list<JS::Value>());
					as_variable(executor).set_undefined();
					//M_log("<---- {}"_sf(fmt::ptr(&thiz)));
					thiz.m_busy = k_false;
					return;
				}
			);
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
