#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.PackageBuilder {

	public sealed partial class SpecialPamResourcePropertyPanel : CustomControl {

		#region life

		public SpecialPamResourcePropertyPanel (
		) {
			this.InitializeComponent();
			this.Controller = new SpecialPamResourcePropertyPanelController() { View = this };
		}

		// ----------------

		private SpecialPamResourcePropertyPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty ConversionSourceProperty = DependencyProperty.Register(
			nameof(SpecialPamResourcePropertyPanel.ConversionSource),
			typeof(List<String>),
			typeof(SpecialPamResourcePropertyPanel),
			new PropertyMetadata(new List<String>())
		);

		public List<String> ConversionSource {
			get => this.GetValue(SpecialPamResourcePropertyPanel.ConversionSourceProperty).AsClass<List<String>>();
			set => this.SetValue(SpecialPamResourcePropertyPanel.ConversionSourceProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(SpecialPamResourcePropertyPanel.Value),
			typeof(SpecialPamResourceProperty),
			typeof(SpecialPamResourcePropertyPanel),
			new PropertyMetadata(new SpecialPamResourceProperty() {
				Conversion = "",
				Path = "",
			})
		);

		public SpecialPamResourceProperty Value {
			get => this.GetValue(SpecialPamResourcePropertyPanel.ValueProperty).AsClass<SpecialPamResourceProperty>();
			set => this.SetValue(SpecialPamResourcePropertyPanel.ValueProperty, value);
		}

		#endregion

	}

	public class SpecialPamResourcePropertyPanelController : CustomController {

		#region data

		public SpecialPamResourcePropertyPanel View { get; init; } = default!;

		// ----------------

		public List<String> ConversionSource => this.View.ConversionSource;

		public SpecialPamResourceProperty Value => this.View.Value;

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
