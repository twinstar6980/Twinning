#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.View.PackageBuilder {

	public sealed partial class SpecialPtxResourcePropertyPanel : CustomControl {

		#region life

		public SpecialPtxResourcePropertyPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private SpecialPtxResourcePropertyPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty ConversionSourceProperty = DependencyProperty.Register(
			nameof(SpecialPtxResourcePropertyPanel.ConversionSource),
			typeof(List<String>),
			typeof(SpecialPtxResourcePropertyPanel),
			new (new List<String>())
		);

		public List<String> ConversionSource {
			get => this.GetValue(SpecialPtxResourcePropertyPanel.ConversionSourceProperty).AsClass<List<String>>();
			set => this.SetValue(SpecialPtxResourcePropertyPanel.ConversionSourceProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(SpecialPtxResourcePropertyPanel.Value),
			typeof(SpecialPtxResourceProperty),
			typeof(SpecialPtxResourcePropertyPanel),
			new (new SpecialPtxResourceProperty() {
				Conversion = "",
				Path = "",
			})
		);

		public SpecialPtxResourceProperty Value {
			get => this.GetValue(SpecialPtxResourcePropertyPanel.ValueProperty).AsClass<SpecialPtxResourceProperty>();
			set => this.SetValue(SpecialPtxResourcePropertyPanel.ValueProperty, value);
		}

		#endregion

	}

	public class SpecialPtxResourcePropertyPanelController : CustomController {

		#region data

		public SpecialPtxResourcePropertyPanel View { get; init; } = default!;

		// ----------------

		public List<String> ConversionSource => this.View.ConversionSource;

		public SpecialPtxResourceProperty Value => this.View.Value;

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
