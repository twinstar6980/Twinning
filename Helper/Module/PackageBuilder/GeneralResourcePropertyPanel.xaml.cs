#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.PackageBuilder {

	public sealed partial class GeneralResourcePropertyPanel : CustomControl {

		#region life

		public GeneralResourcePropertyPanel (
		) {
			this.InitializeComponent();
			this.Controller = new GeneralResourcePropertyPanelController() { View = this };
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
			new PropertyMetadata(new GeneralResourceProperty() {
				Path = "",
				Type = "",
			})
		);

		public GeneralResourceProperty Value {
			get => this.GetValue(GeneralResourcePropertyPanel.ValueProperty).AsClass<GeneralResourceProperty>();
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

		public String uPath_Text {
			get {
				return this.Value.Path;
			}
		}

		public async void uPath_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			this.Value.Path = senders.Text;
			return;
		}

		#endregion

		#region type

		public List<String> uType_ItemsSource {
			get {
				return new List<String>() {
					"File",
					"Image",
					"PopAnim",
					"Sound",
					"SoundBank",
					"DecodedSoundBank",
					"RenderEffect",
					"PrimeFont",
				};
			}
		}

		public String uType_Text {
			get {
				return this.Value.Type;
			}
		}

		public async void uType_TextChanged (
			AutoSuggestBox                     sender,
			AutoSuggestBoxTextChangedEventArgs args
		) {
			var senders = sender.AsClass<AutoSuggestBox>();
			this.Value.Type = senders.Text;
			return;
		}

		#endregion

	}

}
