#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.PackageBuilder {

	public sealed partial class SpecialRtonResourcePropertyPanel : CustomControl {

		#region life

		public SpecialRtonResourcePropertyPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private SpecialRtonResourcePropertyPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty ConversionSourceProperty = DependencyProperty.Register(
			nameof(SpecialRtonResourcePropertyPanel.ConversionSource),
			typeof(List<String>),
			typeof(SpecialRtonResourcePropertyPanel),
			new (new List<String>())
		);

		public List<String> ConversionSource {
			get => this.GetValue(SpecialRtonResourcePropertyPanel.ConversionSourceProperty).As<List<String>>();
			set => this.SetValue(SpecialRtonResourcePropertyPanel.ConversionSourceProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(SpecialRtonResourcePropertyPanel.Value),
			typeof(SpecialRtonResourceProperty),
			typeof(SpecialRtonResourcePropertyPanel),
			new (new SpecialRtonResourceProperty() {
				Conversion = "",
				Path = "",
			})
		);

		public SpecialRtonResourceProperty Value {
			get => this.GetValue(SpecialRtonResourcePropertyPanel.ValueProperty).As<SpecialRtonResourceProperty>();
			set => this.SetValue(SpecialRtonResourcePropertyPanel.ValueProperty, value);
		}

		#endregion

	}

	public class SpecialRtonResourcePropertyPanelController : CustomController {

		#region data

		public SpecialRtonResourcePropertyPanel View { get; init; } = default!;

		// ----------------

		public List<String> ConversionSource => this.View.ConversionSource;

		public SpecialRtonResourceProperty Value => this.View.Value;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
				nameof(this.uConversion_ItemsSource),
				nameof(this.uConversion_SelectedItem),
				nameof(this.uPath_Text)
			);
			return;
		}

		#endregion

		#region conversion

		public List<String> uConversion_ItemsSource {
			get {
				return this.ConversionSource;
			}
		}

		public String uConversion_SelectedItem {
			get {
				return this.Value.Conversion;
			}
		}

		public async void uConversion_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			this.Value.Conversion = senders.SelectedItem.As<String>();
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

	}

}
