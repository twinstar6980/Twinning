#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;

namespace AssistantPlus.Utility {

	public class CommandLineReader {

		#region structor

		private List<String> mView;

		private Size mPosition;

		// ----------------

		public CommandLineReader (
			List<String> view
		) {
			this.mView = view;
			this.mPosition = 0;
		}

		#endregion

		#region query

		public Boolean Done (
		) {
			return !(0 <= this.mPosition && this.mPosition < this.mView.Count);
		}

		public Boolean Check (
			String name
		) {
			var state = !this.Done() && this.mView[this.mPosition] == name;
			if (state) {
				this.mPosition++;
			}
			return state;
		}

		// ----------------

		public Boolean NextBoolean (
		) {
			return this.NextString().SelfLet((it) => (Boolean.Parse(it)));
		}

		public Integer NextInteger (
		) {
			return this.NextString().SelfLet((it) => (Integer.Parse(it)));
		}

		public Floater NextFloater (
		) {
			return this.NextString().SelfLet((it) => (Floater.Parse(it)));
		}

		public String NextString (
		) {
			GF.AssertTest(!this.Done());
			var value = this.mView[this.mPosition];
			this.mPosition++;
			return value;
		}

		public List<String> NextStringList (
		) {
			var value = this.mView[this.mPosition..];
			this.mPosition = this.mView.Count;
			return value;
		}

		#endregion

	}

}
