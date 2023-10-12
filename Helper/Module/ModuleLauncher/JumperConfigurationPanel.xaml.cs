#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.ModuleLauncher {

	public sealed partial class JumperConfigurationPanel : UserControl {

		#region life

		public JumperConfigurationPanel (
		) {
			this.InitializeComponent();
			this.Controller = new JumperConfigurationPanelController() { View = this };
		}

		// ----------------

		public JumperConfigurationPanelController Controller { get; }

		#endregion

		#region property

		public static readonly DependencyProperty ConfigurationProperty = DependencyProperty.Register(
			nameof(JumperConfigurationPanel.Configuration),
			typeof(JumperConfiguration),
			typeof(JumperConfigurationPanel),
			new PropertyMetadata(new JumperConfiguration() { Title = "", WindowOption = new List<String>(), ModuleType = ModuleType.ModuleLauncher, ModuleOption = new List<String>() })
		);

		public JumperConfiguration Configuration {
			get => this.GetValue(JumperConfigurationPanel.ConfigurationProperty) as JumperConfiguration ?? throw new NullReferenceException();
			set => this.SetValue(JumperConfigurationPanel.ConfigurationProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty StampProperty = DependencyProperty.Register(
			nameof(JumperConfigurationPanel.Stamp),
			typeof(UniqueStamp),
			typeof(JumperConfigurationPanel),
			new PropertyMetadata(UniqueStamp.Default, (o, e) => {
				(o as JumperConfigurationPanel)!.Controller.Update();
			})
		);

		public UniqueStamp Stamp {
			get => this.GetValue(JumperConfigurationPanel.StampProperty) as UniqueStamp ?? throw new Exception();
			set => this.SetValue(JumperConfigurationPanel.StampProperty, value);
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

		public async void uTitleText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
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

		public async void uModuleTypeSelect_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			if (sender is not ComboBox senders) { return; }
			this.Configuration.ModuleType = (ModuleType)senders.SelectedIndex;
			return;
		}

		#endregion

		#region module option

		public String uModuleOptionText_Text {
			get {
				return String.Join('\r', this.Configuration.ModuleOption);
			}
		}

		public async void uModuleOptionText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			var parsedValue = senders.Text.Split('\r').ToList();
			if (parsedValue.Count != 0 && parsedValue.Last().Length == 0) {
				parsedValue.RemoveAt(parsedValue.Count - 1);
			}
			this.Configuration.ModuleOption = parsedValue;
			return;
		}

		#endregion

		#region window option

		public String uWindowOptionText_Text {
			get {
				return String.Join('\r', this.Configuration.WindowOption);
			}
		}

		public async void uWindowOptionText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			var parsedValue = senders.Text.Split('\r').ToList();
			if (parsedValue.Count != 0 && parsedValue.Last().Length == 0) {
				parsedValue.RemoveAt(parsedValue.Count - 1);
			}
			this.Configuration.WindowOption = parsedValue;
			return;
		}

		#endregion

	}

}
