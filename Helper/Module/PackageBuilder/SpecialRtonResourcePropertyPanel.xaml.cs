#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.PackageBuilder {

	public sealed partial class SpecialRtonResourcePropertyPanel : CustomControl {

		#region life

		public SpecialRtonResourcePropertyPanel (
		) {
			this.InitializeComponent();
			this.Controller = new SpecialRtonResourcePropertyPanelController() { View = this };
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
			new PropertyMetadata(new List<String>())
		);

		public List<String> ConversionSource {
			get => this.GetValue(SpecialRtonResourcePropertyPanel.ConversionSourceProperty).AsClass<List<String>>();
			set => this.SetValue(SpecialRtonResourcePropertyPanel.ConversionSourceProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(SpecialRtonResourcePropertyPanel.Value),
			typeof(SpecialRtonResourceProperty),
			typeof(SpecialRtonResourcePropertyPanel),
			new PropertyMetadata(new SpecialRtonResourceProperty() {
				Conversion = "",
				Path = "",
			})
		);

		public SpecialRtonResourceProperty Value {
			get => this.GetValue(SpecialRtonResourcePropertyPanel.ValueProperty).AsClass<SpecialRtonResourceProperty>();
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
			var senders = sender.AsClass<ComboBox>();
			this.Value.Conversion = senders.SelectedItem.AsClass<String>();
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

	}

}
