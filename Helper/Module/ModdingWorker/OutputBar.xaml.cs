#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.ModdingWorker {

	public sealed partial class OutputBar : UserControl {

		#region life

		public OutputBar (
		) {
			this.InitializeComponent();
			this.Controller = new OutputBarController() { View = this };
		}

		// ----------------

		private OutputBarController Controller { get; }

		// ----------------

		private void UpdateVisualState (
		) {
			VisualStateManager.GoToState(this, $"{this.Type}State", false);
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty TypeProperty = DependencyProperty.Register(
			nameof(OutputBar.Type),
			typeof(MessageType),
			typeof(OutputBar),
			new PropertyMetadata(MessageType.Verbosity)
		);

		public MessageType Type {
			get => this.GetValue(OutputBar.TypeProperty) as MessageType? ?? throw new NullReferenceException();
			set => this.SetValue(OutputBar.TypeProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
			nameof(OutputBar.Title),
			typeof(String),
			typeof(OutputBar),
			new PropertyMetadata("")
		);

		public String Title {
			get => this.GetValue(OutputBar.TitleProperty) as String ?? throw new NullReferenceException();
			set => this.SetValue(OutputBar.TitleProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty DescriptionProperty = DependencyProperty.Register(
			nameof(OutputBar.Description),
			typeof(List<String>),
			typeof(OutputBar),
			new PropertyMetadata(new List<String>())
		);

		public List<String> Description {
			get => this.GetValue(OutputBar.DescriptionProperty) as List<String> ?? throw new NullReferenceException();
			set => this.SetValue(OutputBar.DescriptionProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty StampProperty = DependencyProperty.Register(
			nameof(OutputBar.Stamp),
			typeof(UniqueStamp),
			typeof(OutputBar),
			new PropertyMetadata(UniqueStamp.Default, (o, e) => {
				(o as OutputBar)!.UpdateVisualState();
				(o as OutputBar)!.Controller.Update();
			})
		);

		public UniqueStamp Stamp {
			get => this.GetValue(OutputBar.StampProperty) as UniqueStamp ?? throw new Exception();
			set => this.SetValue(OutputBar.StampProperty, value);
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
