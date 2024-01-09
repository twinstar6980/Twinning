#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Windows.Globalization.NumberFormatting;

namespace Helper.View.PackageBuilder {

	public sealed partial class TextureResourcePropertyPanel : CustomControl {

		#region life

		public TextureResourcePropertyPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private TextureResourcePropertyPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
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
			get => this.GetValue(TextureResourcePropertyPanel.ValueProperty).AsClass<TextureResourceProperty>();
			set => this.SetValue(TextureResourcePropertyPanel.ValueProperty, value);
		}

		#endregion

	}

	public class TextureResourcePropertyPanelController : CustomController {

		#region data

		public TextureResourcePropertyPanel View { get; init; } = default!;

		// ----------------

		public TextureResourceProperty Value => this.View.Value;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
				nameof(this.uPath_Text),
				nameof(this.uFormat_Value),
				nameof(this.uPitch_Value),
				nameof(this.uSizeWidth_Value),
				nameof(this.uSizeHeight_Value)
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

		#region format

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

		public async void uFormat_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.AsClass<NumberBox>();
			if (!Floater.IsNaN(args.NewValue)) {
				this.Value.Format = (Integer)args.NewValue;
			}
			this.NotifyPropertyChanged(
				nameof(this.uFormat_Value)
			);
			return;
		}

		#endregion

		#region pitch

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

		public async void uPitch_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.AsClass<NumberBox>();
			if (!Floater.IsNaN(args.NewValue)) {
				this.Value.Pitch = (Integer)args.NewValue;
			}
			this.NotifyPropertyChanged(
				nameof(this.uPitch_Value)
			);
			return;
		}

		#endregion

		#region additional byte count

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

		public async void uAdditionalByteCount_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.AsClass<NumberBox>();
			if (!Floater.IsNaN(args.NewValue)) {
				this.Value.AdditionalByteCount = (Integer)args.NewValue;
			}
			this.NotifyPropertyChanged(
				nameof(this.uAdditionalByteCount_Value)
			);
			return;
		}

		#endregion

		#region size

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

		public async void uSizeWidth_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.AsClass<NumberBox>();
			if (!Floater.IsNaN(args.NewValue)) {
				this.Value.Size[0] = (Integer)args.NewValue;
			}
			this.NotifyPropertyChanged(
				nameof(this.uSizeWidth_Value)
			);
			return;
		}

		// ----------------

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

		public async void uSizeHeight_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.AsClass<NumberBox>();
			if (!Floater.IsNaN(args.NewValue)) {
				this.Value.Size[1] = (Integer)args.NewValue;
			}
			this.NotifyPropertyChanged(
				nameof(this.uSizeHeight_Value)
			);
			return;
		}

		#endregion

	}

}
