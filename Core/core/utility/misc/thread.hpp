#pragma once

// TODO : use std::jthread ?

#include "core/utility/trait/trait.hpp"
#include <thread>

namespace TwinKleS::Core {

	#pragma region type

	class Thread {

	private: //

		std::jthread m_thread{};

	public: //

		#pragma region structor

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
		) -> Thread& = delete;

		auto operator = (
			Thread && that
		) -> Thread& = default;

		#pragma endregion

		#pragma region function

		template <typename Executor> requires
			CategoryConstraint<IsPureInstance<Executor>>
			&& (IsCallable<Executor> && CallableTrait<Executor>::Argument::size == 0_szz)
		auto run (
			Executor const & executor
		) -> void {
			thiz.m_thread = std::jthread{executor};
			return;
		}

		#pragma endregion

	};
	#pragma endregion

}
