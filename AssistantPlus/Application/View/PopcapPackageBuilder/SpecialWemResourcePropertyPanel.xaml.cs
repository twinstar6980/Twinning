#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.PopcapPackageBuilder {

	public sealed partial class SpecialWemResourcePropertyPanel : CustomControl {

		#region life

		private SpecialWemResourcePropertyPanelController Controller { get; }

		// ----------------

		public SpecialWemResourcePropertyPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
			return;
		}

		// ----------------

		protected override async Task StampUpdate (
		) {
			await this.Controller.UpdateView();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty ConversionSourceProperty = DependencyProperty.Register(
			nameof(SpecialWemResourcePropertyPanel.ConversionSource),
			typeof(List<String>),
			typeof(SpecialWemResourcePropertyPanel),
			new (new List<String>())
		);

		public List<String> ConversionSource {
			get => this.GetValue(SpecialWemResourcePropertyPanel.ConversionSourceProperty).As<List<String>>();
			set => this.SetValue(SpecialWemResourcePropertyPanel.ConversionSourceProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(SpecialWemResourcePropertyPanel.Value),
			typeof(SpecialWemResourceProperty),
			typeof(SpecialWemResourcePropertyPanel),
			new (new SpecialWemResourceProperty() {
				Conversion = "",
				Path = "",
			})
		);

		public SpecialWemResourceProperty Value {
			get => this.GetValue(SpecialWemResourcePropertyPanel.ValueProperty).As<SpecialWemResourceProperty>();
			set => this.SetValue(SpecialWemResourcePropertyPanel.ValueProperty, value);
		}

		#endregion

	}

	public partial class SpecialWemResourcePropertyPanelController : CustomController {

		#region data

		public SpecialWemResourcePropertyPanel View { get; init; } = default!;

		// ----------------

		public List<String> ConversionSource => this.View.ConversionSource;

		public SpecialWemResourceProperty Value => this.View.Value;

		#endregion

		#region life

		public async Task UpdateView (
		) {
			this.NotifyPropertyChanged([
				nameof(this.uConversion_ItemsSource),
				nameof(this.uConversion_SelectedItem),
				nameof(this.uPath_Text),
			]);
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
			this.NotifyPropertyChanged([
				nameof(this.uPath_Text),
			]);
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
