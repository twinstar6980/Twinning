#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Utility {

	public class CommandLineReader {

		#region structor

		private List<String> View;

		private Size Position;

		// ----------------

		public CommandLineReader (
			List<String> view
		) {
			this.View = view;
			this.Position = 0;
		}

		#endregion

		#region query

		public Boolean Done (
		) {
			return !(0 <= this.Position && this.Position < this.View.Count);
		}

		public Boolean Check (
			String name
		) {
			var result = false;
			if (!this.Done() && this.View[this.Position] == name) {
				result = true;
				this.Position++;
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
			var result = this.View[this.Position];
			this.Position++;
			return result;
		}

		public TValue NextEnumeration<TValue> (
		)
			where TValue : struct {
			return Enum.Parse<TValue>(this.NextString());
		}

		public List<String> NextStringList (
		) {
			var result = this.View[this.Position..];
			this.Position = this.View.Count;
			return result;
		}

		#endregion

	}

}
