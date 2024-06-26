#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.ModdingWorker {

	public sealed partial class MessageCard : CustomControl {

		#region life

		public MessageCard (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private MessageCardController Controller { get; }

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
			nameof(MessageCard.Type),
			typeof(MessageType),
			typeof(MessageCard),
			new (MessageType.Verbosity)
		);

		public MessageType Type {
			get => this.GetValue(MessageCard.TypeProperty).As<MessageType>();
			set => this.SetValue(MessageCard.TypeProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
			nameof(MessageCard.Title),
			typeof(String),
			typeof(MessageCard),
			new ("")
		);

		public String Title {
			get => this.GetValue(MessageCard.TitleProperty).As<String>();
			set => this.SetValue(MessageCard.TitleProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty DescriptionProperty = DependencyProperty.Register(
			nameof(MessageCard.Description),
			typeof(List<String>),
			typeof(MessageCard),
			new (new List<String>())
		);

		public List<String> Description {
			get => this.GetValue(MessageCard.DescriptionProperty).As<List<String>>();
			set => this.SetValue(MessageCard.DescriptionProperty, value);
		}

		#endregion

	}

	public class MessageCardController : CustomController {

		#region data

		public MessageCard View { get; init; } = default!;

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

		public List<String> uDescription_ItemsSource {
			get {
				return this.Description.Select((value) => (value + '\0')).ToList();
			}
		}

		#endregion

	}

}
