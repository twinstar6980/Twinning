/** 计时 */
namespace TwinKleS {

	// ------------------------------------------------

	/** 计时器 */
	export class Timer {

		// ------------------------------------------------

		private m_duration: number;

		private m_last_begin: null | number;

		// ------------------------------------------------

		constructor(
		) {
			this.m_duration = 0;
			this.m_last_begin = null;
		}

		// ------------------------------------------------

		/** 继续计时 */
		start(
		): void {
			if (this.m_last_begin !== null) {
				throw new MyError(`TwinKleS.Timer.start : timer already start`);
			}
			this.m_last_begin = Date.now();
			return;
		}

		/** 暂停计时 */
		stop(
		): void {
			if (this.m_last_begin === null) {
				throw new MyError(`TwinKleS.Timer.stop : timer no start`);
			}
			this.m_duration += Date.now() - this.m_last_begin;
			this.m_last_begin = null;
			return;
		}

		/** 重置计时 */
		reset(
		): void {
			this.m_last_begin = null;
			this.m_duration = 0;
			return;
		}

		// ------------------------------------------------

		/** 获取累计时长 */
		duration(
		): number {
			return this.m_duration;
		}

		// ------------------------------------------------

	}

	// ------------------------------------------------

}