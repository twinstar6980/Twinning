#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.CustomControl {

	public sealed class MExpandBar : Control {

		#region life

		public MExpandBar (
		) {
			this.DefaultStyleKey = typeof(MExpandBar);
		}

		#endregion

		#region property

		public static readonly DependencyProperty IconProperty = DependencyProperty.Register(
			nameof(MExpandBar.Icon), typeof(String), typeof(MExpandBar),
			new PropertyMetadata("", (o, e) => {
			})
		);

		public String Icon {
			get => (String)this.GetValue(MExpandBar.IconProperty);
			set => this.SetValue(MExpandBar.IconProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
			nameof(MExpandBar.Title), typeof(String), typeof(MExpandBar),
			new PropertyMetadata("", (o, e) => {
			})
		);

		public String Title {
			get => (String)this.GetValue(MExpandBar.TitleProperty);
			set => this.SetValue(MExpandBar.TitleProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ActionProperty = DependencyProperty.Register(
			nameof(MExpandBar.Action), typeof(UIElement), typeof(MExpandBar),
			new PropertyMetadata(new MBox(), (o, e) => {
			})
		);

		public UIElement? Action {
			get => (UIElement)this.GetValue(MExpandBar.ActionProperty);
			set => this.SetValue(MExpandBar.ActionProperty, value);
		}

		#endregion

	}

}
