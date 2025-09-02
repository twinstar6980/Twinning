namespace Twinning.Script {

	export class CommandLineReader {

		// #region structor

		private m_view: Array<string>;

		private m_position: number;

		// ----------------

		public constructor(
			view: Array<string>,
		) {
			this.m_view = view;
			this.m_position = 0;
			return;
		}

		// #endregion

		// #region query

		public done(
		): boolean {
			return !(0 <= this.m_position && this.m_position < this.m_view.length);
		}

		public check(
			name: string,
		): boolean {
			let state = !this.done() && this.m_view[this.m_position] === name;
			if (state) {
				this.m_position++;
			}
			return state;
		}

		// ----------------

		public next_string(
		): string {
			assert_test(!this.done());
			let value = this.m_view[this.m_position];
			this.m_position++;
			return value;
		}

		// #endregion

	}

}