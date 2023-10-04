#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.CommandSender {

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
			typeof(List<ArgumentConfiguration>),
			typeof(ArgumentPanel),
			new PropertyMetadata(null)
		);

		public List<ArgumentConfiguration>? Configuration {
			get => this.GetValue(ArgumentPanel.ConfigurationProperty) as List<ArgumentConfiguration>;
			set => this.SetValue(ArgumentPanel.ConfigurationProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(ArgumentPanel.Value),
			typeof(List<ArgumentValue>),
			typeof(ArgumentPanel),
			new PropertyMetadata(null)
		);

		public List<ArgumentValue>? Value {
			get => this.GetValue(ArgumentPanel.ValueProperty) as List<ArgumentValue>;
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

		public List<ArgumentConfiguration>? Configuration => this.View.Configuration;

		public List<ArgumentValue>? Value => this.View.Value;

		#endregion

		#region update

		public async void Update (
		) {
			this.uItem_ItemsSource = new List<ArgumentPanelItemController>();
			if (this.Configuration is not null && this.Value is not null) {
				Debug.Assert(this.Value.Count == this.Configuration.Count);
				for (var index = 0; index < this.Configuration.Count; ++index) {
					this.uItem_ItemsSource.Add(new ArgumentPanelItemController() { Host = this, Configuration = this.Configuration[index], Value = this.Value[index] });
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

		public ArgumentConfiguration Configuration { get; set; } = default!;

		public ArgumentValue Value { get; set; } = default!;

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
				return this.Value.Data is not null;
			}
		}

		public async void uActive_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			this.Value.Data = !newValue ? null : this.Configuration.Option is not null ? this.Configuration.Option[0] : ConfigurationHelper.MakeArgumentValueDefault(this.Configuration.Type);
			this.NotifyPropertyChanged(
				nameof(this.uValue_Stamp)
			);
			return;
		}

		public String uActive_Content {
			get {
				return this.Configuration.Type switch {
					ArgumentType.Boolean => "B",
					ArgumentType.Integer => "I",
					ArgumentType.Floater => "F",
					ArgumentType.Size    => "Z",
					ArgumentType.String  => "S",
					ArgumentType.Path    => "P",
					_                    => throw new ArgumentOutOfRangeException(),
				};
			}
		}

		// ----------------

		public ArgumentType uValue_Type {
			get {
				return this.Configuration.Type;
			}
		}

		public ArgumentValue uValue_Value {
			get {
				return this.Value;
			}
		}

		public List<Object>? uValue_Option {
			get {
				return this.Configuration.Option?.Select((value) => (ConfigurationHelper.MakeArgumentValueDefault(this.Configuration.Type, value) ?? throw new Exception())).ToList();
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
