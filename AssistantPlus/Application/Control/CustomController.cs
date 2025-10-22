#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus {

	public partial class CustomController : INotifyPropertyChanged {

		#region constructor

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
