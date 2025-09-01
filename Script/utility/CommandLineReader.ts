namespace Twinning.Script {

	// ------------------------------------------------

	export class CommandLineReader {

		// ------------------------------------------------

		private m_view: Array<string>;

		private m_position: number;

		// ------------------------------------------------

		constructor(
			view: Array<string>,
		) {
			this.m_view = view;
			this.m_position = 0;
			return;
		}

		// ------------------------------------------------

		done(
		): boolean {
			return !(0 <= this.m_position && this.m_position < this.m_view.length);
		}

		check(
			name: string,
		): boolean {
			let state = !this.done() && this.m_view[this.m_position] === name;
			if (state) {
				this.m_position++;
			}
			return state;
		}

		// ------------------------------------------------

		next_string(
		): string {
			assert_test(!this.done());
			let value = this.m_view[this.m_position];
			this.m_position++;
			return value;
		}

		// ------------------------------------------------

	}

	// ------------------------------------------------

}