#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;

namespace AssistantPlus.Utility {

	public class UniqueStamp {

		#region constructor

		public UniqueStamp (
		) {
			return;
		}

		#endregion

		#region instance

		public static UniqueStamp Create (
		) {
			return new ();
		}

		public static readonly UniqueStamp Default = UniqueStamp.Create();

		#endregion

	}

}
