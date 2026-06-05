namespace Twinning.Script {

	export class Finalizer {

		// #region constructor

		private m_action: () => void;

		// ----------------

		public constructor(
			action: () => void,
		) {
			this.m_action = action;
			return;
		}

		// #endregion

		// #region dispose

		[Symbol.dispose](
		): void {
			this.m_action();
			return;
		}

		// #endregion

	}

}
