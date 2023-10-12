#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.ModuleLauncher {

	public sealed partial class CommandSenderSettingPanel : UserControl {

		#region life

		public CommandSenderSettingPanel (
		) {
			this.InitializeComponent();
			this.Controller = new CommandSenderSettingPanelController() { View = this };
		}

		// ----------------

		public CommandSenderSettingPanelController Controller { get; }

		#endregion

	}

	public class CommandSenderSettingPanelController : CustomController {

		#region data

		public CommandSenderSettingPanel View { get; init; } = default!;

		#endregion

		#region method configuration

		public String uMethodConfigurationText_Text {
			get {
				return Setting.Data.CommandSender.MethodConfiguration;
			}
		}

		public async void uMethodConfigurationText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			Setting.Data.CommandSender.MethodConfiguration = senders.Text;
			return;
		}

		public async void uMethodConfigurationPick_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			var newValue = await StorageHelper.PickFile(WindowHelper.GetForElement(this.View));
			if (newValue is not null) {
				Setting.Data.CommandSender.MethodConfiguration = newValue;
				this.NotifyPropertyChanged(
					nameof(this.uMethodConfigurationText_Text)
				);
			}
			return;
		}

		#endregion

	}

}
