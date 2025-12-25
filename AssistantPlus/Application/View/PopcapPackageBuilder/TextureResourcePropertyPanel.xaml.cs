#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using Windows.Globalization.NumberFormatting;

namespace Twinning.AssistantPlus.View.PopcapPackageBuilder {

	public sealed partial class TextureResourcePropertyPanel : CustomControl {

		#region life

		private TextureResourcePropertyPanelController Controller { get; }

		// ----------------

		public TextureResourcePropertyPanel (
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

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(TextureResourcePropertyPanel.Value),
			typeof(TextureResourceProperty),
			typeof(TextureResourcePropertyPanel),
			new (new TextureResourceProperty() {
				Path = "",
				Format = 0,
				Pitch = 0,
				AdditionalByteCount = 0,
				Size = [0, 0],
				Sprite = [],
			})
		);

		public TextureResourceProperty Value {
			get => this.GetValue(TextureResourcePropertyPanel.ValueProperty).As<TextureResourceProperty>();
			set => this.SetValue(TextureResourcePropertyPanel.ValueProperty, value);
		}

		#endregion

	}

	public partial class TextureResourcePropertyPanelController : CustomController {

		#region data

		public TextureResourcePropertyPanel View { get; init; } = default!;

		// ----------------

		public TextureResourceProperty Value => this.View.Value;

		#endregion

		#region life

		public async Task UpdateView (
		) {
			this.NotifyPropertyChanged([
				nameof(this.uPath_Text),
				nameof(this.uFormat_Value),
				nameof(this.uPitch_Value),
				nameof(this.uSizeWidth_Value),
				nameof(this.uSizeHeight_Value),
			]);
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

		#region format

		public async void uFormat_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value) && IntegerU32.MinValue <= senders.Value && senders.Value <= IntegerU32.MaxValue) {
				this.Value.Format = senders.Value.CastPrimitive<Integer>();
			}
			this.NotifyPropertyChanged([
				nameof(this.uFormat_Value),
			]);
			return;
		}

		public DecimalFormatter uFormat_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uFormat_Value {
			get {
				return this.Value.Format;
			}
		}

		#endregion

		#region pitch

		public async void uPitch_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value) && IntegerU32.MinValue <= senders.Value && senders.Value <= IntegerU32.MaxValue) {
				this.Value.Pitch = senders.Value.CastPrimitive<Integer>();
			}
			this.NotifyPropertyChanged([
				nameof(this.uPitch_Value),
			]);
			return;
		}

		public DecimalFormatter uPitch_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uPitch_Value {
			get {
				return this.Value.Pitch;
			}
		}

		#endregion

		#region additional byte count

		public async void uAdditionalByteCount_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value) && IntegerU32.MinValue <= senders.Value && senders.Value <= IntegerU32.MaxValue) {
				this.Value.AdditionalByteCount = senders.Value.CastPrimitive<Integer>();
			}
			this.NotifyPropertyChanged([
				nameof(this.uAdditionalByteCount_Value),
			]);
			return;
		}

		public DecimalFormatter uAdditionalByteCount_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uAdditionalByteCount_Value {
			get {
				return this.Value.AdditionalByteCount;
			}
		}

		#endregion

		#region size

		public async void uSizeWidth_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value) && IntegerU32.MinValue <= senders.Value && senders.Value <= IntegerU32.MaxValue) {
				this.Value.Size[0] = senders.Value.CastPrimitive<Integer>();
			}
			this.NotifyPropertyChanged([
				nameof(this.uSizeWidth_Value),
			]);
			return;
		}

		public DecimalFormatter uSizeWidth_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uSizeWidth_Value {
			get {
				return this.Value.Size[0];
			}
		}

		// ----------------

		public async void uSizeHeight_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value) && IntegerU32.MinValue <= senders.Value && senders.Value <= IntegerU32.MaxValue) {
				this.Value.Size[1] = senders.Value.CastPrimitive<Integer>();
			}
			this.NotifyPropertyChanged([
				nameof(this.uSizeHeight_Value),
			]);
			return;
		}

		public DecimalFormatter uSizeHeight_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uSizeHeight_Value {
			get {
				return this.Value.Size[1];
			}
		}

		#endregion

	}

}
