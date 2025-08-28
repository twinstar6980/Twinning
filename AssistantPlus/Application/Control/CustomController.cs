#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus {

	public partial class CustomController : INotifyPropertyChanged {

		#region structor

		public CustomController (
		) {
			return;
		}

		#endregion

		#region implement INotifyPropertyChanged

		public event PropertyChangedEventHandler? PropertyChanged;

		public void NotifyPropertyChanged (
			String[] propertyNameList
		) {
			foreach (var propertyName in propertyNameList) {
				this.PropertyChanged?.Invoke(this, new (propertyName));
			}
			return;
		}

		#endregion

	}

}
