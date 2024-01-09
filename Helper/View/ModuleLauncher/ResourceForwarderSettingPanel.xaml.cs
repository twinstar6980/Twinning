#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.View.ModuleLauncher {

	public sealed partial class ResourceForwarderSettingPanel : CustomControl {

		#region life

		public ResourceForwarderSettingPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private ResourceForwarderSettingPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		#endregion

	}

	public class ResourceForwarderSettingPanelController : CustomController {

		#region data

		public ResourceForwarderSettingPanel View { get; init; } = default!;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
			);
			return;
		}

		#endregion

		#region option configuration

		public String uOptionConfigurationText_Text {
			get {
				return Setting.Data.ResourceForwarder.OptionConfiguration;
			}
		}

		public async void uOptionConfigurationText_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			Setting.Data.ResourceForwarder.OptionConfiguration = senders.Text;
			return;
		}

		public async void uOptionConfigurationPick_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			var value = await StorageHelper.PickFile(WindowHelper.Find(this.View));
			if (value is not null) {
				Setting.Data.ResourceForwarder.OptionConfiguration = value;
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

		public async void uAutomaticCloseToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			Setting.Data.ResourceForwarder.AutomaticClose = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region parallel execute

		public Boolean uParallelExecuteToggle_IsChecked {
			get {
				return Setting.Data.ResourceForwarder.ParallelExecute;
			}
		}

		public async void uParallelExecuteToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			Setting.Data.ResourceForwarder.ParallelExecute = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region enable filter

		public Boolean uEnableFilterToggle_IsChecked {
			get {
				return Setting.Data.ResourceForwarder.EnableFilter;
			}
		}

		public async void uEnableFilterToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			Setting.Data.ResourceForwarder.EnableFilter = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region enable batch

		public Boolean uEnableBatchToggle_IsChecked {
			get {
				return Setting.Data.ResourceForwarder.EnableBatch;
			}
		}

		public async void uEnableBatchToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			Setting.Data.ResourceForwarder.EnableBatch = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region remain input

		public Boolean uRemainInputToggle_IsChecked {
			get {
				return Setting.Data.ResourceForwarder.RemainInput;
			}
		}

		public async void uRemainInputToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			Setting.Data.ResourceForwarder.RemainInput = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

	}

}
