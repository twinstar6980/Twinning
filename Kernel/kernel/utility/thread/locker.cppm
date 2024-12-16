module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.thread.locker;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.thread.mutex;

export namespace Twinning::Kernel::Thread {

	#pragma region type

	class Locker {

	private:

		Mutex & m_mutex;

	public:

		#pragma region structor

		~Locker (
		) {
			thiz.m_mutex.unlock();
		}

		// ----------------

		Locker (
		) = delete;

		Locker (
			Locker const & that
		) = delete;

		Locker (
			Locker && that
		) = delete;

		// ----------------

		explicit Locker (
			Mutex & mutex
		) :
			m_mutex{mutex} {
			thiz.m_mutex.lock();
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			Locker const & that
		) -> Locker & = delete;

		auto operator = (
			Locker && that
		) -> Locker & = delete;

		#pragma endregion

	};

	#pragma endregion

}
