#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.Home {

	public sealed partial class LauncherConfigurationPanel : CustomControl {

		#region life

		private LauncherConfigurationPanelController Controller { get; }

		// ----------------

		public LauncherConfigurationPanel (
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
			nameof(LauncherConfigurationPanel.Data),
			typeof(ModuleLauncherConfiguration),
			typeof(LauncherConfigurationPanel),
			new (new ModuleLauncherConfiguration() {
				Title = "",
				Type = ModuleType.ModdingWorker,
				Option = [],
				Command = [],
			})
		);

		public ModuleLauncherConfiguration Data {
			get => this.GetValue(LauncherConfigurationPanel.DataProperty).As<ModuleLauncherConfiguration>();
			set => this.SetValue(LauncherConfigurationPanel.DataProperty, value);
		}

		#endregion

	}

	public partial class LauncherConfigurationPanelController : CustomController {

		#region data

		public LauncherConfigurationPanel View { get; init; } = default!;

		// ----------------

		public ModuleLauncherConfiguration Data => this.View.Data;

		#endregion

		#region life

		public async Task UpdateView (
		) {
			this.NotifyPropertyChanged([
				nameof(this.uTitleText_Text),
				nameof(this.uTypeSelect_SelectedIndex),
				nameof(this.uOptionText_Text),
				nameof(this.uCommandText_Text),
			]);
			return;
		}

		#endregion

		#region title

		public async void uTitleText_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.Data.Title = senders.Text;
			this.NotifyPropertyChanged([
				nameof(this.uTitleText_Text),
			]);
			return;
		}

		public String uTitleText_Text {
			get {
				return this.Data.Title;
			}
		}

		#endregion

		#region type

		public List<String> uTypeSelect_ItemsSource {
			get {
				return Enum.GetValues<ModuleType>().Select((value) => (ModuleHelper.Query(value).Name)).ToList();
			}
		}

		public Size uTypeSelect_SelectedIndex {
			get {
				return this.Data.Type.CastPrimitive<Size>();
			}
		}

		public async void uTypeSelect_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			this.Data.Type = senders.SelectedIndex.CastPrimitive<ModuleType>();
			return;
		}

		#endregion

		#region option

		public async void uOptionText_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.Data.Option = ConvertHelper.ParseStringListFromStringWithLine(senders.Text);
			this.NotifyPropertyChanged([
				nameof(this.uOptionText_Text),
			]);
			return;
		}

		public String uOptionText_Text {
			get {
				return ConvertHelper.MakeStringListToStringWithLine(this.Data.Option);
			}
		}

		#endregion

		#region command

		public async void uCommandText_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.Data.Command = ConvertHelper.ParseStringListFromStringWithLine(senders.Text);
			this.NotifyPropertyChanged([
				nameof(this.uCommandText_Text),
			]);
			return;
		}

		public String uCommandText_Text {
			get {
				return ConvertHelper.MakeStringListToStringWithLine(this.Data.Command);
			}
		}

		#endregion

	}

}
