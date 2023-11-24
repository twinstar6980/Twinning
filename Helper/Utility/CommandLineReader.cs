#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Utility {

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
			var result = false;
			if (!this.Done() && this.mView[this.mPosition] == name) {
				result = true;
				this.mPosition++;
			}
			return result;
		}

		// ----------------

		public Boolean NextBoolean (
		) {
			return Boolean.Parse(this.NextString());
		}

		public Integer NextInteger (
		) {
			return Integer.Parse(this.NextString());
		}

		public Floater NextFloater (
		) {
			return Floater.Parse(this.NextString());
		}

		public String NextString (
		) {
			GF.AssertTest(!this.Done());
			var result = this.mView[this.mPosition];
			this.mPosition++;
			return result;
		}

		public TValue NextEnumeration<TValue> (
		)
			where TValue : struct {
			return Enum.Parse<TValue>(this.NextString());
		}

		public List<String> NextStringList (
		) {
			var result = this.mView.GetRange(this.mPosition, this.mView.Count - this.mPosition);
			this.mPosition = this.mView.Count;
			return result;
		}

		#endregion

	}

}
