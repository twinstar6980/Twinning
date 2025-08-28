#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.ResourceShipper {

	public sealed partial class SettingPanel : CustomControl {

		#region life

		private SettingPanelController Controller { get; }

		// ----------------

		public SettingPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
			return;
		}

		// ----------------

		protected override async Task StampUpdate (
		) {
			await this.Controller.UpdateView();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty DataProperty = DependencyProperty.Register(
			nameof(SettingPanel.Data),
			typeof(Setting),
			typeof(SettingPanel),
			new (new Setting() {
				OptionConfiguration = "",
				ParallelForward = false,
				EnableFilter = true,
				EnableBatch = false,
			})
		);

		public Setting Data {
			get => this.GetValue(SettingPanel.DataProperty).As<Setting>();
			set => this.SetValue(SettingPanel.DataProperty, value);
		}

		#endregion

	}

	public partial class SettingPanelController : CustomController {

		#region data

		public SettingPanel View { get; init; } = default!;

		// ----------------

		public Setting Data => this.View.Data;

		#endregion

		#region life

		public async Task UpdateView (
		) {
			this.NotifyPropertyChanged([
				nameof(this.uOptionConfigurationText_Text),
				nameof(this.uParallelForwardToggle_IsChecked),
				nameof(this.uEnableFilterToggle_IsChecked),
				nameof(this.uEnableBatchToggle_IsChecked),
			]);
			return;
		}

		#endregion

		#region option configuration

		public async void uOptionConfigurationText_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.Data.OptionConfiguration = StorageHelper.Regularize(senders.Text);
			this.NotifyPropertyChanged([
				nameof(this.uOptionConfigurationText_Text),
			]);
			return;
		}

		public String uOptionConfigurationText_Text {
			get {
				return this.Data.OptionConfiguration;
			}
		}

		public async void uOptionConfigurationPick_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			var value = await StorageHelper.PickLoadFile(App.MainWindow, $"@{nameof(ResourceShipper)}.OptionConfiguration");
			if (value != null) {
				this.Data.OptionConfiguration = value;
				this.NotifyPropertyChanged([
					nameof(this.uOptionConfigurationText_Text),
				]);
			}
			return;
		}

		#endregion

		#region parallel forward

		public Boolean uParallelForwardToggle_IsChecked {
			get {
				return this.Data.ParallelForward;
			}
		}

		public async void uParallelForwardToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.Data.ParallelForward = senders.IsChecked.AsNotNull();
			this.NotifyPropertyChanged([
				nameof(this.uParallelForwardToggle_IsChecked),
			]);
			return;
		}

		#endregion

		#region enable filter

		public Boolean uEnableFilterToggle_IsChecked {
			get {
				return this.Data.EnableFilter;
			}
		}

		public async void uEnableFilterToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.Data.EnableFilter = senders.IsChecked.AsNotNull();
			this.NotifyPropertyChanged([
				nameof(this.uEnableFilterToggle_IsChecked),
			]);
			return;
		}

		#endregion

		#region enable batch

		public Boolean uEnableBatchToggle_IsChecked {
			get {
				return this.Data.EnableBatch;
			}
		}

		public async void uEnableBatchToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.Data.EnableBatch = senders.IsChecked.AsNotNull();
			this.NotifyPropertyChanged([
				nameof(this.uEnableBatchToggle_IsChecked),
			]);
			return;
		}

		#endregion

	}

}
