module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.thread.mutex;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel::Thread {

	#pragma region type

	class Mutex {

	private:

		std::mutex m_mutex;

	public:

		#pragma region structor

		~Mutex (
		) = default;

		// ----------------

		Mutex (
		) :
			m_mutex{} {
		}

		Mutex (
			Mutex const & that
		) = delete;

		Mutex (
			Mutex && that
		) = delete;

		#pragma endregion

		#pragma region operator

		auto operator = (
			Mutex const & that
		) -> Mutex & = delete;

		auto operator = (
			Mutex && that
		) -> Mutex & = delete;

		#pragma endregion

		#pragma region control

		auto lock (
		) -> Void {
			thiz.m_mutex.lock();
			return;
		}

		auto unlock (
		) -> Void {
			thiz.m_mutex.unlock();
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
