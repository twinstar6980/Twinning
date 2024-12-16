module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.thread.unlocker;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.thread.mutex;

export namespace Twinning::Kernel::Thread {

	#pragma region type

	class Unlocker {

	private:

		Mutex & m_mutex;

	public:

		#pragma region structor

		~Unlocker (
		) {
			thiz.m_mutex.lock();
		}

		// ----------------

		Unlocker (
		) = delete;

		Unlocker (
			Unlocker const & that
		) = delete;

		Unlocker (
			Unlocker && that
		) = delete;

		// ----------------

		explicit Unlocker (
			Mutex & mutex
		) :
			m_mutex{mutex} {
			thiz.m_mutex.unlock();
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			Unlocker const & that
		) -> Unlocker & = delete;

		auto operator = (
			Unlocker && that
		) -> Unlocker & = delete;

		#pragma endregion

	};

	#pragma endregion

}
