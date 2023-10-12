#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.ModuleLauncher {

	public sealed partial class ModdingWorkerSettingPanel : UserControl {

		#region life

		public ModdingWorkerSettingPanel (
		) {
			this.InitializeComponent();
			this.Controller = new ModdingWorkerSettingPanelController() { View = this };
		}

		// ----------------

		public ModdingWorkerSettingPanelController Controller { get; }

		#endregion

	}

	public class ModdingWorkerSettingPanelController : CustomController {

		#region data

		public ModdingWorkerSettingPanel View { get; init; } = default!;

		#endregion

		#region kernel

		public String uKernelText_Text {
			get {
				return Setting.Data.ModdingWorker.Kernel;
			}
		}

		public async void uKernelText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
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

		public async void uScriptText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			Setting.Data.ModdingWorker.Script = senders.Text;
			return;
		}

		#endregion

		#region argument

		public String uArgumentText_Text {
			get {
				return String.Join('\r', Setting.Data.ModdingWorker.Argument);
			}
		}

		public async void uArgumentText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			var parsedValue = senders.Text.Split('\r').ToList();
			if (parsedValue.Count != 0 && parsedValue.Last().Length == 0) {
				parsedValue.RemoveAt(parsedValue.Count - 1);
			}
			Setting.Data.ModdingWorker.Argument = parsedValue;
			return;
		}

		#endregion

		#region automatic close

		public Boolean uAutomaticCloseToggle_IsChecked {
			get {
				return Setting.Data.ModdingWorker.AutomaticClose;
			}
		}

		public async void uAutomaticCloseToggle_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			Setting.Data.ModdingWorker.AutomaticClose = newValue;
			return;
		}

		#endregion

		#region automatic scroll

		public Boolean uAutomaticScrollToggle_IsChecked {
			get {
				return Setting.Data.ModdingWorker.AutomaticScroll;
			}
		}

		public async void uAutomaticScrollToggle_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			Setting.Data.ModdingWorker.AutomaticScroll = newValue;
			return;
		}

		#endregion

		#region immediate launch

		public Boolean uImmediateLaunchToggle_IsChecked {
			get {
				return Setting.Data.ModdingWorker.ImmediateLaunch;
			}
		}

		public async void uImmediateLaunchToggle_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			Setting.Data.ModdingWorker.ImmediateLaunch = newValue;
			return;
		}

		#endregion

		#region alternative launch script

		public String uAlternativeLaunchScriptText_Text {
			get {
				return Setting.Data.ModdingWorker.AlternativeLaunchScript;
			}
		}

		public async void uAlternativeLaunchScriptText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			Setting.Data.ModdingWorker.AlternativeLaunchScript = senders.Text;
			return;
		}

		public async void uAlternativeLaunchScriptPick_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			var newValue = await StorageHelper.PickFile(WindowHelper.GetForElement(this.View));
			if (newValue is not null) {
				Setting.Data.ModdingWorker.AlternativeLaunchScript = newValue;
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

		public async void uMessageFontFamilyText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			Setting.Data.ModdingWorker.MessageFontFamily = senders.Text;
			return;
		}

		#endregion

	}

}
