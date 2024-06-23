#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Microsoft.Graphics.Canvas.Text;

namespace AssistantPlus.View.ModdingWorker {

	public sealed partial class SettingPanel : CustomControl {

		#region life

		public SettingPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private SettingPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty DataProperty = DependencyProperty.Register(
			nameof(SettingPanel.Data),
			typeof(Setting),
			typeof(SettingPanel),
			new (new Setting() {
				Kernel = "",
				Script = "",
				Argument = [],
				AutomaticScroll = false,
				ImmediateLaunch = false,
				MessageFont = "",
			})
		);

		public Setting Data {
			get => this.GetValue(SettingPanel.DataProperty).As<Setting>();
			set => this.SetValue(SettingPanel.DataProperty, value);
		}

		#endregion

	}

	public class SettingPanelController : CustomController {

		#region data

		public SettingPanel View { get; init; } = default!;

		// ----------------

		public Setting Data => this.View.Data;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
				nameof(this.uKernelText_Text),
				nameof(this.uScriptText_Text),
				nameof(this.uArgumentText_Text),
				nameof(this.uAutomaticScrollToggle_IsChecked),
				nameof(this.uImmediateLaunchToggle_IsChecked),
				nameof(this.uMessageFontText_Text)
			);
			return;
		}

		#endregion

		#region kernel

		public async void uKernelText_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.Data.Kernel = StorageHelper.Regularize(senders.Text);
			this.NotifyPropertyChanged(
				nameof(this.uKernelText_Text)
			);
			return;
		}

		public String uKernelText_Text {
			get {
				return this.Data.Kernel;
			}
		}

		public async void uKernelPick_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			var value = await StorageHelper.PickLoadFile(WindowHelper.Find(this.View), $"{nameof(ModdingWorker)}.Kernel");
			if (value != null) {
				this.Data.Kernel = value;
				this.NotifyPropertyChanged(
					nameof(this.uKernelText_Text)
				);
			}
			return;
		}

		#endregion

		#region script

		public async void uScriptText_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.Data.Script = StorageHelper.Regularize(senders.Text);
			this.NotifyPropertyChanged(
				nameof(this.uScriptText_Text)
			);
			return;
		}

		public String uScriptText_Text {
			get {
				return this.Data.Script;
			}
		}

		public async void uScriptPick_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			var value = await StorageHelper.PickLoadFile(WindowHelper.Find(this.View), $"{nameof(ModdingWorker)}.Script");
			if (value != null) {
				this.Data.Script = value;
				this.NotifyPropertyChanged(
					nameof(this.uScriptText_Text)
				);
			}
			return;
		}

		#endregion

		#region argument

		public async void uArgumentText_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.Data.Argument = ConvertHelper.ParseStringListFromStringWithLine(senders.Text);
			this.NotifyPropertyChanged(
				nameof(this.uArgumentText_Text)
			);
			return;
		}

		public String uArgumentText_Text {
			get {
				return ConvertHelper.MakeStringListToStringWithLine(this.Data.Argument);
			}
		}

		#endregion

		#region automatic scroll

		public Boolean uAutomaticScrollToggle_IsChecked {
			get {
				return this.Data.AutomaticScroll;
			}
		}

		public async void uAutomaticScrollToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.Data.AutomaticScroll = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region immediate launch

		public Boolean uImmediateLaunchToggle_IsChecked {
			get {
				return this.Data.ImmediateLaunch;
			}
		}

		public async void uImmediateLaunchToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.Data.ImmediateLaunch = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region message font

		public async void uMessageFontText_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.Data.MessageFont = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uMessageFontText_Text)
			);
			return;
		}

		public String uMessageFontText_Text {
			get {
				return this.Data.MessageFont;
			}
		}

		// ----------------

		public async void uMessageFontPick_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			var flyout = new MenuFlyout() {
				Placement = FlyoutPlacementMode.Left,
			};
			foreach (var item in CanvasTextFormat.GetSystemFontFamilies().Order()) {
				flyout.Items.Add(new MenuFlyoutItem() {
					Text = item,
					FontFamily = new (item),
				}.SelfAlso((it) => {
					it.Click += (_, _) => {
						this.Data.MessageFont = item;
						this.NotifyPropertyChanged(
							nameof(this.uMessageFontText_Text)
						);
					};
				}));
			}
			flyout.ShowAt(senders);
			return;
		}

		#endregion

	}

}
