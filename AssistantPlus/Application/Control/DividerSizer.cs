#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.Control {

	public sealed partial class DividerSizer : Microsoft.UI.Xaml.Controls.Control {

		#region life

		public DividerSizer (
		) {
			this.DefaultStyleKey = typeof(DividerSizer);
		}

		#endregion

		#region property

		public static readonly DependencyProperty SizerOrientationProperty = DependencyProperty.Register(
			nameof(DividerSizer.SizerOrientation),
			typeof(Orientation),
			typeof(DividerSizer),
			new (Orientation.Vertical)
		);

		public Orientation SizerOrientation {
			get => this.GetValue(DividerSizer.SizerOrientationProperty).As<Orientation>();
			set => this.SetValue(DividerSizer.SizerOrientationProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty SizerInvertedProperty = DependencyProperty.Register(
			nameof(DividerSizer.SizerInverted),
			typeof(Boolean),
			typeof(DividerSizer),
			new (false)
		);

		public Boolean SizerInverted {
			get => this.GetValue(DividerSizer.SizerInvertedProperty).As<Boolean>();
			set => this.SetValue(DividerSizer.SizerInvertedProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty SizerValueProperty = DependencyProperty.Register(
			nameof(DividerSizer.SizerValue),
			typeof(Floater),
			typeof(DividerSizer),
			new (Floater.NaN)
		);

		public Floater SizerValue {
			get => this.GetValue(DividerSizer.SizerValueProperty).As<Floater>();
			set => this.SetValue(DividerSizer.SizerValueProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty SizerMinimumProperty = DependencyProperty.Register(
			nameof(DividerSizer.SizerMinimum),
			typeof(Floater),
			typeof(DividerSizer),
			new (Floater.NaN)
		);

		public Floater SizerMinimum {
			get => this.GetValue(DividerSizer.SizerMinimumProperty).As<Floater>();
			set => this.SetValue(DividerSizer.SizerMinimumProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty SizerMaximumProperty = DependencyProperty.Register(
			nameof(DividerSizer.SizerMaximum),
			typeof(Floater),
			typeof(DividerSizer),
			new (Floater.NaN)
		);

		public Floater SizerMaximum {
			get => this.GetValue(DividerSizer.SizerMaximumProperty).As<Floater>();
			set => this.SetValue(DividerSizer.SizerMaximumProperty, value);
		}

		#endregion

	}

}
