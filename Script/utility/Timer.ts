namespace Twinning.Script {

	// ------------------------------------------------

	export class Timer {

		// ------------------------------------------------

		private m_duration: number;

		private m_last_begin: null | number;

		// ------------------------------------------------

		constructor(
		) {
			this.m_duration = 0;
			this.m_last_begin = null;
			return;
		}

		// ------------------------------------------------

		start(
		): void {
			assert_test(this.m_last_begin === null);
			this.m_last_begin = Date.now();
			return;
		}

		stop(
		): void {
			assert_test(this.m_last_begin !== null);
			this.m_duration += Date.now() - this.m_last_begin;
			this.m_last_begin = null;
			return;
		}

		reset(
		): void {
			this.m_last_begin = null;
			this.m_duration = 0;
			return;
		}

		// ------------------------------------------------

		duration(
		): number {
			return this.m_duration;
		}

		// ------------------------------------------------

	}

	// ------------------------------------------------

}