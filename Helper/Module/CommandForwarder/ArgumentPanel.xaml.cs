#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.CommandForwarder {

	public sealed partial class ArgumentPanel : UserControl {

		#region life

		public ArgumentPanel (
		) {
			this.InitializeComponent();
			this.Controller = new ArgumentPanelController() { View = this };
		}

		// ----------------

		private ArgumentPanelController Controller { get; }

		#endregion

		#region property

		public static readonly DependencyProperty ConfigurationProperty = DependencyProperty.Register(
			nameof(ArgumentPanel.Configuration),
			typeof(List<CommandConfigurationModel.ArgumentConfiguration>),
			typeof(ArgumentPanel),
			new PropertyMetadata(null)
		);

		public List<CommandConfigurationModel.ArgumentConfiguration>? Configuration {
			get => this.GetValue(ArgumentPanel.ConfigurationProperty) as List<CommandConfigurationModel.ArgumentConfiguration>;
			set => this.SetValue(ArgumentPanel.ConfigurationProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(ArgumentPanel.Value),
			typeof(List<CommandConfigurationModel.ArgumentValue>),
			typeof(ArgumentPanel),
			new PropertyMetadata(null)
		);

		public List<CommandConfigurationModel.ArgumentValue>? Value {
			get => this.GetValue(ArgumentPanel.ValueProperty) as List<CommandConfigurationModel.ArgumentValue>;
			set => this.SetValue(ArgumentPanel.ValueProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty StampProperty = DependencyProperty.Register(
			nameof(ArgumentPanel.Stamp),
			typeof(UniqueStamp),
			typeof(ArgumentPanel),
			new PropertyMetadata(UniqueStamp.Default, (o, e) => {
				(o as ArgumentPanel)!.Controller.Update();
			})
		);

		public UniqueStamp Stamp {
			get => this.GetValue(ArgumentPanel.StampProperty) as UniqueStamp ?? throw new Exception();
			set => this.SetValue(ArgumentPanel.StampProperty, value);
		}

		#endregion

	}

	public class ArgumentPanelController : CustomController {

		#region data

		public ArgumentPanel View { get; init; } = default!;

		// ----------------

		public List<CommandConfigurationModel.ArgumentConfiguration>? Configuration => this.View.Configuration;

		public List<CommandConfigurationModel.ArgumentValue>? Value => this.View.Value;

		#endregion

		#region update

		public async void Update (
		) {
			this.uItem_ItemsSource = new List<ArgumentPanelItemController>();
			if (this.Configuration is not null && this.Value is not null) {
				Debug.Assert(this.Value.Count == this.Configuration.Count);
				for (var i = 0; i < this.Configuration.Count; i++) {
					this.uItem_ItemsSource.Add(new ArgumentPanelItemController() { Host = this, Configuration = this.Configuration[i], Value = this.Value[i] });
				}
			}
			this.NotifyPropertyChanged(
				nameof(this.uItem_ItemsSource)
			);
			return;
		}

		#endregion

		#region item

		public List<ArgumentPanelItemController> uItem_ItemsSource { get; set; } = new List<ArgumentPanelItemController>();

		#endregion

	}

	public class ArgumentPanelItemController : CustomController {

		#region data

		public ArgumentPanelController Host { get; init; } = default!;

		// ----------------

		public CommandConfigurationModel.ArgumentConfiguration Configuration { get; set; } = default!;

		public CommandConfigurationModel.ArgumentValue Value { get; set; } = default!;

		#endregion

		#region view

		public String uTitle_Text {
			get {
				return this.Configuration.Name;
			}
		}

		// ----------------

		public Boolean uActive_IsChecked {
			get {
				return this.Value.Value is not null;
			}
		}

		public async void uActive_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			this.Value.Value = !newValue ? null : this.Configuration.Option is not null ? this.Configuration.Option[0] : CommandConfigurationHelper.MakeArgumentValueDefault(this.Configuration.Type);
			this.NotifyPropertyChanged(
				nameof(this.uValue_Stamp)
			);
			return;
		}

		public String uActive_Content {
			get {
				return this.Configuration.Type switch {
					CommandConfigurationModel.ArgumentType.Confirmation => "C",
					CommandConfigurationModel.ArgumentType.Number       => "N",
					CommandConfigurationModel.ArgumentType.Integer      => "I",
					CommandConfigurationModel.ArgumentType.Size         => "Z",
					CommandConfigurationModel.ArgumentType.String       => "S",
					CommandConfigurationModel.ArgumentType.Path         => "P",
					_                                                   => throw new ArgumentOutOfRangeException(),
				};
			}
		}

		// ----------------

		public CommandConfigurationModel.ArgumentType uValue_Type {
			get {
				return this.Configuration.Type;
			}
		}

		public CommandConfigurationModel.ArgumentValue uValue_Value {
			get {
				return this.Value;
			}
		}

		public List<Object>? uValue_Option {
			get {
				return this.Configuration.Option?.Select((value) => (CommandConfigurationHelper.MakeArgumentValueDefault(this.Configuration.Type, value) ?? throw new Exception())).ToList();
			}
		}

		public UniqueStamp uValue_Stamp {
			get {
				return UniqueStamp.Create();
			}
		}

		#endregion

	}

}
