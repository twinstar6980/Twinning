module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.thread.mutex;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;

export namespace Twinning::Kernel::Thread {

	#pragma region type

	class Mutex {

	private:

		std::mutex m_mutex;

		std::thread::id m_owner;

	public:

		#pragma region constructor

		~Mutex(
		) = default;

		// ----------------

		Mutex(
		) :
			m_mutex{},
			m_owner{} {
			return;
		}

		Mutex(
			Mutex const & that
		) = delete;

		Mutex(
			Mutex && that
		) = delete;

		#pragma endregion

		#pragma region operator

		auto operator =(
			Mutex const & that
		) -> Mutex & = delete;

		auto operator =(
			Mutex && that
		) -> Mutex & = delete;

		#pragma endregion

		#pragma region control

		auto lock(
		) -> Void {
			thiz.m_mutex.lock();
			thiz.m_owner = std::this_thread::get_id();
			return;
		}

		auto unlock(
		) -> Void {
			assert_test(thiz.m_owner == std::this_thread::get_id());
			thiz.m_owner = std::thread::id{};
			thiz.m_mutex.unlock();
			return;
		}

		// ----------------

		auto is_locked_in_current(
		) const -> Boolean {
			return thiz.m_owner == std::this_thread::get_id();
		}

		#pragma endregion

	};

	#pragma endregion

}
