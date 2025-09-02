namespace Twinning.Script {

	export class Timer {

		// #region structor

		private m_duration: number;

		private m_last_begin: null | number;

		// ----------------

		public constructor(
		) {
			this.m_duration = 0;
			this.m_last_begin = null;
			return;
		}

		// #endregion

		// #region action

		public start(
		): void {
			assert_test(this.m_last_begin === null);
			this.m_last_begin = Date.now();
			return;
		}

		public stop(
		): void {
			assert_test(this.m_last_begin !== null);
			this.m_duration += Date.now() - this.m_last_begin;
			this.m_last_begin = null;
			return;
		}

		public reset(
		): void {
			this.m_last_begin = null;
			this.m_duration = 0;
			return;
		}

		// ----------------

		public duration(
		): number {
			return this.m_duration;
		}

		// #endregion

	}

}