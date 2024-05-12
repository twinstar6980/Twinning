#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.CommandSender {

	public sealed partial class ArgumentPanel : CustomControl {

		#region life

		public ArgumentPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private ArgumentPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty ConfigurationProperty = DependencyProperty.Register(
			nameof(ArgumentPanel.Configuration),
			typeof(List<ArgumentConfiguration>),
			typeof(ArgumentPanel),
			new (new List<ArgumentConfiguration>())
		);

		public List<ArgumentConfiguration> Configuration {
			get => this.GetValue(ArgumentPanel.ConfigurationProperty).AsClass<List<ArgumentConfiguration>>();
			set => this.SetValue(ArgumentPanel.ConfigurationProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(ArgumentPanel.Value),
			typeof(List<ArgumentValue>),
			typeof(ArgumentPanel),
			new (new List<ArgumentValue>())
		);

		public List<ArgumentValue> Value {
			get => this.GetValue(ArgumentPanel.ValueProperty).AsClass<List<ArgumentValue>>();
			set => this.SetValue(ArgumentPanel.ValueProperty, value);
		}

		#endregion

	}

	public class ArgumentPanelController : CustomController {

		#region data

		public ArgumentPanel View { get; init; } = default!;

		// ----------------

		public List<ArgumentConfiguration> Configuration => this.View.Configuration;

		public List<ArgumentValue> Value => this.View.Value;

		#endregion

		#region update

		public async void Update (
		) {
			GF.AssertTest(this.Configuration.Count == this.Value.Count);
			this.uList_ItemsSource = this.Configuration.Select((value, index) => (new ArgumentPanelItemController() { Host = this, Configuration = value, Value = this.Value[index] })).ToList();
			this.NotifyPropertyChanged(
				nameof(this.uList_ItemsSource)
			);
			return;
		}

		#endregion

		#region item

		public List<ArgumentPanelItemController> uList_ItemsSource { get; set; } = [];

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

		public async void uActive_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			this.Value.Data = !senders.IsChecked.AsNotNull()
				? null
				: this.Configuration.Option is not null
					? this.Configuration.Option[0]
					: ConfigurationHelper.MakeArgumentValueDefault(this.Configuration.Type);
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
					_                    => throw new (),
				};
			}
		}

		// ----------------

		public ArgumentType uValue_Type {
			get {
				return this.Configuration.Type;
			}
		}

		public List<Object>? uValue_Option {
			get {
				return this.Configuration.Option?.Select((value) => (ConfigurationHelper.MakeArgumentValueDefault(this.Configuration.Type, value).AsNotNull())).ToList();
			}
		}

		public ArgumentValue uValue_Value {
			get {
				return this.Value;
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
