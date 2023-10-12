#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.CustomControl;
using Helper.Utility;

namespace Helper.Module.ModuleLauncher {

	public sealed partial class ResourceForwarderSettingPanel : UserControl {

		#region life

		public ResourceForwarderSettingPanel (
		) {
			this.InitializeComponent();
			this.Controller = new ResourceForwarderSettingPanelController() { View = this };
		}

		// ----------------

		public ResourceForwarderSettingPanelController Controller { get; }

		#endregion

	}

	public class ResourceForwarderSettingPanelController : CustomController {

		#region data

		public ResourceForwarderSettingPanel View { get; init; } = default!;

		#endregion

		#region option configuration

		public String uOptionConfigurationText_Text {
			get {
				return Setting.Data.ResourceForwarder.OptionConfiguration;
			}
		}

		public async void uOptionConfigurationText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			Setting.Data.ResourceForwarder.OptionConfiguration = senders.Text;
			return;
		}

		public async void uOptionConfigurationPick_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			var newValue = await StorageHelper.PickFile(WindowHelper.GetForElement(this.View));
			if (newValue is not null) {
				Setting.Data.ResourceForwarder.OptionConfiguration = newValue;
				this.NotifyPropertyChanged(
					nameof(this.uOptionConfigurationText_Text)
				);
			}
			return;
		}

		#endregion

		#region automatic close

		public Boolean uAutomaticCloseToggle_IsChecked {
			get {
				return Setting.Data.ResourceForwarder.AutomaticClose;
			}
		}

		public async void uAutomaticCloseToggle_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			Setting.Data.ResourceForwarder.AutomaticClose = newValue;
			return;
		}

		#endregion

		#region parallel execute

		public Boolean uParallelExecuteToggle_IsChecked {
			get {
				return Setting.Data.ResourceForwarder.ParallelExecute;
			}
		}

		public async void uParallelExecuteToggle_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			Setting.Data.ResourceForwarder.ParallelExecute = newValue;
			return;
		}

		#endregion

		#region enable filter

		public Boolean uEnableFilterToggle_IsChecked {
			get {
				return Setting.Data.ResourceForwarder.EnableFilter;
			}
		}

		public async void uEnableFilterToggle_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			Setting.Data.ResourceForwarder.EnableFilter = newValue;
			return;
		}

		#endregion

		#region enable batch

		public Boolean uEnableBatchToggle_IsChecked {
			get {
				return Setting.Data.ResourceForwarder.EnableBatch;
			}
		}

		public async void uEnableBatchToggle_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			Setting.Data.ResourceForwarder.EnableBatch = newValue;
			return;
		}

		#endregion

		#region remain input

		public Boolean uRemainInputToggle_IsChecked {
			get {
				return Setting.Data.ResourceForwarder.RemainInput;
			}
		}

		public async void uRemainInputToggle_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			Setting.Data.ResourceForwarder.RemainInput = newValue;
			return;
		}

		#endregion

	}

}
