#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Utility {

	public class UniqueStamp {

		#region instance

		public static UniqueStamp Create (
		) {
			return new ();
		}

		public static UniqueStamp Default = UniqueStamp.Create();

		#endregion

	}

}
