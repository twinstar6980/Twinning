#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.ModuleLauncher {

	public sealed partial class ModdingWorkerSettingPanel : CustomControl {

		#region life

		public ModdingWorkerSettingPanel (
		) {
			this.InitializeComponent();
			this.Controller = new ModdingWorkerSettingPanelController() { View = this };
		}

		// ----------------

		private ModdingWorkerSettingPanelController Controller { get; }

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

	public class ModdingWorkerSettingPanelController : CustomController {

		#region data

		public ModdingWorkerSettingPanel View { get; init; } = default!;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
			);
			return;
		}

		#endregion

		#region kernel

		public String uKernelText_Text {
			get {
				return Setting.Data.ModdingWorker.Kernel;
			}
		}

		public async void uKernelText_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			Setting.Data.ModdingWorker.Kernel = senders.Text;
			return;
		}

		#endregion

		#region script

		public String uScriptText_Text {
			get {
				return Setting.Data.ModdingWorker.Script;
			}
		}

		public async void uScriptText_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			Setting.Data.ModdingWorker.Script = senders.Text;
			return;
		}

		#endregion

		#region argument

		public String uArgumentText_Text {
			get {
				return ConvertHelper.StringListToTextWithCr(Setting.Data.ModdingWorker.Argument);
			}
		}

		public async void uArgumentText_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			Setting.Data.ModdingWorker.Argument = ConvertHelper.StringListFromTextWithCr(senders.Text);
			return;
		}

		#endregion

		#region automatic scroll

		public Boolean uAutomaticScrollToggle_IsChecked {
			get {
				return Setting.Data.ModdingWorker.AutomaticScroll;
			}
		}

		public async void uAutomaticScrollToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			Setting.Data.ModdingWorker.AutomaticScroll = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region immediate launch

		public Boolean uImmediateLaunchToggle_IsChecked {
			get {
				return Setting.Data.ModdingWorker.ImmediateLaunch;
			}
		}

		public async void uImmediateLaunchToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			Setting.Data.ModdingWorker.ImmediateLaunch = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region alternative launch script

		public String uAlternativeLaunchScriptText_Text {
			get {
				return Setting.Data.ModdingWorker.AlternativeLaunchScript;
			}
		}

		public async void uAlternativeLaunchScriptText_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			Setting.Data.ModdingWorker.AlternativeLaunchScript = senders.Text;
			return;
		}

		public async void uAlternativeLaunchScriptPick_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			var value = await StorageHelper.PickFile(WindowHelper.GetForElement(this.View));
			if (value is not null) {
				Setting.Data.ModdingWorker.AlternativeLaunchScript = value;
				this.NotifyPropertyChanged(
					nameof(this.uAlternativeLaunchScriptText_Text)
				);
			}
			return;
		}

		#endregion

		#region message font family

		public String uMessageFontFamilyText_Text {
			get {
				return Setting.Data.ModdingWorker.MessageFontFamily;
			}
		}

		public async void uMessageFontFamilyText_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			Setting.Data.ModdingWorker.MessageFontFamily = senders.Text;
			return;
		}

		#endregion

	}

}
