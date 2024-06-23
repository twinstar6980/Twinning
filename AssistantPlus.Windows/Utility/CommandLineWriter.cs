#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;

namespace AssistantPlus.Utility {

	public class CommandLineWriter {

		#region structor

		private List<String> mView;

		// ----------------

		public CommandLineWriter (
		) {
			this.mView = [];
		}

		#endregion

		#region query

		public List<String> Done (
		) {
			return this.mView;
		}

		public Boolean Check (
			String  name,
			Boolean state = true
		) {
			if (state) {
				this.NextString(name);
			}
			return state;
		}

		// ----------------

		public void NextBoolean (
			Boolean value
		) {
			this.NextString(value.SelfLet((it) => (ConvertHelper.MakeBooleanToString(it))));
			return;
		}

		public void NextInteger (
			Integer value
		) {
			this.NextString(value.SelfLet((it) => (ConvertHelper.MakeIntegerToString(it, false))));
			return;
		}

		public void NextFloater (
			Floater value
		) {
			this.NextString(value.SelfLet((it) => (ConvertHelper.MakeFloaterToString(it, false))));
			return;
		}

		public void NextString (
			String value
		) {
			this.mView.Add(value);
			return;
		}

		public void NextStringList (
			List<String> value
		) {
			this.mView.AddRange(value);
			return;
		}

		#endregion

	}

}
