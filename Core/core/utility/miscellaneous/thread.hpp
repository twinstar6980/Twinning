#pragma once

#include "core/utility/trait/trait.hpp"
#include <thread>

namespace TwinStar::Core {

	#pragma region type

	class Thread {

	private:

		std::thread m_thread{};

	public:

		#pragma region structor

		~Thread (
		) = default;

		// ----------------

		Thread (
		) = default;

		Thread (
			Thread const & that
		) = delete;

		Thread (
			Thread && that
		) = default;

		#pragma endregion

		#pragma region operator

		auto operator = (
			Thread const & that
		) -> Thread & = delete;

		auto operator = (
			Thread && that
		) -> Thread & = default;

		#pragma endregion

		#pragma region control

		template <typename Executor> requires
			CategoryConstraint<IsPureInstance<Executor>>
			&& (IsCallable<Executor> && CallableTrait<Executor>::Argument::size == 0_szz)
		auto run (
			Executor const & executor
		) -> void {
			assert_test(!thiz.joinable());
			thiz.m_thread = std::thread{executor};
			return;
		}

		// ----------------

		auto joinable (
		) -> Boolean {
			return thiz.m_thread.joinable();
		}

		// ----------------

		auto join (
		) -> Void {
			assert_test(thiz.joinable());
			return thiz.m_thread.join();
		}

		auto detach (
		) -> Void {
			assert_test(thiz.joinable());
			return thiz.m_thread.detach();
		}

		// ----------------

		static auto yield (
		) -> Void {
			std::this_thread::yield();
			return;
		}

		static auto sleep (
			Size const & duration
		) -> Void {
			std::this_thread::sleep_for(std::chrono::milliseconds{duration.value});
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
