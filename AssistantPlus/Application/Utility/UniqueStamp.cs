#pragma warning disable 0,

using Twinning.AssistantPlus;

namespace Twinning.AssistantPlus.Utility {

	public class UniqueStamp {

		#region constructor

		private UniqueStamp(
		) {
			return;
		}

		#endregion

		#region instance

		public static UniqueStamp Create(
		) {
			return new ();
		}

		public static readonly UniqueStamp Default = UniqueStamp.Create();

		#endregion

	}

}
