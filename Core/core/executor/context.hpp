#pragma once

#include "core/utility/utility.hpp"
#include "core/executor/shell_callback.hpp"

namespace TwinKleS::Core::Executor {

	#pragma region type

	class Context {

	protected: //

		JS::Runtime         m_runtime;
		JS::Context         m_context;
		ShellCallback       m_shell_callback;
		Optional<JS::Value> m_executor;
		Optional<JS::Value> m_result;

	public: //

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
			m_executor{},
			m_result{JS::Value{}} {
		}

		Context (
			JS::Runtime &         runtime,
			ShellCallback const & shell_callback
		) :
			m_runtime{JS::Runtime::new_reference(runtime._runtime())},
			m_context{m_runtime.new_context()},
			m_shell_callback{shell_callback},
			m_executor{},
			m_result{JS::Value{}} {
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
		) -> List<String> const& {
			auto guard = std::lock_guard{JS::g_mutex};
			return as_variable(thiz.m_shell_callback(argument));
		}

		#pragma endregion

		#pragma region thread api

		auto spawn (
		) -> Context {
			auto guard = std::lock_guard{JS::g_mutex};
			return Context{thiz.m_runtime, thiz.m_shell_callback};
		}

		// ----------------

		auto yield (
		) -> Void {
			std::this_thread::yield();
			return;
		}

		// ----------------

		auto execute (
			JS::Value & executor,
			Thread &    thread
		) -> Void {
			thiz.m_executor.set(executor);
			thiz.m_result.reset();
			auto guard = std::lock_guard{JS::g_mutex};
			thread.run(
				[&] {
					auto guard = std::lock_guard{JS::g_mutex};
					//M_log("----> {}"_sf(fmt::ptr(&thiz)));
					thiz.m_result.set(JS::Value::new_reference(thiz.m_context._context(), as_variable(thiz.m_executor.get())._value()).call(make_list<JS::Value>()));
					thiz.m_executor.reset();
					//M_log("<---- {}"_sf(fmt::ptr(&thiz)));
					return;
				}
			);
			return;
		}

		auto state (
		) -> Boolean {
			return !thiz.m_result.has();
		}

		auto result (
		) -> JS::Value {
			return thiz.m_result.get();
		}

		#pragma endregion

	};

	#pragma endregion

}
