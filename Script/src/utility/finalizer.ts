namespace Twinning.Script {

	export class Finalizer {

		// #region constructor

		private m_action: () => void;

		private m_disposed: boolean;

		// ----------------

		public constructor(
			action: () => void,
		) {
			this.m_action = action;
			this.m_disposed = false;
			return;
		}

		// #endregion

		// #region dispose

		public dispose(
		): void {
			assert_test(!this.m_disposed);
			this.m_disposed = true;
			this.m_action();
			return;
		}

		// ----------------

		[Symbol.dispose](
		): void {
			if (!this.m_disposed) {
				this.dispose();
			}
			return;
		}

		// #endregion

	}

}
