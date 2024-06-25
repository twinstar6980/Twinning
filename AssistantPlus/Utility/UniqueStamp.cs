#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;

namespace AssistantPlus.Utility {

	public class UniqueStamp {

		#region structor

		public UniqueStamp (
		) {
		}

		#endregion

		#region instance

		public static UniqueStamp Create (
		) {
			return new ();
		}

		public static UniqueStamp Default = UniqueStamp.Create();

		#endregion

	}

}
