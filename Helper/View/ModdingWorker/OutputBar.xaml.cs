#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.View.ModdingWorker {

	public sealed partial class OutputBar : CustomControl {

		#region life

		public OutputBar (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private OutputBarController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			VisualStateManager.GoToState(this, $"{this.Type}State", false);
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty TypeProperty = DependencyProperty.Register(
			nameof(OutputBar.Type),
			typeof(MessageType),
			typeof(OutputBar),
			new (MessageType.Verbosity)
		);

		public MessageType Type {
			get => this.GetValue(OutputBar.TypeProperty).AsStruct<MessageType>();
			set => this.SetValue(OutputBar.TypeProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
			nameof(OutputBar.Title),
			typeof(String),
			typeof(OutputBar),
			new ("")
		);

		public String Title {
			get => this.GetValue(OutputBar.TitleProperty).AsClass<String>();
			set => this.SetValue(OutputBar.TitleProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty DescriptionProperty = DependencyProperty.Register(
			nameof(OutputBar.Description),
			typeof(List<String>),
			typeof(OutputBar),
			new (new List<String>())
		);

		public List<String> Description {
			get => this.GetValue(OutputBar.DescriptionProperty).AsClass<List<String>>();
			set => this.SetValue(OutputBar.DescriptionProperty, value);
		}

		#endregion

	}

	public class OutputBarController : CustomController {

		#region data

		public OutputBar View { get; init; } = default!;

		// ----------------

		public MessageType Type => this.View.Type;

		public String Title => this.View.Title;

		public List<String> Description => this.View.Description;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
				nameof(this.uTitle_Text),
				nameof(this.uDescription_ItemsSource)
			);
			return;
		}

		#endregion

		#region view

		public String uTitle_Text {
			get {
				return this.Title;
			}
		}

		public List<OutputBarDescriptionItemController> uDescription_ItemsSource {
			get {
				return this.Description.Select((value) => (new OutputBarDescriptionItemController() { Host = this, Text = value })).ToList();
			}
		}

		#endregion

	}

	public class OutputBarDescriptionItemController : CustomController {

		#region data

		public OutputBarController Host { get; init; } = default!;

		// ----------------

		public String Text { get; set; } = default!;

		#endregion

		#region view

		public String uText_Text {
			get {
				return this.Text;
			}
		}

		#endregion

	}

}
