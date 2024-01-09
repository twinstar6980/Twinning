#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.View.ModuleLauncher {

	public sealed partial class CommandSenderSettingPanel : CustomControl {

		#region life

		public CommandSenderSettingPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private CommandSenderSettingPanelController Controller { get; }

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

	public class CommandSenderSettingPanelController : CustomController {

		#region data

		public CommandSenderSettingPanel View { get; init; } = default!;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
			);
			return;
		}

		#endregion

		#region method configuration

		public String uMethodConfigurationText_Text {
			get {
				return Setting.Data.CommandSender.MethodConfiguration;
			}
		}

		public async void uMethodConfigurationText_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			Setting.Data.CommandSender.MethodConfiguration = senders.Text;
			return;
		}

		public async void uMethodConfigurationPick_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			var value = await StorageHelper.PickFile(WindowHelper.Find(this.View));
			if (value is not null) {
				Setting.Data.CommandSender.MethodConfiguration = value;
				this.NotifyPropertyChanged(
					nameof(this.uMethodConfigurationText_Text)
				);
			}
			return;
		}

		#endregion

	}

}
