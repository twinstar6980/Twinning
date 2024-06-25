#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.PackageBuilder {

	public sealed partial class GeneralResourcePropertyPanel : CustomControl {

		#region life

		public GeneralResourcePropertyPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private GeneralResourcePropertyPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(GeneralResourcePropertyPanel.Value),
			typeof(GeneralResourceProperty),
			typeof(GeneralResourcePropertyPanel),
			new (new GeneralResourceProperty() {
				Path = "",
				Type = "",
			})
		);

		public GeneralResourceProperty Value {
			get => this.GetValue(GeneralResourcePropertyPanel.ValueProperty).As<GeneralResourceProperty>();
			set => this.SetValue(GeneralResourcePropertyPanel.ValueProperty, value);
		}

		#endregion

	}

	public class GeneralResourcePropertyPanelController : CustomController {

		#region data

		public GeneralResourcePropertyPanel View { get; init; } = default!;

		// ----------------

		public GeneralResourceProperty Value => this.View.Value;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
				nameof(this.uPath_Text),
				nameof(this.uType_Text)
			);
			return;
		}

		#endregion

		#region path

		public async void uPath_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.Value.Path = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uPath_Text)
			);
			return;
		}

		public String uPath_Text {
			get {
				return this.Value.Path;
			}
		}

		#endregion

		#region type

		public async void uType_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.Value.Type = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uType_Text)
			);
			return;
		}

		public String uType_Text {
			get {
				return this.Value.Type;
			}
		}

		public async void uTypePick_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			var flyout = new MenuFlyout() {
				Placement = FlyoutPlacementMode.BottomEdgeAlignedRight,
			};
			foreach (var type in new[] {
					"File",
					"Image",
					"PopAnim",
					"Sound",
					"SoundBank",
					"DecodedSoundBank",
					"RenderEffect",
					"PrimeFont",
				}) {
				flyout.Items.Add(new MenuFlyoutItem() {
					Text = type,
				}.SelfAlso((it) => {
					it.Click += (_, _) => {
						this.Value.Type = type;
						this.NotifyPropertyChanged(
							nameof(this.uType_Text)
						);
					};
				}));
			}
			flyout.ShowAt(senders);
			return;
		}

		#endregion

	}

}
