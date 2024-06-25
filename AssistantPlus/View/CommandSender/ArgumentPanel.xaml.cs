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
			get => this.GetValue(ArgumentPanel.ConfigurationProperty).As<List<ArgumentConfiguration>>();
			set => this.SetValue(ArgumentPanel.ConfigurationProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(ArgumentPanel.Value),
			typeof(List<Wrapper<ValueExpression>>),
			typeof(ArgumentPanel),
			new (new List<Wrapper<ValueExpression>>())
		);

		public List<Wrapper<ValueExpression>> Value {
			get => this.GetValue(ArgumentPanel.ValueProperty).As<List<Wrapper<ValueExpression>>>();
			set => this.SetValue(ArgumentPanel.ValueProperty, value);
		}

		#endregion

	}

	public class ArgumentPanelController : CustomController {

		#region data

		public ArgumentPanel View { get; init; } = default!;

		// ----------------

		public List<ArgumentConfiguration> Configuration => this.View.Configuration;

		public List<Wrapper<ValueExpression>> Value => this.View.Value;

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

		public Wrapper<ValueExpression> Value { get; set; } = default!;

		#endregion

		#region view

		public String uValue_Label {
			get {
				return this.Configuration.Name;
			}
		}

		public ArgumentType uValue_Type {
			get {
				return this.Configuration.Type;
			}
		}

		public List<ValueExpression>? uValue_Option {
			get {
				return this.Configuration.Option?.Select((value) => (ConfigurationHelper.ParseArgumentValueJson(this.Configuration.Type, value).AsNotNull())).ToList();
			}
		}

		public Wrapper<ValueExpression> uValue_Value {
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
