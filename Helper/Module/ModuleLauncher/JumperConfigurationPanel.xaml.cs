#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.ModuleLauncher {

	public sealed partial class JumperConfigurationPanel : CustomControl {

		#region life

		public JumperConfigurationPanel (
		) {
			this.InitializeComponent();
			this.Controller = new JumperConfigurationPanelController() { View = this };
		}

		// ----------------

		private JumperConfigurationPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty ConfigurationProperty = DependencyProperty.Register(
			nameof(JumperConfigurationPanel.Configuration),
			typeof(JumperConfiguration),
			typeof(JumperConfigurationPanel),
			new PropertyMetadata(new JumperConfiguration() {
				Title = "",
				WindowOption = new List<String>(),
				ModuleType = ModuleType.ModuleLauncher,
				ModuleOption = new List<String>(),
			})
		);

		public JumperConfiguration Configuration {
			get => this.GetValue(JumperConfigurationPanel.ConfigurationProperty).AsClass<JumperConfiguration>();
			set => this.SetValue(JumperConfigurationPanel.ConfigurationProperty, value);
		}

		#endregion

	}

	public class JumperConfigurationPanelController : CustomController {

		#region data

		public JumperConfigurationPanel View { get; init; } = default!;

		// ----------------

		public JumperConfiguration Configuration => this.View.Configuration;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
				nameof(this.uTitleText_Text),
				nameof(this.uModuleTypeSelect_SelectedIndex),
				nameof(this.uModuleOptionText_Text),
				nameof(this.uWindowOptionText_Text)
			);
			return;
		}

		#endregion

		#region title

		public String uTitleText_Text {
			get {
				return this.Configuration.Title;
			}
		}

		public async void uTitleText_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			this.Configuration.Title = senders.Text;
			return;
		}

		#endregion

		#region module type

		public List<String> uModuleTypeSelect_ItemsSource {
			get {
				return ModuleInformationConstant.Value.Select((value) => (value.Name)).ToList();
			}
		}

		public Size uModuleTypeSelect_SelectedIndex {
			get {
				return (Size)this.Configuration.ModuleType;
			}
		}

		public async void uModuleTypeSelect_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.AsClass<ComboBox>();
			this.Configuration.ModuleType = (ModuleType)senders.SelectedIndex;
			return;
		}

		#endregion

		#region module option

		public String uModuleOptionText_Text {
			get {
				return ConvertHelper.StringListToTextWithCr(this.Configuration.ModuleOption);
			}
		}

		public async void uModuleOptionText_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			this.Configuration.ModuleOption = ConvertHelper.StringListFromTextWithCr(senders.Text);
			return;
		}

		#endregion

		#region window option

		public String uWindowOptionText_Text {
			get {
				return ConvertHelper.StringListToTextWithCr(this.Configuration.WindowOption);
			}
		}

		public async void uWindowOptionText_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			this.Configuration.WindowOption = ConvertHelper.StringListFromTextWithCr(senders.Text);
			return;
		}

		#endregion

	}

}
